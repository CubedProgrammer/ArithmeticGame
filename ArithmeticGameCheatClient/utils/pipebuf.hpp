#ifndef UTILS_PIPEBUF_HPP_
#define UTILS_PIPEBUF_HPP_
#ifdef _WIN32
#include<windows.h>
typedef HANDLE pipe_type;
#else
typedef int pipe_type;
#endif
#include<streambuf>
class pipebuf:public virtual std::streambuf
{
	pipe_type p_in, p_out;
	char_type ch;
public:
	pipebuf(pipe_type p_in, pipe_type p_out);
	~pipebuf();
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
