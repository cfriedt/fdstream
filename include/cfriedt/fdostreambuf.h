#ifndef com_github_cfriedt_fdostreambuf_h_
#define com_github_cfriedt_fdostreambuf_h_

#include <streambuf>
#include <vector>

namespace com {
namespace github {
namespace cfriedt {

class fdostreambuf : public std::streambuf {

public:

	fdostreambuf( int fd, std::size_t buffer_size = 256 );
	virtual ~fdostreambuf();

	void interrupt();

    fdostreambuf &operator=( const fdostreambuf & other );

protected:
	int fd;
	std::vector<char_type> buffer;

	int_type overflow( int_type ch );
	int sync();

private:
    // copy ctor and assignment not implemented;
    // copying not allowed
	fdostreambuf( const fdostreambuf & );

    enum {
    	INTERRUPTEE,
		INTERRUPTOR,
    };

    int sv[ 2 ];

    void close_fds();
};

}
}
} // com.github.cfriedt

#endif /* com_github_cfriedt_fdostreambuf_h_ */
