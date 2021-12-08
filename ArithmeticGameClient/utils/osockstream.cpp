#include"osockstream.hpp"
osockstream::osockstream(std::streambuf *sbuf)
	:std::ostream(sbuf) {}
osockstream::~osockstream()=default;
