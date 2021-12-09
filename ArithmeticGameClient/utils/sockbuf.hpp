#ifndef UTILS_SOCKBUF_HPP_
#define UTILS_SOCKBUF_HPP_
#ifdef _WIN32
#include<windows.h>
typedef SOCKET sock_type;
#else
typedef int sock_type;
#endif
#include<streambuf>
class sockbuf:public virtual std::streambuf
{
	sock_type sockfd;
	char_type* rbuf;
	std::streamsize bc;
public:
	sockbuf(sock_type sockfd);
	~sockbuf();
protected:
	virtual int sync()override;
	virtual std::streamsize showmanyc()override;
	virtual int_type underflow()override;
	virtual int_type uflow()override;
	virtual std::streamsize xsgetn(char_type *buf, std::streamsize sz)override;
	virtual std::streamsize xsputn(const char_type *buf, std::streamsize sz)override;
	virtual int_type overflow(int_type ch = traits_type::eof())override;
};
#endif
