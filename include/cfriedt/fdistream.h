#ifndef com_github_cfriedt_fdistream_h_
#define com_github_cfriedt_fdistream_h_

#include <istream>

#include "cfriedt/fdistreambuf.h"

namespace com {
namespace github {
namespace cfriedt {

class fdistream : public std::istream {

public:
	fdistream();
	fdistream( int fd, std::size_t buffer_size = 256 );
	virtual ~fdistream();

	void interrupt();

	fdistream &operator=( const fdistream & other );

private:
    // copy ctor and assignment not implemented;
	fdistream( const fdistream & );

	fdistreambuf buf;

};

}
}
} // com.github.cfriedt

#endif /* com_github_cfriedt_fdistream_h_ */
