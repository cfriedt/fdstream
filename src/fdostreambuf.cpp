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

#include <algorithm>
#include <cstdint>

#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>

#include "cfriedt/fdostreambuf.h"

using std::size_t;

using namespace ::std;
using namespace ::com::github::cfriedt;

// adapted from http://www.mr-edd.co.uk/blog/beginners_guide_streambuf

fdostreambuf::fdostreambuf( int fd, size_t buffer_size )
:
	fd( fd ),
	buffer( ::max( buffer_size, (size_t) 1 ) )
{

	char_type *base;
	int r;

	base = &buffer.front();
	setp( base, base + buffer.size() - 1 );

	r = ::socketpair( AF_UNIX, SOCK_STREAM, 0, sv );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}
}

fdostreambuf::~fdostreambuf() {
	close_fds();
}

fdostreambuf::int_type fdostreambuf::overflow( int_type ch ) {

	int_type r;

	r = traits_type::eof();

    if ( ch == traits_type::eof() ) {
    	goto out;
    }

	if ( pptr() > epptr() ) {
		// assert(std::less_equal<char *>()(pptr(), epptr()));
		goto out;
	}

	*pptr() = ch;
	pbump( 1 );

	r = ch;

out:
    return r;
}

void fdostreambuf::interrupt() {
	int r;
	const uint8_t x = '!';

	r = ::write( sv[ fdostreambuf::INTERRUPTOR ], &x, sizeof( x ) );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}
}

void fdostreambuf::close_fds() {
	// TODO: add another public method to set whether the fd is automatically closed
//	if ( -1 != fd ) {
//		::close( fd );
//		fd = -1;
//	}
	if ( -1 != sv[ 0 ] ) {
		::close( sv[ 0 ] );
		sv[ 0 ] = -1;
	}
	if ( -1 != sv[ 1 ] ) {
		::close( sv[ 1 ] );
		sv[ 1 ] = -1;
	}
}

int fdostreambuf::sync() {

	int r;

	fd_set rfds;
	fd_set wfds;
	int nfds;
    std::ptrdiff_t n;

    n = pptr() - pbase();

    if ( 0 == n ) {
    	r = EXIT_SUCCESS;
    }

    FD_ZERO( & rfds );
    FD_SET( sv[ fdostreambuf::INTERRUPTEE ], & rfds );

    FD_ZERO( & rfds );
    FD_SET( fd, & wfds );

    nfds = ::max( fd, sv[ fdostreambuf::INTERRUPTEE ] ) + 1;

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
	if ( FD_ISSET( sv[ fdostreambuf::INTERRUPTEE ], & rfds ) ) {
		errno = EINTR;
		r = -1;
		goto rethrow;
	}
	if ( ! FD_ISSET( fd, & wfds ) ) {
		errno = EAGAIN;
		r = -1;
		goto rethrow;
	}

	r = ::write( fd, pbase(), n );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}
	n = r;
    pbump( -n );

    r = EXIT_SUCCESS;

    return r;
}


fdostreambuf & fdostreambuf::operator=( const fdostreambuf & other ) {
	if ( & other == this ) {
		goto out;
	}

	fd = other.fd;


out:
	return *this;
}
