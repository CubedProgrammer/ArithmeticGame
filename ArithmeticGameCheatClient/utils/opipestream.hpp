#ifndef UTILS_OPIPESTREAM_HPP_
#define UTILS_OPIPESTREAM_HPP_
#include<ostream>
#include"pipebuf.hpp"
class opipestream:public virtual std::ostream
{
public:
	opipestream(pipebuf *sbuf);
	~opipestream();
};
#endif
