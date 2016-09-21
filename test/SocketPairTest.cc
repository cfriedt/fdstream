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
#include <thread>

#include <errno.h>
#include <sys/socket.h>

#include <gtest/gtest.h>

#include "cfriedt/fdstream.h"

using namespace ::std;
using namespace ::com::github::cfriedt;

class SocketPairTest : public testing::Test {

public:

	enum {
		CLIENT,
		SERVER,
	};

	fdistream is;
	fdostream os;
	int sv[ 2 ];

	SocketPairTest();
	~SocketPairTest();

	void SetUp();
	void TearDown();
};

SocketPairTest::SocketPairTest()
{
}

SocketPairTest::~SocketPairTest() {
}

void SocketPairTest::SetUp() {

	int r;
	r = socketpair( AF_UNIX, SOCK_STREAM, 0, sv );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}

	os = fdostream( sv[ CLIENT ] );
	is = fdistream( sv[ SERVER ] );
}

void SocketPairTest::TearDown() {
}

static void interrupt_test( SocketPairTest *spt ) {
	::sleep( 1 );
	spt->is.interrupt();
	spt->os.interrupt();
}

TEST_F( SocketPairTest, PassMessage ) {

	std::string tx_msg = "Hi there!";
	char rx_msg_buf[ 64 ];
	std::string rx_msg;

	memset( rx_msg_buf, 0, sizeof( rx_msg_buf ) );

	std::thread th( interrupt_test, this );

	try {
		os << tx_msg;
		os.flush();
		is.read( rx_msg_buf, sizeof( rx_msg_buf ) );
		rx_msg = std::string( rx_msg_buf );

		EXPECT_EQ( tx_msg, rx_msg );

	} catch( ... ) {
		EXPECT_EQ( true, false );
	}

	th.join();
}

TEST_F( SocketPairTest, PassBinary ) {

	uint16_t tx_msg = 0x7e57;
	uint16_t rx_msg;

	std::thread th( interrupt_test, this );

	try {
		os << tx_msg;
		os.flush();
		is >> rx_msg;

		EXPECT_EQ( tx_msg, rx_msg );

	} catch( ... ) {
		EXPECT_EQ( true, false );
	}

	th.join();
}
