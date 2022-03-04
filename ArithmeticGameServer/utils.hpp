#ifndef UTILS_HPP_
#define UTILS_HPP_
#include<arpa/inet.h>
#include<unistd.h>
#include<chrono>
#include<string>
inline std::string time_str()
{
	using namespace std::chrono;
#if __cplusplus >= 202002L
#else
	auto dur = duration_cast<seconds>(system_clock::now().time_since_epoch());
	return std::to_string(dur.count());
#endif
}
template<typename T>
inline std::size_t fdget_obj(int fd, T&obj)
{
	return read(fd, &obj, sizeof(T));
}
template<typename T>
inline std::size_t fdput_obj(int fd, const T&obj)
{
	return write(fd, &obj, sizeof(T));
}
inline std::size_t fdget_str(int fd, std::string&obj)
{
	unsigned len;
	std::size_t bc = fdget_obj(fd, len);
	len = ntohl(len);
	char *buf = new char[len];
	std::size_t cc = read(fd, buf, len);
	obj = std::string(buf, len);
	delete[]buf;
	return bc + cc;
}
inline std::size_t fdput_str(int fd, const std::string&obj)
{
	unsigned len = obj.size();
	len = htonl(len);
	std::size_t bc = fdput_obj(fd, len);
	bc += write(fd, obj.c_str(), obj.size());
	return bc;
}
#endif
