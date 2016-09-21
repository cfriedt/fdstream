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

#ifndef com_github_cfriedt_fdstreambuf_h_
#define com_github_cfriedt_fdstreambuf_h_

#include <streambuf>
#include <vector>

namespace com {
namespace github {
namespace cfriedt {

class fdstreambuf : public std::streambuf {

public:
	fdstreambuf( int fd, std::size_t buffer_size = 256);
	virtual ~fdstreambuf() = 0;

	void interrupt();
	int getFd();

protected:

	enum {
		INTERRUPTEE,
		INTERRUPTOR,
	};

	int fd;
    int sv[ 2 ];
	std::vector<char_type> buffer;

};

}
}
} // com.cfriedt.github

#include "cfriedt/fdistreambuf.h"
#include "cfriedt/fdostreambuf.h"

#endif /* com_github_cfriedt_fdstreambuf_h_ */
