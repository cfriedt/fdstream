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

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <termios.h>

#include <iostream>
#include <cstring>

#include "cfriedt/fdstream.h"
#include "cfriedt/fdistream.h"

using namespace ::std;
using namespace ::com::github::cfriedt;

// XXX: @CF: can use fdistream or fdstream. either works
#define ISTYPE fdstream

/*
XXX: @CF: add some argv[] processing
#define DEFAULT_BAUD 115200
#define DEFAULT_DATA_BITS 8
#define DEFAULT_PARITY 'N'
#define DEFAULT_STOPBITS 1
*/

#pragma pack(push,1)
class blob {
public:
	static const int len = 9;
	uint8_t data[ len ];

	uint8_t & operator[]( const int index ) {
		return data[ index % len ];
	}
};
#pragma pack(pop)

ISTYPE & operator>>( ISTYPE & is, blob & b ) {
	is.read( (char *) b.data, b.len );
	return is;
}

std::ostream & operator<<( std::ostream & os, const std::vector<uint8_t> & vec ) {
	unsigned c;
	os << "[";
	for( int i = 0; i < vec.size(); i++ ) {
		if ( 0 == i ) {
			os << " ";
		}
		c = vec[ i ];
		os << std::hex << c;
		if ( i < vec.size() - 1 ) {
			os << ",";
		}
		os << " ";
	}
	os << "]";
	return os;
}

void usage( const char *progname ) {
	cout << "usage: " << progname << " /dev/ttyX" << endl;
	cout << endl;
	cout << "note: use Ctrl+C to quit" << endl;
}

void read_file_descriptor( ISTYPE &fds ) {

	blob blob;
	std::vector<uint8_t> vec;

	for(
		::memset( & blob, 0, sizeof( blob ) )
		;;
		::memset( & blob, 0, sizeof( blob ) )
	) {

		//fds.read( (char *) & blob, sizeof( blob ) );
		fds >> blob;

		cout.fill( '0' );
		cout.width( 2 );
		std::cout << "[ ";
		for( int i = 0; i < blob::len; i++ ) {
			std::cout << std::hex << (unsigned) blob.data[ i ];
			if ( i < blob::len - 1 ) {
				std::cout << ",";
			}
			std::cout << " ";
		}
		std::cout << "]" << std::endl;
	}
}

void setup( const char *fn, int *fd, ISTYPE &fds ) {

	int r;
	struct termios config;

	if (
		false
		|| -1 == ( r = open( fn, O_RDWR | O_NOCTTY ) )
		|| -1 == ( *fd = r )
		|| ! isatty( *fd )
		|| -1 == tcgetattr( *fd, & config )
	) {
		throw std::system_error( errno, std::system_category() );
	}

	config.c_iflag      &= ~( IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON );
	config.c_oflag       = 0;
	config.c_lflag      &= ~( ECHO | ECHONL | ICANON | IEXTEN | ISIG );
	config.c_cflag      &= ~( CSIZE | PARENB );
	config.c_cflag      |= CS8;
	config.c_cc[ VMIN ]  = 1;
	config.c_cc[ VTIME ] = 0;

	if (
		false
		|| -1 == cfsetspeed( &config, B115200 )
		|| -1 == tcsetattr( *fd, TCSAFLUSH, & config  )
	) {
		throw std::system_error( errno, std::system_category() );
	}

	fds = ISTYPE( *fd );
	fds.clear();
	fds.exceptions( std::ios::badbit | std::ios::failbit | std::ios::eofbit );
}

int main( int argc, char *argv[] ) {

	int r;

	char *fn;
	int fd;
	ISTYPE fds;

	if ( 2 != argc ) {
		usage( argv[ 0 ] );
		r = EXIT_SUCCESS;
		return r;
	}

	for( int i = 1; i < argc; i++ ) {
		if ( std::string( "--help" ) == argv[ i ] || std::string( "-h" ) == argv[ i ] ) {
			usage( argv[ 0 ] );
			r = EXIT_SUCCESS;
			return r;
		}
		fn = argv[ i ];
	}

	setup( fn, &fd, fds );
	read_file_descriptor( fds );

	r = EXIT_SUCCESS;

	return r;
}
