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

#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <streambuf>

#include "cfriedt/fdstreambuf.h"

using namespace ::std;
using namespace ::com::github::cfriedt;

fdstreambuf::fdstreambuf( int fd, std::streamsize input_buffer_size, std::streamsize output_buffer_size, std::streamsize put_back_size )
:
	fd( fd ),
	put_back_size( std::max( put_back_size, std::streamsize( 1 ) ) ),
	ibuffer( std::max( input_buffer_size, std::streamsize( 1 ) ) ),
	obuffer( output_buffer_size + 1 ) // +1 means obuffer.size() >= 1, makes overflow easier
{
	char *base;
	base = & obuffer.front();
	setp( base, base + obuffer.size() - 1 ); // see above comment
}

fdstreambuf::~fdstreambuf()
{
	close_interrupt_fds();
}

fdstreambuf::fdstreambuf( const fdstreambuf & other )
:
	fd( other.fd ),
	put_back_size( other.put_back_size ),
	ibuffer( other.ibuffer ),
	obuffer( other.obuffer )
{
}
fdstreambuf::fdstreambuf( fdstreambuf && other ) noexcept
:
	fd( std::move( other.fd ) ),
	put_back_size( std::move( other.put_back_size ) ),
	ibuffer( std::move( other.ibuffer ) ),
	obuffer( std::move( other.obuffer ) )
{
}

fdstreambuf & fdstreambuf::operator=( const fdstreambuf & other )
{
	fdstreambuf tmp( other );
	*this = std::move( tmp );
	return *this;
}
fdstreambuf & fdstreambuf::operator=( fdstreambuf && other )
{
	swap( other );
	return *this;
}

void fdstreambuf::swap( fdstreambuf & other ) {
	::std::streambuf::swap( other );
	fd = std::move( other.fd );

	std::streamsize tmp;
	tmp = other.put_back_size;
	*( (std::streamsize *) & other.put_back_size ) = put_back_size;
	*( (std::streamsize *) & put_back_size ) = tmp;

	ibuffer = std::move( other.ibuffer );
	obuffer = std::move( other.obuffer );
}


int fdstreambuf::sync()
{
	int r;

	const int max_tries = 10;
	int tries;
	char *ch;
	int nw;

	fd_set rfds;
	fd_set wfds;
	int nfds;
    std::ptrdiff_t n;
    int fd;

    n = pptr() - pbase();
    if ( 0 == n ) {
    	// e.g. if this is a read-only buffer or if it is empty
    	return 0;
    }

    fd = get_fd();

    setup_interrupt_fds();

	for( ch = pbase(), nw = 0, tries = 0; n > 0 && tries < max_tries; ch += nw, n -= nw, tries++ ) {

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

		r = ::write( fd, ch, n );
		if ( -1 == r ) {
			throw std::system_error( errno, std::system_category() );
		}

		nw = r;
	}

	return 0;
}

std::streamsize fdstreambuf::showmanyc() {
	std::streamsize r;

	int rr;
	int fd;
	int nread;

	nread = 0;
	fd = get_fd();

	rr = ::ioctl( fd, FIONREAD, & nread );
	if ( -1 == rr ) {
		throw std::system_error( errno, std::system_category() );
	}

	r = nread;

	return r;
}

fdstreambuf::int_type fdstreambuf::underflow()
{
	int r;

	int nfds;
	fd_set rfds;
	char *base;
	char *start;
	int fd;

	if ( gptr() < egptr() ) {
		// buffer not exhausted
		return traits_type::to_int_type(*gptr());
	}

	base = & ibuffer.front();
	start = base;

	if ( eback() == base ) {
		// not the first fill. ensure there is room for the put back characters
		std::memmove( base, egptr() - put_back_size, put_back_size );
		start += put_back_size;
	}

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
		// timeout occurred (even though we did not specify one)
		// might be because file was opened non-blocking
		return traits_type::eof();
		//		errno = EAGAIN;
		//		r = -1;
		//		goto rethrow;
	}
	if ( FD_ISSET( sv[ fdstreambuf::INTERRUPTEE ], & rfds ) ) {
		errno = EINTR;
		r = -1;
		goto rethrow;
	}
	if ( ! FD_ISSET( fd, & rfds ) ) {
		return traits_type::eof();
//		errno = EAGAIN;
//		r = -1;
//		goto rethrow;
	}

	r = ::read( fd, start, ibuffer.size() - ( start - base ) );
	switch( r ) {
	case -1:
		throw std::system_error( errno, std::system_category() );
		break;
	case 0:
		return traits_type::eof();
		break;
	default:
		break;
	}

	setg( base, start, start + r );
	return traits_type::to_int_type( *gptr() );
}

fdstreambuf::int_type fdstreambuf::overflow( int_type ch )
{
	if ( traits_type::eof() != ch ) {
		*pptr() = ch;
		pbump( 1 );
		if ( 0 == sync() ) {
			// successful
			return ch;
		}
	}

	return traits_type::eof();
}

void fdstreambuf::interrupt()
{
	const char msg = '!';
	if ( -1 == ::write( sv[ INTERRUPTOR ], & msg, sizeof( msg ) ) ) {
		//throw std::system_error( errno, std::system_category() );
	}
}

int fdstreambuf::get_fd() {
	return fd;
}

std::streamsize fdstreambuf::getInputBufferSize() {
	return ibuffer.size();
}

std::streamsize fdstreambuf::getOutputBufferSize() {
	return obuffer.size() - 1;
}

std::streamsize fdstreambuf::getPutbackSize() {
	return put_back_size;
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
