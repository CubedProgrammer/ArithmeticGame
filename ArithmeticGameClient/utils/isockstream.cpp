#include"isockstream.hpp"
isockstream::isockstream(std::streambuf *sbuf)
	:std::istream(sbuf) {}
isockstream::~isockstream()
{
	delete this->rdbuf();
}
