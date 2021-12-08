#include<arpa/inet.h>
#include<sys/socket.h>
#include<iostream>
#include"formatting.hpp"
#include"Socket.hpp"

Socket::Socket(const std::string &addr, short port)
{
#ifdef _WIN32
#else
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	int succ = inet_aton(addr.c_str(), &saddr.sin_addr);
	if(succ != 0)
	{
		succ = connect(this->sockfd, reinterpret_cast<sockaddr*>(&saddr), sizeof(saddr));
		if(succ != 0)
			std::cerr << fmt_red_foreground << fmt_bold << "Error: " << fmt_reset << "Could not connect to server!" << std::endl;
	}
	else
		std::cerr << fmt_red_foreground << fmt_bold << "Error: " << fmt_reset << "IP address is invalid!" << std::endl;
	sockbuf *buf = new sockbuf(this->sockfd);
	this->iss = new isockstream(buf);
	this->oss = new osockstream(buf);
#endif
}

Socket::~Socket()
{
	delete this->iss;
	delete this->oss;
}

std::istream &Socket::getIstream()
{
	return *this->iss;
}

std::ostream &Socket::getOstream()
{
	return *this->oss;
}
