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

#include "cfriedt/fdstream.h"

using namespace ::std;
using namespace ::com::github::cfriedt;

/*
XXX: @CF: add some argv[] processing
#define DEFAULT_BAUD 115200
#define DEFAULT_DATA_BITS 8
#define DEFAULT_PARITY 'N'
#define DEFAULT_STOPBITS 1
*/

void usage( const char *progname ) {
	cout << "usage: " << progname << " /dev/ttyX" << endl;
	cout << endl;
	cout << "note: use Ctrl+C to quit" << endl;
}

void read_file_descriptor( int fd ) {

	fdstream fds;
	ios::fmtflags default_fmt;

	fds = fdstream( fd );
	uint8_t byte;

	default_fmt = cout.flags();

	for( cout.flags( default_fmt ); ; cout.flags( default_fmt ) ) {

		fds >> byte;

		std::cout << "received byte 0x";
		cout.width( 2 );
		cout.fill('0');
		cout << std::hex << (unsigned)byte << std::endl;
	}
}

void setup( const char *fn, int *fd ) {

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
}

int main( int argc, char *argv[] ) {

	int r;

	char *fn;
	int fd;

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

	setup( fn, &fd );
	read_file_descriptor( fd );

	r = EXIT_SUCCESS;

	return r;
}
