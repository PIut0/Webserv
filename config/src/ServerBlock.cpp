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
	std::string token;
	std::vector<std::string> temp;
	u_short state = S_SERVER;

	this->getMonitor().print("new server block");
	for(size_t i = 0 ; i < data.size() ; i++) {
		std::stringstream ss(data[i]);
		if (data[i][0] != '\t')
			getMonitor().log("config error");
		if (state & S_SERVER) {
			if (data[i].substr(0, 9) == LOCATION_BLOCK_OPEN) {
				this->getMonitor().print("new location block");
				state <<= 1;
			}
		} else if (state & S_LOCATION) {
			if (data[i].substr(0, 2) == LOCATION_BLOCK_CLOSE) {

			}
		}
	}
}
