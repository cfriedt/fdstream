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

#include <gtest/gtest.h>

#include <signal.h>
#include <sys/time.h>


#include "cfriedt/fdistream.h"

using namespace ::std;
using namespace ::com::github::cfriedt;

class StdInTest : public testing::Test {

public:

	fdistream is;

	StdInTest();
	~StdInTest();

	void SetUp();
	void TearDown();
};

StdInTest::StdInTest()
{
}

StdInTest::~StdInTest() {
}

void StdInTest::SetUp() {
	// XXX: use dup(2) so that we don't inadvertantly close the last reference to stdout
	is = fdistream( ::dup( STDIN_FILENO ) );
}

void StdInTest::TearDown() {
}

static void interrupt_fdistream( fdistream *is ) {
	::sleep( 1 );
	is->interrupt();
}

TEST_F( StdInTest, InterruptAfterOneSecond ) {

	std::thread th( interrupt_fdistream, & this->is );

	try {

		std::string something;
		is >> something;

	} catch( ... ) {
	}

	th.join();
}
