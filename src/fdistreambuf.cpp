#include <algorithm>
#include <sstream>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>

#include "cfriedt/fdistreambuf.h"

using std::size_t;

using namespace ::std;
using namespace ::com::github::cfriedt;

// derived from http://www.mr-edd.co.uk/blog/beginners_guide_streambuf

fdistreambuf::fdistreambuf( int fd, size_t buffer_size, size_t put_back )
:
	fd( fd ),
	put_back( std::max( put_back, size_t( 1 ) ) ),
	buffer( std::max( buffer_size, put_back ) + put_back )
{

	char_type *end;
	int r;

	end = &buffer.front() + buffer.size();
	setg( end, end, end );

	r = ::socketpair( AF_UNIX, SOCK_STREAM, 0, sv );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}
}

fdistreambuf::~fdistreambuf() {
	close_fds();
}

void fdistreambuf::interrupt() {
	int r;
	const uint8_t x = '!';

	r = ::write( sv[ fdistreambuf::INTERRUPTOR ], &x, sizeof( x ) );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}
}

std::streambuf::int_type fdistreambuf::underflow() {

	int r;
	int nfds;
	fd_set rfds;

	if ( gptr() < egptr() ) { // buffer not exhausted
		return traits_type::to_int_type( *gptr() );
	}

	char *base = &buffer.front();
	char *start = base;

	if ( eback() == base ) { // true when this isn't the first fill
		// Make arrangements for putback characters
		::memmove( base, egptr() - put_back, put_back );
		start += put_back;
	}

	FD_ZERO( & rfds );
	FD_SET( fd, & rfds );
	FD_SET( sv[ fdistreambuf::INTERRUPTEE ], & rfds );
	nfds = ::max( fd, sv[ fdistreambuf::INTERRUPTEE ] ) + 1;

	r = ::select( nfds, & rfds, NULL, NULL, NULL );
rethrow:
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}
	if ( 0 == r ) {
		errno = EAGAIN;
		r = -1;
		goto rethrow;
	}
	if ( FD_ISSET( sv[ fdistreambuf::INTERRUPTEE ], & rfds ) ) {
		errno = EINTR;
		r = -1;
		goto rethrow;
	}
	if ( ! FD_ISSET( fd, & rfds ) ) {
		errno = EAGAIN;
		r = -1;
		goto rethrow;
	}

	// start is now the start of the buffer, proper.
	// Read from fd into the provided buffer
	r = ::read( fd, start, buffer.size() - ( start - base ) );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}
	if ( 0 == r ) {
		return traits_type::eof();
	}

	// Set buffer pointers
	setg( base, start, start + r );

	return traits_type::to_int_type( *gptr() );
}

void fdistreambuf::close_fds() {
	// TODO: add another public method to set whether the fd is automatically closed
//	if ( -1 != fd ) {
//		::close( fd );
//		fd = -1;
//	}
	if ( -1 != sv[ 0 ] ) {
		::close( sv[ 0 ] );
		sv[ 0 ] = -1;
	}
	if ( -1 != sv[ 1 ] ) {
		::close( sv[ 1 ] );
		sv[ 1 ] = -1;
	}
}

fdistreambuf & fdistreambuf::operator=( const fdistreambuf & other ) {
	if ( & other == this ) {
		goto out;
	}

	fd = other.fd;
	*( (size_t *) & put_back ) = other.put_back;


out:
	return *this;
}
