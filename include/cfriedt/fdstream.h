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

#ifndef com_github_cfriedt_fdstream_h_
#define com_github_cfriedt_fdstream_h_

#include "cfriedt/fdstreambuf.h"
#include "fstream.h"

namespace com {
namespace github {
namespace cfriedt {

class fdstream : public ::com::github::cfriedt::fstream {

public:
	fdstream( int fd = -1, std::ios::openmode mode = std::ios::in | std::ios::out )
	:
		fstream( & buf ),
		buf( fd, mode )
	{
	}
	virtual ~fdstream() {}

	fdstream & operator=( fdstream && __rhs ) {
		fstream::operator=( std::move( __rhs ) );
		buf.swap( __rhs.buf );
		return *this;
	}

	void interrupt() {
		buf.interrupt();
	}

	// XXX: @CF: for some reason binary values are not read as binary (for arbitrary FD's) but they are for files
	// That is a bug that should eventually be fixed. The >> overloads are a work-around for that.
	// XXX: @CF: Also, note, that they use native endianness for multibyte values, also should be configurable.

    // 27.7.1.2 Formatted input:
    basic_iostream& operator>>(bool& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream& operator>>(short& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream& operator>>(unsigned short& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream& operator>>(int& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream& operator>>(unsigned int& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream& operator>>(long& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream& operator>>(unsigned long& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream& operator>>(long long& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream& operator>>(unsigned long long& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream& operator>>(float& __f) {
    	read( (char *)&__f, sizeof( __f ) );
    	return *this;
    }
    basic_iostream& operator>>(double& __f) {
    	read( (char *)&__f, sizeof( __f ) );
    	return *this;
    }
    basic_iostream& operator>>(long double& __f) {
    	read( (char *)&__f, sizeof( __f ) );
    	return *this;
    }
    basic_iostream& operator>>(void*& __p) {
    	read( (char *)&__p, sizeof( __p ) );
    	return *this;
    }

	// XXX: @CF: for some reason binary values are not written as binary (for arbitrary FD's) but they are for files
	// That is a bug that should eventually be fixed. The << overloads are a work-around for that.
    // XXX: @CF: Also, note, that they use native endianness for multibyte values, also should be configurable.

	// 27.7.2.6 Formatted output:
    basic_iostream<char>& operator<<(bool __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream<char>& operator<<(short __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream<char>& operator<<(unsigned short __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream<char>& operator<<(int __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream<char>& operator<<(unsigned int __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream<char>& operator<<(long __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream<char>& operator<<(unsigned long __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream<char>& operator<<(long long __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream<char>& operator<<(unsigned long long __n) {
    	write( (char *) & __n, sizeof( __n ) );
    	return *this;
    }
    basic_iostream<char>& operator<<(float __f) {
    	write( (char *) & __f, sizeof( __f ) );
    	return *this;
    }
    basic_iostream<char>& operator<<(double __f) {
    	write( (char *) & __f, sizeof( __f ) );
    	return *this;
    }
    basic_iostream<char>& operator<<(long double __f) {
    	write( (char *) & __f, sizeof( __f ) );
    	return *this;
    }
    basic_iostream<char>& operator<<(const void* __p) {
    	write( (char *) & __p, sizeof( __p ) );
    	return *this;
    }
    basic_iostream<char>& operator<<(const char * __str) {
    	write( __str, ::strlen( __str ) );
    	return *this;
    }

protected:
	fdstreambuf buf;
};

}
}
} // com.github.cfriedt

#endif /* com_github_cfriedt_fdstream_h_ */
