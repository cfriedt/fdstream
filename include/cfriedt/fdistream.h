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

#ifndef com_github_cfriedt_fdistream_h_
#define com_github_cfriedt_fdistream_h_

#include "cfriedt/fdstreambuf.h"

#include <istream>

#include "cfriedt/fdstreambuf.h"

namespace com {
namespace github {
namespace cfriedt {

class fdistream : public ::std::istream {

public:
	fdistream( int fd = -1, std::streamsize buffer_size = 256, std::streamsize put_back_size = 16 )
	:
		::std::istream( 0 ),
		buf( fd, buffer_size, 0, put_back_size )
	{
		rdbuf( & buf );
	}
	virtual ~fdistream() {}

	fdistream & operator=( fdistream && __rhs ) {
		::std::istream::swap( __rhs );
		buf.swap( __rhs.buf );
		return *this;
	}

	void interrupt() {
		buf.interrupt();
	}

    // 27.7.1.2 Formatted input:
    basic_istream& operator>>(bool& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
	basic_istream& operator>>(char& __n) {
		read( (char *)&__n, sizeof( __n ) );
		return *this;
	}
	basic_istream& operator>>(unsigned char& __n) {
		read( (char *)&__n, sizeof( __n ) );
		return *this;
	}
    basic_istream& operator>>(short& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
    basic_istream& operator>>(unsigned short& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
    basic_istream& operator>>(int& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
    basic_istream& operator>>(unsigned int& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
    basic_istream& operator>>(long& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
    basic_istream& operator>>(unsigned long& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
    basic_istream& operator>>(long long& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
    basic_istream& operator>>(unsigned long long& __n) {
    	read( (char *)&__n, sizeof( __n ) );
    	return *this;
    }
    basic_istream& operator>>(float& __f) {
    	read( (char *)&__f, sizeof( __f ) );
    	return *this;
    }
    basic_istream& operator>>(double& __f) {
    	read( (char *)&__f, sizeof( __f ) );
    	return *this;
    }
    basic_istream& operator>>(long double& __f) {
    	read( (char *)&__f, sizeof( __f ) );
    	return *this;
    }
    basic_istream& operator>>(void*& __p) {
    	read( (char *)&__p, sizeof( __p ) );
    	return *this;
    }

protected:
	fdstreambuf buf;
};

}
}
} // com.github.cfriedt

#endif /* com_github_cfriedt_fdistream_h_ */
