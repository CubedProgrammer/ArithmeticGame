#ifndef PLAYER_HPP_
#define PLAYER_HPP_
#include<string>
class Player
{
private:
	int fd;
	std::size_t correct, pos;
	std::string name;
public:
	Player()
		:fd(), correct(), pos(), name() {}
	Player(int fd, const std::string &name)
		:fd(fd), name(name), correct(), pos() {}
	std::size_t getCorrect()const;
	std::size_t getPos()const;
	const std::string &getName()const;
	int getFileDes()const;
	void ans(bool correct);
};
#endif
