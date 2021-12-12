#ifndef UTILS_PROCESS_HPP_
#define UTILS_PROCESS_HPP_
#include<string>
#include"ipipestream.hpp"
#include"opipestream.hpp"
class Process
{
	ipipestream *ips;
	opipestream *ops;
public:
	Process(const std::string &cmd);
	Process()
		:Process("") {}
	~Process();
	std::ostream &getStdin()
	{
		return *this->ops;
	}
	std::istream &getStdout()
	{
		return *this->ips;
	}
};
#endif
