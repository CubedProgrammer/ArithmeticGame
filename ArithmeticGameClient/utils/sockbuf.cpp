#include<unistd.h>
#include<algorithm>
#include"sockbuf.hpp"
#define BUFSZ 8192
sockbuf::sockbuf(int sockfd)
	:sockfd(sockfd), bc()
{
	this->rbuf = new char_type[BUFSZ];
}

sockbuf::~sockbuf()
{
	delete[]this->rbuf;
#ifdef _WIN32
	CloseHandle(sock);
#else
	close(this->sockfd);
#endif
}

int sockbuf::sync()
{
	this->bc = BUFSZ;
	return 0;
}

std::streamsize sockbuf::showmanyc()
{
	return BUFSZ - this->bc;
}

std::streamsize sockbuf::xsgetn(char_type *buf, std::streamsize sz)
{
	std::streamsize br = sz;
	if(BUFSZ-this->bc>=sz)
	{
		std::copy(this->rbuf + this->bc, this->rbuf + this->bc + sz, buf);
		this->bc += sz;
	}
	else
	{
		if(this->bc<BUFSZ)
		{
			std::streamsize remain = BUFSZ - this->bc;
			std::copy(this->rbuf + this->bc, this->rbuf + BUFSZ, buf);
			this->bc += remain;
			buf += remain;
			sz -= remain;
			br = remain;
		}
		else
			br = 0;
		if(BUFSZ <= sz)
			 br += read(this->sockfd, buf, sz);
		else
		{
			std::streamsize tot = read(this->sockfd, this->rbuf, BUFSZ);
			if(tot <= sz)
			{
				br += tot;
				std::copy(this->rbuf, this->rbuf + tot, buf);
			}
			else
			{
				this->bc = 0;
				if(BUFSZ > tot)
				{
					this->bc = BUFSZ - tot;
					std::move_backward(this->rbuf, this->rbuf + tot, this->rbuf + BUFSZ);
				}
				std::copy(this->rbuf + this->bc, this->rbuf + this->bc + sz, buf);
				this->bc += sz;
				br += sz;
			}
		}
	}
	return br;
}

std::streamsize sockbuf::xsputn(const char_type *buf, std::streamsize sz)
{
	return write(this->sockfd, buf, sz);
}

sockbuf::int_type sockbuf::overflow(int_type ch)
{
	char_type acc = ch;
	int succ = write(this->sockfd, &acc, 1);
	return succ == 0 ? traits_type::eof() : 0;
}
