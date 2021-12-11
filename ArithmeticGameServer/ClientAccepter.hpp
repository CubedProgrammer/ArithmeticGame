#ifndef CLIENTACCEPTER_HPP_
#define CLIENTACCEPTER_HPP_
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string>
class ClientAccepter
{
private:
	int serv;
	sockaddr_in saddr;
	socklen_t socklen, *slenp;
public:
	ClientAccepter(short port);
	int getServer()const
	{
		return this->serv;
	}
	int accept_cli(bool &cr, int &fd, std::string &name);
};
#endif
