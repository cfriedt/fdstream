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
#include <sstream>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>

#include "cfriedt/fdstreambuf.h"

using std::size_t;

using namespace ::std;
using namespace ::com::github::cfriedt;

// derived from http://www.mr-edd.co.uk/blog/beginners_guide_streambuf

fdistreambuf::fdistreambuf( int fd, std::ios_base::openmode mode )
:
	fdstreambuf( fd, mode )
{
}

fdistreambuf::~fdistreambuf() {
}

std::streambuf::int_type fdistreambuf::underflow() {
	return traits_type::eof();
/*
	int r;
	int nfds;
	fd_set rfds;

	if ( gptr() < egptr() ) { // buffer not exhausted
		return traits_type::to_int_type( *gptr() );
	}

	char *base = &buffer.front();
	char *start = base;

	if ( eback() == base ) { // true when this isn't the first fill
		// Make arrangements for putback characters
		::memmove( base, egptr() - put_back, put_back );
		start += put_back;
	}

	FD_ZERO( & rfds );
	FD_SET( fd, & rfds );
	FD_SET( sv[ fdistreambuf::INTERRUPTEE ], & rfds );
	nfds = ::max( fd, sv[ fdistreambuf::INTERRUPTEE ] ) + 1;

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
	if ( FD_ISSET( sv[ fdistreambuf::INTERRUPTEE ], & rfds ) ) {
		errno = EINTR;
		r = -1;
		goto rethrow;
	}
	if ( ! FD_ISSET( fd, & rfds ) ) {
		errno = EAGAIN;
		r = -1;
		goto rethrow;
	}

	// start is now the start of the buffer, proper.
	// Read from fd into the provided buffer
	r = ::read( fd, start, buffer.size() - ( start - base ) );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}
	if ( 0 == r ) {
		return traits_type::eof();
	}

	// Set buffer pointers
	setg( base, start, start + r );

	return traits_type::to_int_type( *gptr() );
	*/
}
