#ifndef FDSTREAMBUF_H_
#define FDSTREAMBUF_H_

#include <streambuf>
#include <vector>

class fdstreambuf : public std::streambuf {

public:

	fdstreambuf( int fd, std::size_t buffer_size = 256, std::size_t put_back = 8 );
	virtual ~fdstreambuf();

	void interrupt();

protected:
	int fd;
	const std::size_t put_back;
	std::vector<char_type> buffer;

	int_type underflow();

private:
    // copy ctor and assignment not implemented;
    // copying not allowed
    fdstreambuf(const fdstreambuf &);
    fdstreambuf &operator= (const fdstreambuf &);

    enum {
    	INTERRUPTEE,
		INTERRUPTOR,
    };

    int sv[ 2 ];
};

#endif /* INCLUDE_FDSTREAMBUF_H_ */
