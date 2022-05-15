#include <vector>
#include <string>

#include "ServerManager.hpp"
#include "utils.cpp"


int main(int argc, char** argv) {
	if (check_arg(argv)) {
		perror("error!");
		exit(1);
	}
	ServerManager serverManager(argv[1]);

}