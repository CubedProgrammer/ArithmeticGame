#include"opipestream.hpp"
opipestream::opipestream(pipebuf *sbuf)
	:std::ostream(sbuf) {}

opipestream::~opipestream()=default;
