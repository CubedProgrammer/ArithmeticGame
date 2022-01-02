#include<signal.h>
#include<ctime>
#include<iostream>
#include<random>
#include<thread>
#include<unordered_map>
#include"ClientAccepter.hpp"
#include"GameRoom.hpp"
#include"utils.hpp"
#define PORT 6969
void handle_client(std::size_t indpl, GameRoom *roomp)
{
	GameRoom &room = *roomp;
	char msgt = 29;
	auto &player = room.getPlayer(indpl);
	int cli = player.getFileDes();
	if(indpl == 0)
	{
		fdget_obj(cli, msgt);
		while(msgt != 29)
			fdget_obj(cli, msgt);
		room.begin();
	}
	else
		fdget_obj(cli, msgt);
	bool quit = msgt != 29;
	int corr = 0;
	for(std::size_t i=0;i<room.getProblemCount();i++)
	{
		msgt = 47;
		fdput_obj(cli, msgt);
		fdput_str(cli, room.getProblem(i));
		fdput_obj(cli, htonl(corr));
		fdput_obj(cli, htonl(i));
		fdget_obj(cli, msgt);
		if(msgt >= 32 && msgt <= 35)
		{
			msgt = room.ans(indpl, msgt - 32) ? 37 : 41;
			fdput_obj(cli, msgt);
			if(msgt == 41)
				fdput_obj(cli, htonl(room.getAnsVal(i)));
			else
				++corr;
			usleep(1000000);
		}
		else
		{
			quit = true;
			i = -2;
		}
	}
	if(!quit)
	{
		msgt = 43;
		fdput_obj(cli, msgt);
		fdget_obj(cli, msgt);
		quit = msgt == 31;
		fdput_obj(cli, htonl(room.getPlayerCount()));
		while(!quit)
		{
			quit = true;
			msgt = 43;
			fdput_obj(cli, msgt);
			for(std::size_t i=0;i<room.getPlayerCount();i++)
			{
				const auto &player = room.getPlayer(i);
				quit = quit && player.getPos() == room.getProblemCount();
				fdput_str(cli, player.getName());
				fdput_obj(cli, htonl(player.getCorrect()));
				fdput_obj(cli, htonl(player.getPos()));
			}
			usleep(500000);
		}
	}
	msgt = 31;
	fdput_obj(cli, msgt);
	usleep(1000000);
	close(cli);
	std::cout << "Disconnected " << player.getName() << " of room " << room.getRoomNumber() << std::endl;
}
void accept_clients(int&server, std::vector<std::thread>&ths, std::unordered_map<uint32_t, GameRoom>&rooms)
{
	using namespace std;
	linear_congruential_engine<uint64_t, 25214903917, 11, 1l << 48>dice(time(nullptr) ^ 25214903917);
	ClientAccepter handler(PORT);
	server = handler.getServer();
	bool cr;
	int cli, succ;
	uint32_t rnum, maxi;
	uint32_t pcnt;
	string name;
	char status;
	GameRoom *roomp = nullptr;
	vector<uint32_t>to_remove;
	for(;;)
	{
		succ = handler.accept_cli(cr, cli, name);
		to_remove.clear();
		for(auto& [num, room] : rooms)
		{
			if(room.isFinished())
				to_remove.push_back(num);
		}
		for(auto &num : to_remove)
			rooms.erase(num);
		if(rooms.empty())
		{
			for(auto &th : ths)
				th.join();
			ths.clear();
			cout << "Rooms are empty, cleared thread array." << endl;
		}
		if(succ == 0)
		{
			if(cr)
			{
				rnum = dice();
				fdget_obj(cli, maxi);
				fdget_obj(cli, pcnt);
				maxi = ntohl(maxi);
				pcnt = ntohl(pcnt);
				rooms[rnum] = GameRoom(pcnt, maxi, rnum);
				cout << name << " has created room " << hex << rnum << dec << " with " << maxi << " as maximum operand and " << pcnt << " problems." << endl;
				fdput_obj(cli, htonl(rnum));
				rooms[rnum].addPlayer(cli, name);
				roomp = &rooms[rnum];
			}
			else
			{
				fdget_obj(cli, rnum);
				rnum = ntohl(rnum);
				if(rooms.find(rnum) == rooms.end())
					status = 23;
				else
				{
					roomp = &rooms[rnum];
					if(roomp->hasBegun())
						status = 19;
					else
						status = 17;
				}
				fdput_obj(cli, status);
				if(status == 17)
				{
					auto &rm = *roomp;
					rm.addPlayer(cli, name);
					cout << name << " has joined room " << hex << rnum << dec << endl;
				}
				else
					succ = -1;
			}
			if(succ == 0)
				ths.emplace_back(handle_client, roomp->getPlayerCount() - 1, roomp);
			cout << ths.size() << ' ' << rooms.size() << endl;
		}
		else
			cerr << "Accepting a client has failed." << endl;
	}
}
struct AcceptFunction
{
	void operator()(int*server, std::vector<std::thread>*ths, std::unordered_map<uint32_t, GameRoom>*rooms)const
	{
		accept_clients(*server, *ths, *rooms);
	}
};
int main(int argl,char**argv)
{
	signal(SIGPIPE, SIG_IGN);
	using namespace std;
	cout << "ArithmeticGameServer has begun." << endl;
	int server;
	unordered_map<uint32_t, GameRoom>rooms;
	vector<thread>ths;
	thread th(AcceptFunction(), &server, &ths, &rooms);
	cin.get();
	th.detach();
	close(server);
	return 0;
}
