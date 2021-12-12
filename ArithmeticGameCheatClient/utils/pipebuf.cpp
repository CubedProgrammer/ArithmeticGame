#include<unistd.h>
#include"pipebuf.hpp"
pipebuf::pipebuf(pipe_type p_in, pipe_type p_out)
	:p_in(p_in), p_out(p_out), ch() {}

pipebuf::~pipebuf()
{
#ifdef _WIN32
	CloseHandle(p_in);
	CloseHandle(p_out);
#else
	close(this->p_in);
	close(this->p_out);
#endif
}

int pipebuf::sync()
{
	return 0;
}

std::streamsize pipebuf::showmanyc()
{
	return 0;
}

pipebuf::int_type pipebuf::underflow()
{
	this->xsgetn(&this->ch, 1);
	this->setg(&this->ch, &this->ch, &this->ch + 1);
	return traits_type::to_int_type(this->ch);
}

pipebuf::int_type pipebuf::uflow()
{
	return this->underflow();
}

std::streamsize pipebuf::xsgetn(char_type *buf, std::streamsize sz)
{
#ifdef _WIN32
#else
	return read(this->p_out, buf, sz);
#endif
}

std::streamsize pipebuf::xsputn(const char_type *buf, std::streamsize sz)
{
#ifdef _WIN32
#else
	return write(this->p_in, buf, sz);
#endif
}

pipebuf::int_type pipebuf::overflow(int_type ch)
{
	char_type acc = ch;
	std::streamsize sent = this->xsputn(&acc, 1);
	return sent == 0 ? traits_type::eof() : 0;
}
