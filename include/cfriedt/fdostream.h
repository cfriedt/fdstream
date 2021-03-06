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

#ifndef com_github_cfriedt_fdostream_h_
#define com_github_cfriedt_fdostream_h_

#include <ostream>

#include "cfriedt/fdstreambuf.h"

namespace com {
namespace github {
namespace cfriedt {

class fdostream : public ::std::ostream {

public:
	fdostream( int fd = -1, std::streamsize buffer_size = 256 )
	:
		::std::ostream( 0 ),
		buf( fd, 0, buffer_size )
	{
		rdbuf( & buf );
	}
	virtual ~fdostream() {}

	fdostream & operator=( fdostream && __rhs ) {
		::std::ostream::swap( __rhs );
		buf = std::move( __rhs.buf );
		return *this;
	}

	void interrupt() {
		buf.interrupt();
	}

	// 27.7.2.6 Formatted output:
    basic_ostream<char>& operator<<(bool __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_ostream<char>& operator<<(char __n) {
		write( (char *) & __n, sizeof( __n ) );
		return *this;
    }
    basic_ostream<char>& operator<<(unsigned char __n) {
		write( (char *) & __n, sizeof( __n ) );
		return *this;
    }
    basic_ostream<char>& operator<<(short __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_ostream<char>& operator<<(unsigned short __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_ostream<char>& operator<<(int __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_ostream<char>& operator<<(unsigned int __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_ostream<char>& operator<<(long __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_ostream<char>& operator<<(unsigned long __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_ostream<char>& operator<<(long long __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_ostream<char>& operator<<(unsigned long long __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_ostream<char>& operator<<(float __f) {
    	write( (char *) & __f, sizeof( __f ) );
    	return *this;
    }
    basic_ostream<char>& operator<<(double __f) {
    	write( (char *) & __f, sizeof( __f ) );
    	return *this;
    }
    basic_ostream<char>& operator<<(long double __f) {
    	write( (char *) & __f, sizeof( __f ) );
    	return *this;
    }
    basic_ostream<char>& operator<<(const void* __p) {
    	write( (char *) & __p, sizeof( __p ) );
    	return *this;
    }
    basic_ostream<char>& operator<<(const char * __str) {
    	write( __str, ::strlen( __str ) );
    	return *this;
    }

protected:
	fdstreambuf buf;
};

}
}
} // com.github.cfriedt

#endif /* com_github_cfriedt_fdostream_h_ */
