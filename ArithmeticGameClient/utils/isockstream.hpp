#ifndef ISOCKSTREAM_HPP_
#define ISOCKSTREAM_HPP_
#include<istream>
#include"sockbuf.hpp"
class isockstream:public virtual std::istream
{
public:
	isockstream(std::streambuf *sbuf);
	~isockstream();
};
#endif
