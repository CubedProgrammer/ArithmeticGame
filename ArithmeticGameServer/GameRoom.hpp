#ifndef GAMEROOM_HPP_
#define GAMEROOM_HPP_
#include<array>
#include<cstdint>
#include<vector>
#include"Player.hpp"
struct Ans
{
	std::array<std::uint32_t, 4>choices;
	std::size_t actual;
};

class GameRoom
{
private:
	std::uint32_t num;
	std::vector<Player>players;
	std::vector<std::string>problems;
	std::vector<Ans>answers;
	bool begun;
public:
	GameRoom(std::size_t pcnt, std::uint32_t maxi, std::uint32_t num);
	GameRoom()
		:GameRoom(0,0,0) {}
	void addPlayer(int fd, const std::string &name);
	bool ans(std::size_t player, std::uint32_t ans);
	std::uint32_t getAnsVal(std::size_t indp)const
	{
		return this->answers.at(indp).choices[this->answers.at(indp).actual];
	}
	const std::string &getProblem(std::size_t indp)const
	{
		return this->problems.at(indp);
	}
	const std::size_t getProblemCount()const
	{
		return this->problems.size();
	}
	Player &getPlayer(std::size_t player)
	{
		return this->players.at(player);
	}
	std::size_t getPlayerCount()const
	{
		return this->players.size();
	}
	void begin();
	constexpr bool hasBegun()const
	{
		return this->begun;
	}
	constexpr std::uint32_t getRoomNumber()const
	{
		return this->num;
	}
};
#endif
