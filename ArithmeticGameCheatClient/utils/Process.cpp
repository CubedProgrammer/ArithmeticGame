#ifdef _WIN32
#include<windows.h>
#else
#include<unistd.h>
#include<cstdlib>
#endif
#include<stdexcept>
#include<iostream>
#include"Process.hpp"
Process::Process(const std::string &cmd)
{
#ifdef _WIN32
#else
	int p_ins[2], p_outs[2];
	int succ = pipe(p_ins);
	succ = succ | pipe(p_outs);
	if(succ != 0)
		throw std::runtime_error("Pipes could not be created.");
	int pid = fork();
	if(pid < 0)
		throw std::runtime_error("Process could not be forked.");
	else if(pid == 0)
	{
		succ = dup2(p_ins[0], STDIN_FILENO);
		if(succ < 0)
			throw std::runtime_error("Could not redirect stdin");
		succ = dup2(p_outs[1], STDOUT_FILENO);
		if(succ < 0)
			throw std::runtime_error("Could not redirect stdout.");
		int ans = std::system(cmd.c_str());
		std::exit(WEXITSTATUS(ans));
	}
	else
	{
		close(p_ins[0]);
		close(p_outs[1]);
	}
	pipebuf *pbuf = new pipebuf(p_ins[1], p_outs[0]);
#endif
	this->ips = new ipipestream(pbuf);
	this->ops = new opipestream(pbuf);
}

Process::~Process()
{
	delete this->ips;
	delete this->ops;
}
