/*
 * MIT License
 *
 * Copyright (c) 2016 Christopher Friedt
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <cfriedt/fstream.h>
#include "cfriedt/fdstreambuf.h"

#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>

using ::com::github::cfriedt::filebuf;

using namespace ::std;
using namespace ::com::github::cfriedt;

fdstreambuf::fdstreambuf( int fd, std::ios_base::openmode mode, std::streamsize buffer_size  )
:
	filebuf( fd, mode )
{
	setbuf( 0, buffer_size );
}

fdstreambuf::~fdstreambuf() {
	close_interrupt_fds();
}

void fdstreambuf::swap( fdstreambuf & __rhs ) {
    filebuf::swap( __rhs );
}

int fdstreambuf::sync() {

	int r;

	fd_set rfds;
	fd_set wfds;
	int nfds;
    std::ptrdiff_t n;
    int fd;

    n = pptr() - pbase();

    if ( 0 == n ) {
    	r = EXIT_SUCCESS;
    }

    fd = get_fd();

    setup_interrupt_fds();

    FD_ZERO( & rfds );
    FD_SET( sv[ fdstreambuf::INTERRUPTEE ], & rfds );

    FD_ZERO( & rfds );
    FD_SET( fd, & wfds );

    nfds = ::max( fd, sv[ fdstreambuf::INTERRUPTEE ] ) + 1;

    r = ::select( nfds, & rfds, & wfds, NULL, NULL );
rethrow:
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}
	if ( 0 == r ) {
		errno = EAGAIN;
		r = -1;
		goto rethrow;
	}
	if ( FD_ISSET( sv[ fdstreambuf::INTERRUPTEE ], & rfds ) ) {
		errno = EINTR;
		r = -1;
		goto rethrow;
	}
	if ( ! FD_ISSET( fd, & wfds ) ) {
		errno = EAGAIN;
		r = -1;
		goto rethrow;
	}

	return filebuf::sync();
}


int fdstreambuf::underflow() {
	int r;
	int nfds;
	fd_set rfds;

	int fd;

	fd = get_fd();
	setup_interrupt_fds();

	FD_ZERO( & rfds );
	FD_SET( fd, & rfds );
	FD_SET( sv[ fdstreambuf::INTERRUPTEE ], & rfds );
	nfds = ::max( fd, sv[ fdstreambuf::INTERRUPTEE ] ) + 1;

	r = ::select( nfds, & rfds, NULL, NULL, NULL );
rethrow:
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}
	if ( 0 == r ) {
		errno = EAGAIN;
		r = -1;
		goto rethrow;
	}
	if ( FD_ISSET( sv[ fdstreambuf::INTERRUPTEE ], & rfds ) ) {
		errno = EINTR;
		r = -1;
		goto rethrow;
	}
	if ( ! FD_ISSET( fd, & rfds ) ) {
		errno = EAGAIN;
		r = -1;
		goto rethrow;
	}

	return filebuf::underflow();
}


void fdstreambuf::interrupt() {
	int r;
	const uint8_t x = '!';

	r = ::write( sv[ fdstreambuf::INTERRUPTOR ], &x, sizeof( x ) );
//	if ( -1 == r ) {
//		throw std::system_error( errno, std::system_category() );
//	}
}

int fdstreambuf::get_fd() {
	return __file_fd_;
}

void fdstreambuf::setup_interrupt_fds() {
	int r;
	int nclosed;

	// just to ensure that the sv[0:1] are either both open or both closed
	for( r = 0, nclosed = 0; r < 2; r++ ) {
		if ( -1 == sv[ r ] ) {
			close_interrupt_fds();
			break;
		}
	}

	if ( -1 == sv[ fdstreambuf::INTERRUPTEE ] ) {
		r = ::socketpair( AF_UNIX, SOCK_STREAM, 0, sv );
		if ( -1 == r ) {
			throw std::system_error( errno, std::system_category() );
		}
	}
}

void fdstreambuf::close_interrupt_fds() {
	for( int i = 0; i < 2; i++ ) {
		if ( -1 != sv[ i ] ) {
			::close( sv[ i ] );
			sv[ i ] = -1;
		}
	}
}
