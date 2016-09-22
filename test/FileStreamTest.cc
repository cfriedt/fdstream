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

#include <fstream>

#include <errno.h>
#include <sys/socket.h>

#include <gtest/gtest.h>

using namespace ::std;

class FileStreamTest : public testing::Test {

public:

	ofstream os;
	ifstream is;

	char *temp_file_name;

	FileStreamTest();
	~FileStreamTest();

	void SetUp();
	void TearDown();
};

FileStreamTest::FileStreamTest()
:
	temp_file_name( 0 )
{
}

FileStreamTest::~FileStreamTest() {
}

void FileStreamTest::SetUp() {

	temp_file_name = ::tmpnam( NULL );

	os = ofstream( temp_file_name, std::fstream::binary );
	is = ifstream( temp_file_name, std::fstream::binary );
}

void FileStreamTest::TearDown() {
	remove( temp_file_name );
}

TEST_F( FileStreamTest, PassMessage ) {

	std::string tx_msg = "Hi there!";
	char rx_msg_buf[ 64 ];
	std::string rx_msg;

	os << tx_msg;
	os.flush();
	// Note, std::string overloads the >> operator, and tokenizes string
	// input by default, which is why is.read() must be used instead
	is.read( rx_msg_buf, sizeof( rx_msg_buf ) );
	rx_msg = std::string( rx_msg_buf );

	EXPECT_EQ( tx_msg, rx_msg );
}

TEST_F( FileStreamTest, PassBinary ) {

	uint16_t tx_msg = 0x7e57;
	uint16_t rx_msg;

	os << tx_msg;
	os.flush();
	is >> rx_msg;

	EXPECT_EQ( tx_msg, rx_msg );
}
