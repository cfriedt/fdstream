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
	is = fdistream( STDIN_FILENO );
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
