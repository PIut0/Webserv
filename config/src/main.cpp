#include <vector>
#include <string>

#include "ServerManager.hpp"
#include "utils.hpp"


int main(int argc, char** argv) {
	// if (!check_arg(argc, argv)) {
	// 	std::cerr << "error" << std::endl;
	// 	exit(1);
	// }
	ServerManager serverManager(check_arg(argc, argv));
	return (0);
}