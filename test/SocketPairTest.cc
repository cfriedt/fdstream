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

#include <cfriedt/fstream.h>
#include <cstdio>
#include <thread>

#include <errno.h>
#include <sys/socket.h>

#include <gtest/gtest.h>

#include "cfriedt/fdstreambuf.h"
#include "cfriedt/fdostream.h"
#include "cfriedt/fdistream.h"
#include "BaseTest.h"

#ifndef EOK
#define EOK 0
#endif

using namespace ::std;
using namespace ::com::github::cfriedt;

class SocketPairTest : public BaseTest {

public:

	enum {
		CLIENT,
		SERVER,
	};

	fdistream is;
	fdostream os;
	int sv[ 2 ];

	SocketPairTest() {}
	~SocketPairTest() {}

	void SetUpVirt();

	void interrupt_cb();
};

void SocketPairTest::SetUpVirt() {

	int r;
	r = socketpair( AF_UNIX, SOCK_STREAM, 0, sv );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}

	os = fdostream( sv[ CLIENT ] );
	is = fdistream( sv[ SERVER ] );
}

void SocketPairTest::interrupt_cb() {
	is.interrupt();
	is.exceptions( ios::badbit | ios::eofbit | ios::failbit );
	os.interrupt();
	os.exceptions( ios::badbit | ios::eofbit | ios::failbit );
}

TEST_F( SocketPairTest, SocketPairOK ) {

	std::string tx_msg = "Hi there!";
	char rx_msg_buf[ 64 ];
	std::string rx_msg;
	int r;

	fd_set rfds;
	struct timeval timeout;

	memset( rx_msg_buf, 0, sizeof( rx_msg_buf ) );

	r = ::write( sv[ CLIENT ], tx_msg.c_str(), tx_msg.length() );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}

	FD_ZERO( & rfds );
	FD_SET( sv[ SERVER ], & rfds );
	timeout.tv_sec = interrupt_delay_ms() / 1000;
	timeout.tv_usec = ( interrupt_delay_ms() % 1000 ) * 1000;
	r = select( sv[ SERVER ] + 1, & rfds, NULL, NULL, & timeout );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}

	EXPECT_NE( 0, r );
	EXPECT_NE( 0, FD_ISSET( sv[ SERVER ], & rfds ) );

	if ( 0 != r ) {

		r = ::read( sv[ SERVER ], rx_msg_buf, sizeof( rx_msg_buf ) );
		if ( -1 == r ) {
			throw std::system_error( errno, std::system_category() );
		}

	}
	rx_msg = std::string( rx_msg_buf );

	EXPECT_EQ( tx_msg, rx_msg );
}

TEST_F( SocketPairTest, CatchInterrupt ) {

	std::string something;
	std::system_error myerr = std::system_error( EOK, std::system_category() );

	int expected_int;
	int actual_int;

	try {

		is >> something;
		if ( ! is.good() ) {
			throw std::system_error( EINVAL, std::system_category() );
		}

	} catch( const std::system_error& e ) {
		myerr = e;
	}

	expected_int = EINTR;
	actual_int = myerr.code().value();

	EXPECT_EQ( something, "" );
	EXPECT_EQ( expected_int, actual_int );
}

TEST_F( SocketPairTest, IStreamOK ) {

	std::string tx_msg = "Hi there!";
	char rx_msg_buf[ 64 ];
	std::string rx_msg;
	int r;
	streamsize expected_streamsize;
	streamsize actual_streamsize;

	memset( rx_msg_buf, 0, sizeof( rx_msg_buf ) );

	r = ::write( sv[ CLIENT ], tx_msg.c_str(), tx_msg.length() );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}

	is.unsetf( ios::skipws );

	expected_streamsize = tx_msg.length();
	actual_streamsize = is.readsome( rx_msg_buf, sizeof( rx_msg_buf ) );
	EXPECT_EQ( expected_streamsize, actual_streamsize );

	rx_msg = std::string( rx_msg_buf );
	EXPECT_EQ( tx_msg, rx_msg );
}

TEST_F( SocketPairTest, IStreamByteByByteOK ) {

	std::string tx_msg = "Hi there!";
	char rx_msg_buf[ 64 ];
	std::string rx_msg;
	int r;

	memset( rx_msg_buf, 0, sizeof( rx_msg_buf ) );

	r = ::write( sv[ CLIENT ], tx_msg.c_str(), tx_msg.length() );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}

	is.unsetf( ios::skipws );

	for( int i = 0; i < tx_msg.length(); i++ ) {
		//std::cout << "reading character " << i << std::endl;
		is >> rx_msg_buf[ i ];
		//should_interrupt = false;
		//std::cout << "read char '" << (char)rx_msg_buf[ i ] << "'" << std::endl;
	}

	rx_msg = std::string( rx_msg_buf );
	EXPECT_EQ( tx_msg, rx_msg );
}

TEST_F( SocketPairTest, OStreamOK ) {

	std::string tx_msg = "Hi there!";
	char rx_msg_buf[ 64 ];
	std::string rx_msg;
	int r;

	fd_set rfds;
	struct timeval timeout;

	memset( rx_msg_buf, 0, sizeof( rx_msg_buf ) );

	os << tx_msg << std::flush;

	FD_ZERO( & rfds );
	FD_SET( sv[ SERVER ], & rfds );
	timeout.tv_sec = interrupt_delay_ms() / 1000;
	timeout.tv_usec = ( interrupt_delay_ms() % 1000 ) * 1000;
	r = select( sv[ SERVER ] + 1, & rfds, NULL, NULL, & timeout );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}

	EXPECT_NE( 0, r );
	EXPECT_NE( 0, FD_ISSET( sv[ SERVER ], & rfds ) );

	if ( 0 != r ) {

		r = ::read( sv[ SERVER ], rx_msg_buf, sizeof( rx_msg_buf ) );
		if ( -1 == r ) {
			throw std::system_error( errno, std::system_category() );
		}

	}
	rx_msg = std::string( rx_msg_buf );

	EXPECT_EQ( tx_msg, rx_msg );
}

TEST_F( SocketPairTest, PassBinary ) {

	uint16_t tx_msg = 0x7e57;
	uint16_t rx_msg;

	os << tx_msg << std::flush;
	is >> rx_msg;

	EXPECT_EQ( tx_msg, rx_msg );
}

TEST_F( SocketPairTest, PassMessage ) {

	std::string tx_msg = "Hi there!";
	char rx_msg_buf[ 64 ];
	std::string rx_msg;

	memset( rx_msg_buf, 0, sizeof( rx_msg_buf ) );

	os << tx_msg << std::flush;
	// Note, std::string overloads the >> operator and tokenizes string
	// input by default, which is why is.read() must be used instead
	is.read( rx_msg_buf, sizeof( rx_msg_buf ) );
	rx_msg = std::string( rx_msg_buf );

	EXPECT_EQ( tx_msg, rx_msg );
}


