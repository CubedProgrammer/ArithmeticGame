#ifndef UTILS_IPIPESTREAM_HPP_
#define UTILS_IPIPESTREAM_HPP_
#include<istream>
#include"pipebuf.hpp"
class ipipestream:public virtual std::istream
{
public:
	ipipestream(pipebuf *sbuf);
	~ipipestream();
};
#endif
