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

#include <sys/socket.h>

#include <iostream>
#include <thread>

#include "cfriedt/fdstream.h"

using namespace ::std;
using namespace ::com::github::cfriedt;

void usage( const char *progname ) {
	cout << "usage: " << progname << endl;
	cout << endl;
	cout << "note: use Ctrl+C to quit" << endl;
}

class Node {
public:
	Node( int fd )
	:
		fds( fd ),
		should_cancel( false )
	{
		fds.exceptions( ios::badbit | ios::eofbit | ios::failbit );
	}
	~Node() {}

	fdstream fds;
	bool should_cancel;
};

void cancel_fn( Node *node, unsigned timeout_ms ) {
	usleep( timeout_ms * 1000 );
	if ( node->should_cancel ) {
		node->fds.interrupt();
	}
}

void converse( Node &server, Node & client ) {

	const std::string server_msg = "Buenos dias!";
	const std::string client_msg = "Moin, Moin!";
	char msg_buf[ 64 ];
	std::string msg;

	for( ;; ) {

		memset( msg_buf, 0, sizeof( msg_buf ) );
		std::thread server_cancel_th( cancel_fn, & server, 1000 );
		try {

			server.should_cancel = true;
			server.fds.read( msg_buf, sizeof( msg_buf ) );
			if ( ! server.fds.good() ) {
				throw std::system_error( EINVAL, std::system_category() );
			}
			server.should_cancel = false;
		} catch( std::system_error &e ) {
			if ( EINTR != e.code().value() ) {
				throw e;
			}
			server.fds.clear();
			cout << "SERVER: timed-out while reading" << endl;
		}
		server_cancel_th.join();

		msg = std::string( msg_buf );
		if ( ! msg.empty() ) {
			cout << "SERVER: received '" << msg << "'" << endl;
			cout << "SERVER: sending '" << server_msg << "'" << endl;
			server.fds << server_msg << std::flush;
		}

		memset( msg_buf, 0, sizeof( msg_buf ) );
		std::thread client_cancel_th( cancel_fn, & client, 1000 );
		try {

			client.should_cancel = true;
			client.fds.read( msg_buf, sizeof( msg_buf ) );
			if ( ! client.fds.good() ) {
				throw std::system_error( EINVAL, std::system_category() );
			}
			client.should_cancel = false;
		} catch( std::system_error &e ) {
			if ( EINTR != e.code().value() ) {
				throw e;
			}
			client.fds.clear();
			cout << "CLIENT: timed-out while reading" << endl;
		}
		client_cancel_th.join();

		msg = std::string( msg_buf );
		if ( ! msg.empty() ) {
			cout << "CLIENT: received '" << msg << "'" << endl;
		}

		cout << "CLIENT: sending '" << client_msg << "'" << endl;
		client.fds << client_msg << std::flush;
	}
}

void setup( int *sv ) {

	if (
		false
		|| -1 == socketpair( AF_UNIX, SOCK_STREAM, 0, sv )
	) {
		throw std::system_error( errno, std::system_category() );
	}
}

int main( int argc, char *argv[] ) {

	int r;

	int sv[2];

	for( int i = 1; i < argc; i++ ) {
		if ( std::string( "--help" ) == argv[ i ] || std::string( "-h" ) == argv[ i ] ) {
			usage( argv[ 0 ] );
			r = EXIT_SUCCESS;
			return r;
		}
	}

	setup( sv );
	Node server( sv[ 0 ] );
	cout << "server fd is " << sv[ 0 ] << endl;
	Node client( sv[ 1 ] );
	cout << "client fd is " << sv[ 1 ] << endl;
	converse( server, client );

	r = EXIT_SUCCESS;

	return r;
}
