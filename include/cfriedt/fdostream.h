#ifndef com_github_cfriedt_fdostream_h_
#define com_github_cfriedt_fdostream_h_

#include <ostream>

#include "cfriedt/fdostreambuf.h"

namespace com {
namespace github {
namespace cfriedt {

class fdostream : public std::ostream {

public:
	fdostream();
	fdostream( int fd, std::size_t buffer_size = 256 );
	virtual ~fdostream();

	void interrupt();

	fdostream &operator=( const fdostream & other );

private:
    // copy ctor and assignment not implemented;
    // copying not allowed
	fdostream( const fdostream & );

	fdostreambuf buf;

};

}
}
} // com.github.cfriedt

#endif /* com_github_cfriedt_fdostream_h_ */
