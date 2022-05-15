#include "ServerBlock.hpp"

ServerBlock::ServerBlock()
{
	// init_server_block(DEFAULT_SERVER_BLOCK("root /"));
}

ServerBlock::ServerBlock(const std::vector<std::string> &data)
{
	init_server_block(data);
}

ServerBlock::~ServerBlock()
{
}

void ServerBlock::init_server_block(const std::vector<std::string> &data)
{
	std::cout << "[NEW SERVER BLOCK]" << std::endl;
	for(size_t i = 0 ; i < data.size() ; i++) {
		std::cout << data[i] << std::endl;
		if (data[i][0] != '\t')
			getMonitor().log("config error");
	}
}
