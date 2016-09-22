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

#include "cfriedt/fstream"
#include "cfriedt/fdstream.h"
#include "cfriedt/fdistreambuf.h"

namespace com {
namespace github {
namespace cfriedt {

class fdistream : public ::com::github::cfriedt::ifstream, public fdstream {

public:
	fdistream( int fd = -1, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary, bool auto_close = false, std::size_t buffer_size = 256, std::size_t put_back_size = 8 );
	virtual ~fdistream();

	fdistream & operator=( fdistream && __rhs );

protected:
	fdstreambuf & getBuf();

private:
	fdistreambuf buf;

};

}
}
} // com.github.cfriedt

#endif /* com_github_cfriedt_fdistream_h_ */
