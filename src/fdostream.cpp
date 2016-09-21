#include "cfriedt/fdostream.h"

using std::size_t;

using namespace ::std;
using namespace ::com::github::cfriedt;

fdostream::fdostream( int fd, size_t buffer_size )
:
	std::ios( 0 ),
	std::ostream( & buf ),
	buf( fd, buffer_size )
{
}

fdostream::fdostream()
:
	fdostream( -1, 0 )
{
}

fdostream::~fdostream() {
}

void fdostream::interrupt() {
	buf.interrupt();
}

fdostream & fdostream::operator=( const fdostream & other ) {
	if ( & other == this ) {
		goto out;
	}
	buf = other.buf;
	rdbuf( (streambuf *) & buf );
out:
	return *this;
}
