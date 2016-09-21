#include <cstdio>

#include <gtest/gtest.h>

#include "cfriedt/fdostream.h"

using namespace ::std;
using namespace ::com::github::cfriedt;

class StdOutTest : public testing::Test {

public:

	fdostream os;

	StdOutTest();
	~StdOutTest();

	void SetUp();
	void TearDown();
};

StdOutTest::StdOutTest()
{
}

StdOutTest::~StdOutTest() {
}

void StdOutTest::SetUp() {
	os = fdostream( STDOUT_FILENO );
}

void StdOutTest::TearDown() {
}

TEST_F( StdOutTest, WriteHelloWorld ) {
	os << "Hello, World!" << std::endl;
}
