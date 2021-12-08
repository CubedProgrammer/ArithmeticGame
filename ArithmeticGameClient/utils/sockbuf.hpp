#ifndef UTILS_SOCKBUF_HPP_
#define UTILS_SOCKBUF_HPP_
#include<streambuf>
class sockbuf:public virtual std::streambuf
{
#ifdef _WIN32
	HANDLE sock;
#else
	int sockfd;
	char_type *rbuf;
	std::streamsize bc;
#endif
public:
	sockbuf(int sockfd);
	~sockbuf();
protected:
	virtual int sync()override;
	virtual std::streamsize showmanyc()override;
	virtual std::streamsize xsgetn(char_type *buf, std::streamsize sz)override;
	virtual std::streamsize xsputn(const char_type *buf, std::streamsize sz)override;
	virtual int_type overflow(int_type ch = traits_type::eof())override;
};
#endif
