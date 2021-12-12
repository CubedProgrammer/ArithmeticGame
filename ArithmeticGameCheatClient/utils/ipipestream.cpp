#include"ipipestream.hpp"
ipipestream::ipipestream(pipebuf *sbuf)
	:std::istream(sbuf) {}

ipipestream::~ipipestream()
{
	delete this->rdbuf();
}
