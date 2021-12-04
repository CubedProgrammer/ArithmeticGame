#include<iostream>
#include<thread>
#include<unordered_map>
#include"ClientHandler.hpp"
#include"GameRoom.hpp"
#define PORT 6969
void accept_clients(std::vector<std::thread>&ths, std::unordered_map<uint32_t, GameRoom>&rooms)
{
	ClientHandler handler(PORT);
}
int main(int argl,char**argv)
{
	using namespace std;
	cout << "ArithmeticGameServer has begun." << endl;
	unordered_map<uint32_t, GameRoom>rooms;
	vector<thread>ths;
	thread th(accept_clients, ths, rooms);
	cin.get();
	return 0;
}
