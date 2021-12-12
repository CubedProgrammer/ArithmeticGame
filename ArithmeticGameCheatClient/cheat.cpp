#ifdef _WIN32
#include<windows.h>
#define gch getch();
#else
#include<termios.h>
#include<unistd.h>
#define gch std::cin.get()
#endif
#include<iostream>
#include"utils/Process.hpp"
int main(int argl,char**argv)
{
	using namespace std;
	if(argl < 5)
		cout << "Enter name of the default client executable followed by the IP address of the server, then your name and the room number." << endl;
	else
	{
		string cmd = argv[1];
		cmd += ' ';
		cmd += argv[2];
		string name = argv[3], rnstr = argv[4];
		cout << cmd << endl;
		Process proc(cmd);
		istream &proc_out = proc.getStdout();
		ostream &proc_in = proc.getStdin();
		string prompt;
		getline(proc_out, prompt);
		proc_in.put('q');
		cout << prompt << endl;
	}
	return 0;
}
