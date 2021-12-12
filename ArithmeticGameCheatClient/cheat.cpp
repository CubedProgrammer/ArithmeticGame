#ifdef _WIN32
#include<windows.h>
#define gch getch();
#else
#include<termios.h>
#include<unistd.h>
#define gch std::cin.get()
#endif
#include<algorithm>
#include<array>
#include<iostream>
#include"utils/Process.hpp"
std::uint32_t solve(const std::string &prob);
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
		cout << prompt << endl;
		proc_in.put('j');
		getline(proc_out, prompt);
		cout << prompt << endl;
		proc_in << name << '\n' << rnstr << endl;
		getline(proc_out, prompt);
		cout << prompt << endl;
		if(prompt.find("Success") != string::npos)
		{
			while(prompt != "Starting in 3\b2\b1\b0")
			{
				getline(proc_out, prompt);
				cout << prompt << endl;
			}
			char ch = proc_out.get();
			while(ch == '\n')
			{
				cout.put(ch);
				ch = proc_out.get();
			}
			getline(proc_out, prompt);
			cout << ch << prompt << endl;
			string prob;
			uint32_t ans;
			array<uint32_t, 4>choices;
			while(prompt.find('/') != string::npos)
			{
				proc_out >> prob >> choices[0] >> choices[1] >> choices[2] >> choices[3];
				cout << prob << ' ' << choices[0] << ' ' << choices[1] << ' ' << choices[2] << ' ' << choices[3] << endl;
				ans = find(choices.begin(), choices.end(), solve(prob)) - choices.begin();
				proc_in.put(ans + '1');
				getline(proc_out, prompt);
				getline(proc_out, prompt);
				cout << prompt << endl;
			}
		}
		proc_in.put('q');
		while(prompt.size() > 0 && !proc_out.eof())
		{
			getline(proc_out, prompt);
			cout << prompt << endl;
		}
		cout << "Finished" << endl;
	}
	return 0;
}
std::uint32_t solve(const std::string &prob)
{
	using namespace std::string_literals;
	std::uint32_t ans = 0, ind;
	for(ind = 0; "0123456789"s.find(prob[ind]) != std::string::npos; ++ind);
	std::uint32_t l = std::stoul(prob.substr(0, ind));
	std::uint32_t r = std::stoul(prob.substr(ind + 1, prob.size() - ind - 3));
	switch(prob[ind])
	{
		case'+':
			ans = l + r;
			break;
		case'-':
			ans = l - r;
			break;
		case'*':
			ans = l * r;
			break;
		case'/':
			ans = l / r;
			break;
	}
	return ans;
}
