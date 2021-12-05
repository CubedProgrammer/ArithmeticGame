#include"Player.hpp"
std::size_t Player::getCorrect()const
{
	return this->correct;
}

std::size_t Player::getPos()const
{
	return this->pos;
}

const std::string &Player::getName()const
{
	return this->name;
}

int Player::getFileDes()const
{
	return this->fd;
}

void Player::ans(bool correct)
{
	++this->pos;
	if(correct)
		++this->correct;
}
