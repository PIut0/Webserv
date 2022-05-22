#include <vector>
#include <string>

#include "ServerManager.hpp"
#include "utils.hpp"


int main(int argc, char** argv) {
	// if (check_arg(argc, argv)) {
	// 	std::cerr << "error" << std::endl;
	// 	exit(1);
	// }
	ServerManager serverManager(check_arg(argc, argv));
	// std::vector<std::string> str = string_split("hi hello world", " ", 0);
	// for (size_t i = 0 ; i < str.size() ; i++)
	// 	std::cout << str[i] << std::endl;
	return (0);
}
