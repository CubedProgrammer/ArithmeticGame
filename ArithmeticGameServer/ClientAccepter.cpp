#include"ClientAccepter.hpp"
#include"utils.hpp"
#include<iostream>
int ClientAccepter::accept_cli(bool &cr, int &fd, std::string &name)
{
	int cli = accept(this->serv, reinterpret_cast<sockaddr*>(&this->saddr), this->slenp);
	int succ = cli == -1;
	if(succ == 0)
	{
		fd = cli;
		char c;
		fdget_obj(cli, c);
		if(c != 13 && c != 17)
			std::cerr << "A client has sent invalid request." << std::endl;
		else
		{
			fdget_str(cli, name);
			cr = c == 13;
		}
	}
	return succ;
}
ClientAccepter::ClientAccepter(short port)
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
