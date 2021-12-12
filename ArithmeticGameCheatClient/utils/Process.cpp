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
	HANDLE p_ins[2], p_outs[2];
	SECURITY_ATTRIBUTES attr;
	attr.nLength = sizeof(attr);
	attr.lpSecurityDescriptor = NULL;
	attr.bInheritHandle = TRUE;
	CreatePipe(p_ins, p_ins + 1, &attr, 0);
	CreatePipe(p_outs, p_outs + 1, &attr, 0);
	SetHandleInformation(p_ins[1], HANDLE_FLAG_INHERIT, 0);
	SetHandleInformation(p_outs[0], HANDLE_FLAG_INHERIT, 0);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	si.hStdInput = p_ins[0];
	si.hStdOutput = p_outs[1];
	si.dwFlags |= STARTF_USESTDHANDLES;
	std::string cmd_cp = cmd;
	cmd_cp.reserve(cmd.size() + 4);
	CreateProcessA(NULL, cmd_cp.data(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	CloseHandle(p_ins[0]);
	CloseHandle(p_outs[1]);
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
#endif
	pipebuf *pbuf = new pipebuf(p_ins[1], p_outs[0]);
	this->ips = new ipipestream(pbuf);
	this->ops = new opipestream(pbuf);
}

Process::~Process()
{
	delete this->ips;
	delete this->ops;
}
