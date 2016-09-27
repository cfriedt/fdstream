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

#include <vector>

#include "cfriedt/fstream"

namespace com {
namespace github {
namespace cfriedt {

class fdstreambuf : public ::com::github::cfriedt::filebuf {

public:
	fdstreambuf( int fd = -1, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out | std::ios_base::binary, std::streamsize buffer_size = 256 );
	virtual ~fdstreambuf();

	void interrupt();
	int get_fd();

	void swap( fdstreambuf & __rhs );

	int_type underflow();
	int_type overflow( int_type ch );
	int sync();

protected:

	enum {
		INTERRUPTEE,
		INTERRUPTOR,
	};

    int sv[ 2 ] = { -1, -1, };

    void setup_interrupt_fds();
    void close_interrupt_fds();
};

}
}
} // com.cfriedt.github

#endif /* com_github_cfriedt_fdstreambuf_h_ */
