#ifndef CLIENTHANDLER_HPP_
#define CLIENTHANDLER_HPP_
class ClientHandler
{
private:
	int serv;
	sockaddr_in saddr;
	socklen_t socklen, *slenp;
public:
	ClientHandler(short port);
};
#endif
