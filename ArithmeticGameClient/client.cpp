#ifdef _WIN32
#include<windows.h>
#define gch getch()
#else
#include<termios.h>
#include<unistd.h>
#define gch std::cin.get()
#endif
#include<cctype>
#include<iostream>
#include"utils/Socket.hpp"
#define gchlo std::tolower(gch)
#define ring std::cout.put('\a').flush()
#define PORT 6969
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
	char cmd = gchlo;
	Socket sock(serv_addr, PORT);
	while(cmd != 'q')
	{
		if(cmd == 'c')
		{
		}
		else if(cmd == 'j')
		{
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
