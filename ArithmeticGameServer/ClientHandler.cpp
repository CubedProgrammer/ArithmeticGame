#include<arpa/inet.h>
#include<sys/socket.h>
#include<iostream>
#include"ClientHandler.hpp"
ClientHandler::ClientHandler(short port)
	:saddr()
{
	using namespace std;
	this->serv = socket(AF_INET, SOCK_STREAM, 0);
	this->socklen = sizeof(this->saddr);
	this->saddr.sin_port = htons(port);
	this->saddr.sin_family = AF_INET;
	this->saddr.sin_addr.s_addr = INADDR_ANY;
	this->slenp = &this->socklen;
	int succ = bind(this->serv, reinterpret_cast<sockaddr*>(&this->saddr), this->socklen);
	if(succ == 0)
	{
		succ = listen(this->serv, 3);
		if(succ != 0)
			cerr << "Could not listen." << endl;
	}
	else
		cerr << "Could not bind." << endl;
}
