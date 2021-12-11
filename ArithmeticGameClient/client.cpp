#ifdef _WIN32
#include<conio.h>
#include<windows.h>
#define gch getch()
#else
#include<arpa/inet.h>
#include<termios.h>
#include<unistd.h>
#define gch std::cin.get()
#endif
#include<cctype>
#include<future>
#include<iostream>
#include<memory>
#include<thread>
#include"utils/formatting.hpp"
#include"utils/Socket.hpp"
#ifdef _MSC_VER
#pragma comment(lib, "ws2_32.lib")
#endif
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
void display_names(std::istream *issp)
{
	auto &iss = *issp;
	char msgt;
	iss.GETOBJ(msgt);
	std::uint32_t cc;
	std::string name;
	while(msgt != 89)
	{
		if (msgt == 83)
		{
			iss.GETOBJ(cc);
			cc = ntohl(cc);
			name = std::string(cc, msgt);
			iss.read(name.data(), cc);
			std::cout << fmt_bold << name << fmt_reset << " has joined the game." << std::endl;
		}
		iss.GETOBJ(msgt);
	}
}
void play(bool signal, std::istream &iss, std::ostream&oss)
{
	using namespace std;
	using namespace chrono;
	using this_thread::sleep_for;
	cout << "Press buttons 1 through 4 to enter your answer." << endl;
	cout << "Starting in 3";
	cout.flush();
	sleep_for(997ms);
	cout << "\b2";
	cout.flush();
	sleep_for(997ms);
	cout << "\b1";
	cout.flush();
	char msgt;
	iss.read(&msgt, 1);
	if (msgt == 97)
	{
		string ps, anshis;
		char cho;
		cout << "\b0" << endl;
		if (signal)
			oss.put(29);
		uint32_t corrects, tot;
		uint32_t cc;
		cout << "\n\n";
		iss.GETOBJ(msgt);
		while (msgt == 47)
		{
			iss.GETOBJ(cc);
			cc = ntohl(cc);
			ps = std::string(cc, msgt);
			iss.read(ps.data(), cc);
			iss.GETOBJ(corrects).GETOBJ(tot);
			corrects = ntohl(corrects);
			tot = ntohl(tot);
			cout << "\033\1332F";
			cout << anshis << fmt_reset << corrects << '/' << tot << endl;
			cout << ps << "             \b\b\b\b\b\b\b\b\b\b\b\b\b" << endl;
			cho = gch - '0' + 31;
			while(cho > 35 || cho < 32)
			{
				cout.put('\a').flush();
				cho = gch - '0' + 31;
			}
			oss.put(cho);
			iss.GETOBJ(msgt);
			if (msgt == 37)
			{
				cout << fmt_green_foreground << fmt_bold << "CORRECT" << fmt_reset << " +1 for you.";
				anshis += fmt_green_background;
				++corrects;
			}
			else if (msgt == 41)
			{
				iss.GETOBJ(cc);
				cc = ntohl(cc);
				cout << fmt_red_foreground << fmt_bold << "WRONG" << fmt_reset << " No point for you, " << cc << " was the answer.";
				anshis += fmt_red_background;
			}
			cout.flush();
			anshis += ' ';
			iss.read(&msgt, 1);
		}
		endl(cout);
		cout << anshis << fmt_reset << corrects << '/' << tot + 1 << " is your final score." << endl;
		msgt = 43;
		oss.put(msgt);
		iss.GETOBJ(cc);
		uint32_t pcnt = ntohl(cc), finish = tot + 1;
		iss.GETOBJ(msgt);
		while(msgt == 43)
		{
			for(uint32_t i=0;i<pcnt;i++)
			{
				iss.GETOBJ(cc);
				cc = ntohl(cc);
				ps = string(cc, msgt);
				iss.read(ps.data(), cc);
				iss.GETOBJ(corrects).GETOBJ(tot);
				corrects = ntohl(corrects);
				tot = ntohl(tot);
				cout << fmt_bold << ps << fmt_reset << ' ' << corrects << '/' << tot;
				for(uint32_t i=0;i<=tot;i++)
					cout << '-';
				if(tot < finish)
					cout << '>';
				for(uint32_t i=tot+1;i<finish;i++)
					cout << ' ';
				cout << '|';
				if(i + 1 < pcnt)
					endl(cout);
			}
			cout.flush();
			iss.GETOBJ(msgt);
			if(msgt == 43)
			{
				if(pcnt > 0)
					cout << "\033\133" << pcnt - 1 << 'F';
				else
					cout << '\r';
			}
		}
	}
	endl(cout);
}
int main(int argl,char**argv)
{
	using namespace std;
	using namespace string_literals;
#ifdef _WIN32
	WSADATA mssdat;
	WSAStartup(MAKEWORD(2, 2), &mssdat);
	HANDLE ho = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD cm;
	GetConsoleMode(ho, &cm);
	cm |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(ho, cm);
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
	future<void>async_names;
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
			cin.get();
#ifndef _WIN32
			term.c_lflag &= ~ICANON;
			term.c_lflag &= ~ECHO;
			tcsetattr(STDIN_FILENO, TCSANOW, &term);
#endif
			async_names = async(display_names, &sockp->getIstream());
			cout << "Press any key to start." << endl;
			gch;
			sockp->getOstream().put(29);
			async_names.wait();
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
			cin.get();
			namlen = htonl(name.size());
			rnum = htonl(stoul(rnstr, nullptr, 16));
			sockp = make_unique<Socket>(serv_addr, PORT);
			sockp->getOstream().put(msgt);
			sockp->getOstream().PUTOBJ(namlen) << name;
			sockp->getOstream().PUTOBJ(rnum);
			sockp->getIstream().GETOBJ(msgt);
#ifndef _WIN32
			term.c_lflag &= ~ICANON;
			term.c_lflag &= ~ECHO;
			tcsetattr(STDIN_FILENO, TCSANOW, &term);
#endif
			if(msgt == 17)
			{
				cout << fmt_green_foreground << "Successfully joined game." << fmt_reset << endl;
				display_names(&sockp->getIstream());
				play(true, sockp->getIstream(), sockp->getOstream());
			}
			else if(msgt == 19)
				cout << fmt_red_foreground << "Sorry, you're too late, the game has already begun." << fmt_reset << endl;
			else
				cout << fmt_red_foreground << fmt_bold << "No such room exists." << fmt_reset << endl;
		}
		else
			ring;
		cout << "Press q to quit, c to create a room, j to join an existing one." << endl;
		cmd = gchlo;
	}
#ifdef _WIN32
	WSACleanup();
#else
	term.c_lflag |= ICANON;
	term.c_lflag |= ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
#endif
	return 0;
}
