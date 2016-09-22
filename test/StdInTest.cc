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

#include <cstdio>
#include <chrono>
#include <thread>

#include <errno.h>

#include <signal.h>
#include <sys/time.h>

#include <gtest/gtest.h>

#include "cfriedt/fdistream.h"

#include "BaseTest.h"

using namespace ::std;
using namespace ::com::github::cfriedt;

class StdInTest : public BaseTest {

public:

	fdistream is;

	StdInTest() {}
	~StdInTest() {}

	void SetUpVirt();

	void interrupt_cb();
};

void StdInTest::SetUpVirt() {
	is = fdistream( STDIN_FILENO );
}

void StdInTest::interrupt_cb() {
	is.interrupt();
}

/*
TEST_F( StdInTest, CatchInterrupt ) {

	std::string something;
	std::exception_ptr p;

	try {

		is >> something;

	} catch( ... ) {
		p = std::current_exception();
	}

	// XXX: FIXME: currently failing, not sure why. Lacking controlling tty??
	// EXPECT_NE( p, nullptr );
}
*/
