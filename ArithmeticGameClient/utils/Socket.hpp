#ifndef UTILS_SOCKET_HPP_
#define UTILS_SOCKET_HPP_
#ifdef _WIN32
#include<windows.h>
#endif
#include<string>
#include"isockstream.hpp"
#include"osockstream.hpp"

class Socket
{
#ifdef _WIN32
	SOCKET sock;
#else
	int sockfd;
#endif
	isockstream *iss;
	osockstream *oss;
public:
	Socket(const std::string &addr, short port);
	Socket()
		:Socket("", 0){}
	~Socket();
	std::istream &getIstream();
	std::ostream &getOstream();
};

#endif
