#include "util.hpp"

void exit_with_perror(const std::string &msg)
{
	std::cerr << msg << ": " << strerror(errno) << std::endl;
	exit(1);
}
