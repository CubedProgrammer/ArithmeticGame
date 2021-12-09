#ifdef _WIN32
#include<winsock2.h>
#include<ws2tcpip.h>
#else
#include<arpa/inet.h>
#include<sys/socket.h>
#endif
#include<iostream>
#include"formatting.hpp"
#include"Socket.hpp"

Socket::Socket(const std::string &addr, short port)
{
#ifdef _WIN32
	addrinfo addri, * addrp;
	ZeroMemory(&addri, sizeof(addri));
	addri.ai_family = AF_UNSPEC;
	addri.ai_socktype = SOCK_STREAM;
	addri.ai_protocol = IPPROTO_TCP;
	std::string portstr = std::to_string(port);
	int succ = getaddrinfo(addr.c_str(), portstr.c_str(), &addri, &addrp);
	if (succ == 0)
	{
		SOCKET serv = socket(addrp->ai_family, addrp->ai_socktype, addrp->ai_protocol);
		if (serv != INVALID_SOCKET)
		{
			succ = connect(serv, addrp->ai_addr, addrp->ai_addrlen);
			if (succ != 0)
				std::cerr << fmt_red_foreground << fmt_bold << "Error: " << fmt_reset << "Could not connect to server!" << std::endl;
			else
				this->sock = serv;
		}
		else
			std::cerr << fmt_red_foreground << fmt_bold << "Error: " << fmt_reset << "Socket could not be created!" << std::endl;
	}
	else
		std::cerr << fmt_red_foreground << fmt_bold << "Error: " << fmt_reset << "IP address is invalid!" << std::endl;
	sockbuf* buf = new sockbuf(this->sock);
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
#endif
	this->iss = new isockstream(buf);
	this->oss = new osockstream(buf);
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
