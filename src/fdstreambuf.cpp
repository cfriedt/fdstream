#include "fdstreambuf.h"

#include <algorithm>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>

using std::size_t;

using namespace ::std;

fdstreambuf::fdstreambuf( int fd, size_t buffer_size, size_t put_back )
:
	fd( fd ),
	put_back( std::max( put_back, size_t( 1 ) ) ),
	buffer( std::max( buffer_size, put_back ) + put_back )
{
	char_type *end;
	end = &buffer.front() + buffer.size();
	setg( end, end, end );

	int r;
	r = ::socketpair( AF_UNIX, SOCK_STREAM, 0, sv );

}

fdstreambuf::~fdstreambuf() {
	if ( -1 != fd ) {
		::close( fd );
		fd = -1;
	}
	if ( -1 != sv[ 0 ] ) {
		::close( sv[ 0 ] );
		sv[ 0 ] = -1;
	}
	if ( -1 != sv[ 1 ] ) {
		::close( sv[ 1 ] );
		sv[ 1 ] = -1;
	}
}

fdstreambuf::int_type fdstreambuf::underflow() {
	fdstreambuf::int_type r;

	r = -1;

	return r;
}
