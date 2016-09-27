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

#include <unistd.h>

#include <gtest/gtest.h>

#include "cfriedt/fdostream.h"

#include "BaseTest.h"

using namespace ::std;
using namespace ::com::github::cfriedt;

class StdOutTest : public BaseTest {

public:

	fdostream os;

	StdOutTest() {}
	~StdOutTest() {}

	void SetUpVirt();

	void interrupt_cb();
};

void StdOutTest::SetUpVirt() {
	os = fdostream( STDOUT_FILENO );
}

void StdOutTest::interrupt_cb() {
//	os.interrupt();
}

TEST_F( StdOutTest, IsCorrectFileNo ) {
	int expected_int;
	int actual_int;

	expected_int = STDOUT_FILENO;
	actual_int = ( (fdstreambuf *) os.rdbuf() )->get_fd();

	EXPECT_EQ( expected_int, actual_int );
}

TEST_F( StdOutTest, IsOpen ) {
	bool expected_bool;
	bool actual_bool;

	expected_bool = true;
	actual_bool = os.is_open();

	EXPECT_EQ( expected_bool, actual_bool );
}

TEST_F( StdOutTest, WriteHelloWorld ) {

	bool expected_bool;
	bool actual_bool;

	const std::string str = "Hello, World!\n";
	const char *cstr = str.c_str();
	const std::size_t strsz = str.length();
	os.write( cstr, strsz );
	os.flush();

//	::write( STDOUT_FILENO, cstr, strsz );

	expected_bool = false;
	actual_bool = os.badbit;
	EXPECT_EQ( expected_bool, actual_bool );

//	os << str;
//	os.flush();
}
