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

class fdstreambuf : public ::std::streambuf {

public:
	fdstreambuf( int fd = -1, std::streamsize input_buffer_size = 256, std::streamsize output_buffer_size = 256, std::streamsize put_back_size = 16 );

	virtual ~fdstreambuf();

	fdstreambuf( const fdstreambuf & other );
	fdstreambuf( fdstreambuf && other ) noexcept;

	fdstreambuf & operator=( const fdstreambuf & other );
	fdstreambuf & operator=( fdstreambuf && other );

	void swap( fdstreambuf & other );

	// 27.6.2.4.2 Buffer management and positioning:
	int sync();

    // 27.6.2.4.3 Get area:
	std::streamsize showmanyc();
	int_type underflow();

    // 27.6.2.4.5 Put area
	int_type overflow( int_type ch = traits_type::eof() );

	void interrupt();
	int get_fd();

protected:

	int fd;
    const std::streamsize put_back_size;
    std::vector<char> ibuffer;
    std::vector<char> obuffer;

    void setup_interrupt_fds();
    void close_interrupt_fds();

private:
	enum {
		INTERRUPTEE,
		INTERRUPTOR,
	};
    int sv[ 2 ] = { -1, -1, };
};

}
}
} // com.cfriedt.github

#endif /* com_github_cfriedt_fdstreambuf_h_ */
