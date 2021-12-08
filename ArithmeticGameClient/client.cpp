#ifdef _WIN32
#include<windows.h>
#define gch getch()
#else
#include<arpa/inet.h>
#include<termios.h>
#include<unistd.h>
#define gch std::cin.get()
#endif
#include<cctype>
#include<iostream>
#include<memory>
#include"utils/formatting.hpp"
#include"utils/Socket.hpp"
#define gchlo std::tolower(gch)
#define ring std::cout.put('\a').flush()
#define GETOBJ(obj)read(reinterpret_cast<char*>(&obj), sizeof(obj))
#define PUTOBJ(obj)write(reinterpret_cast<const char*>(&obj), sizeof(obj))
#define PORT 6969
std::pair<std::uint32_t,std::uint32_t>getSettings()
{
	using namespace std;
	pair<uint32_t, uint32_t> settings;
	cout << "Enter the number of problems and the maximum size of the operand." << endl;
	cin >> settings.second >> settings.first;
	settings.first = htonl(settings.first-1);
	settings.second = htonl(settings.second);
	return settings;
}
void play(bool signal, std::istream &iss, std::ostream&oss)
{
}
int main(int argl,char**argv)
{
	using namespace std;
	using namespace string_literals;
#ifdef _WIN32
#else
	termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ICANON;
	term.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
#endif
	auto serv_addr = "127.0.0.1"s;
	if(argl == 2)
		serv_addr = argv[1];
	cout << "Press q to quit, c to create a room, j to join an existing one." << endl;
	char cmd = gchlo, msgt;
	string name, rnstr;
	unique_ptr<Socket>sockp;
	pair<uint32_t, uint32_t> settings;
	uint32_t rnum, namlen;
	while(cmd != 'q')
	{
		if(cmd == 'c')
		{
			msgt = 13;
#ifndef _WIN32
			term.c_lflag |= ICANON;
			term.c_lflag |= ECHO;
			tcsetattr(STDIN_FILENO, TCSANOW, &term);
#endif
			cout << "Enter your name." << endl;
			getline(cin, name);
			namlen = htonl(name.size());
			sockp = make_unique<Socket>(serv_addr, PORT);
			sockp->getOstream().put(msgt);
			sockp->getOstream().PUTOBJ(namlen) << name;
			settings = getSettings();
			sockp->getOstream().write(reinterpret_cast<char*>(&settings), sizeof(settings)).flush();
			sockp->getIstream().read(reinterpret_cast<char*>(&rnum), sizeof(rnum));
			rnum = ntohl(rnum);
			cout << "Room number is " << hex << rnum << dec << endl;
#ifndef _WIN32
			term.c_lflag &= ~ICANON;
			term.c_lflag &= ~ECHO;
			tcsetattr(STDIN_FILENO, TCSANOW, &term);
#endif
			cout << "Press any key to start." << endl;
			gch;
			sockp->getOstream().put(29);
			play(false, sockp->getIstream(), sockp->getOstream());
		}
		else if(cmd == 'j')
		{
			msgt = 17;
#ifndef _WIN32
			term.c_lflag |= ICANON;
			term.c_lflag |= ECHO;
			tcsetattr(STDIN_FILENO, TCSANOW, &term);
#endif
			cout << "Enter your name and the room you wish to join, on separate lines." << endl;
			getline(cin, name) >> rnstr;
			namlen = htonl(name.size());
			rnum = htonl(stoul(rnstr));
			sockp = make_unique<Socket>(serv_addr, PORT);
			sockp->getOstream().put(msgt);
			sockp->getOstream().PUTOBJ(namlen) << name;
			sockp->getOstream().PUTOBJ(rnum);
			msgt = sockp->getIstream().get();
			if(msgt == 17)
				cout << fmt_green_foreground << "Successfully joined game." << fmt_reset << endl;
			else if(msgt == 19)
				cout << fmt_red_foreground << "Sorry, you're too late, the game has already begun." << fmt_reset << endl;
			else
				cout << fmt_red_foreground << fmt_bold << "No such room exists." << fmt_reset << endl;
#ifndef _WIN32
			term.c_lflag &= ~ICANON;
			term.c_lflag &= ~ECHO;
			tcsetattr(STDIN_FILENO, TCSANOW, &term);
#endif
			play(true, sockp->getIstream(), sockp->getOstream());
		}
		else
			ring;
		cmd = gchlo;
	}
#ifndef _WIN32
	term.c_lflag |= ICANON;
	term.c_lflag |= ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
#endif
	return 0;
}
