#include<unistd.h>
#include<algorithm>
#include<random>
#include"GameRoom.hpp"
#include"utils.hpp"
GameRoom::GameRoom(std::size_t pcnt, std::uint32_t maxi, std::uint32_t num)
	:num(num), players(), begun(), problems(pcnt), answers(pcnt)
{
	std::array<std::uint32_t, 28>tricks{10, 20, 90, 100, 110, 120, 190, 200, 210, 220, 900, 990, 1000, 1010, 1020, 1090, 1100, 1110, 1120, 1190, 1200, 1210, 2000, 2010, 2090, 2100, 2110, 2190};
	std::array<std::uint32_t, 3>fakes;
	std::mt19937_64 dice(num);
	std::uint32_t lop, rop, ans;
	char op;
	for(std::size_t i=0;i<pcnt;i++)
	{
		lop = dice() % maxi + 1;
		rop = dice() % maxi + 1;
		op = dice() % 4;
		switch(op)
		{
			case 0:
				op = '+';
				ans = lop + rop;
				break;
			case 1:
				op = '-';
				ans = lop;
				lop += rop;
				break;
			case 2:
				op = '*';
				ans = lop * rop;
				break;
			case 3:
				op = '/';
				ans = lop;
				lop *= rop;
				break;
		}
		std::uint32_t fake, trick;
		std::size_t end;
		for(std::size_t j=0;j<fakes.size();j++)
		{
			if(ans < 101)
			{
				fake = dice() % 100 + 1;
				if(j > 0)
				{
					while(fake == ans || std::find(fakes.begin(), fakes.begin() + j, fake) != fakes.begin() + j)
						fake = dice() % 100 + 1;
				}
			}
			else
			{
				end = std::upper_bound(tricks.begin(), tricks.end(), ans) - tricks.begin();
				fake = ans;
				if(j > 0)
				{
					while(fake == ans || std::find(fakes.begin(), fakes.begin() + j, fake) != fakes.begin() + j)
					{
						trick = tricks[dice() % end];
						if(dice() % 2)
							fake = ans + trick;
						else
							fake = ans - trick;
					}
				}
			}
			fakes[j] = fake;
		}
		this->answers[i].choices[0] = fakes[0];
		this->answers[i].choices[1] = fakes[1];
		this->answers[i].choices[2] = fakes[2];
		this->answers[i].choices[3] = ans;
		std::sort(this->answers[i].choices.begin(), this->answers[i].choices.end());
		this->answers[i].actual = std::find(this->answers[i].choices.begin(), this->answers[i].choices.end(), ans) - this->answers[i].choices.begin();
		this->problems[i] = std::to_string(lop) + op + std::to_string(rop) + "=?";
		for(const auto &choice : this->answers[i].choices)
			this->problems[i] += ' ' + std::to_string(choice);
	}
}

void GameRoom::begin()
{
	char msgt = 89;
	for(const auto &player : this->players)
		fdput_obj(player.getFileDes(), msgt);
	usleep(3000000);
	this->begun = true;
	msgt = 97;
	for(const auto &player : this->players)
		fdput_obj(player.getFileDes(), msgt);
}

void GameRoom::addPlayer(int fd, const std::string &name)
{
	char msgt = 83;
	for(const auto &player : this->players)
	{
		fdput_obj(player.getFileDes(), msgt);
		fdput_str(player.getFileDes(), name);
	}
	this->players.emplace_back(fd, name);
}

bool GameRoom::ans(std::size_t player, std::uint32_t ans)
{
	Player &p = this->players.at(player);
	std::size_t pos = p.getPos();
	bool correct = ans == this->answers[pos].actual;
	p.ans(correct);
	return correct;
}
