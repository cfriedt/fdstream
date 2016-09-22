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

#include "cfriedt/fstream"
#include "cfriedt/fdistream.h"

using std::size_t;
using ::com::github::cfriedt::ifstream;

using namespace ::std;
using namespace ::com::github::cfriedt;

fdistream::fdistream( int fd, std::ios_base::openmode mode, bool auto_close, size_t buffer_size, size_t put_back_size )
:
	std::ios( 0 ),
	ifstream( buf ),
	fdstream( auto_close ),
	buf( fd, ( mode & ~std::ios_base::out ) | std::ios_base::in | std::ios_base::binary, buffer_size, put_back_size )
{
}

fdistream::~fdistream() {
}

fdistream & fdistream::operator=( fdistream && __rhs ) {
	buf.swap( __rhs.buf );
	return *this;
}

fdstreambuf & fdistream::getBuf() {
	return buf;
}
