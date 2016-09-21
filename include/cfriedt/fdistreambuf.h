#ifndef com_github_cfriedt_fdistreambuf_h_
#define com_github_cfriedt_fdistreambuf_h_

#include <streambuf>
#include <vector>

namespace com {
namespace github {
namespace cfriedt {

class fdistreambuf : public std::streambuf {

public:

	fdistreambuf( int fd, std::size_t buffer_size = 256, std::size_t put_back = 8 );
	virtual ~fdistreambuf();

	void interrupt();

    fdistreambuf & operator=( const fdistreambuf & other );

protected:
	int fd;
	const std::size_t put_back;
	std::vector<char_type> buffer;

	int_type underflow();

private:
    // copy ctor and assignment not implemented;
    fdistreambuf( const fdistreambuf & );

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

#endif /* com_github_cfriedt_fdistreambuf_h_ */
