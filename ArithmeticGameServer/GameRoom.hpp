#ifndef GAMEROOM_HPP_
#define GAMEROOM_HPP_
#include<cstdint>
#include<vector>
class GameRoom
{
private:
	std::uint32_t num;
	std::vector<int>fds;
	std::vector<std::size_t>poses;
public:
	GameRoom(std::uint32_t num)
		:num(num), fds(), poses() {}
	GameRoom()
		:num(), fds(), poses() {}
	bool ans(std::size_t player, std::uint32_t ans);
};
#endif
