#include "cfriedt/fdistream.h"

using std::size_t;

using namespace ::std;
using namespace ::com::github::cfriedt;

fdistream::fdistream( int fd, size_t buffer_size )
:
	std::ios( 0 ),
	std::istream( & buf ),
	buf( fd, buffer_size )
{
}

fdistream::fdistream()
:
	fdistream( -1, 0 )
{
}

fdistream::~fdistream() {
}

void fdistream::interrupt() {
	buf.interrupt();
}

fdistream & fdistream::operator=( const fdistream & other ) {
	if ( & other == this ) {
		goto out;
	}
	buf = other.buf;
	rdbuf( (streambuf *) & buf );
out:
	return *this;
}
