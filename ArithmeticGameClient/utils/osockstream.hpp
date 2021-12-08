#ifndef UTILS_OSOCKSTREAM_HPP_
#define UTILS_OSOCKSTREAM_HPP_
#include<ostream>
#include"sockbuf.hpp"
class osockstream:public virtual std::ostream
{
public:
	osockstream(std::streambuf *sbuf);
	~osockstream();
};
#endif
