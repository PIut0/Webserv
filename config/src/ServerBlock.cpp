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

ServerBlock::ServserAttribute ServerBlock::check_validate(const std::string &data)
{
	if (data[0] != '\t') {
		getMonitor().log("server block config error");
		return ERROR;
	}
	return TRUE;
}

void ServerBlock::parse_server(const std::string &data)
{
	std::vector<std::string> tocken;
	if (check_validate(data) == ERROR)
		error_with_exit();
	if (data.substr(1,11) == "server_name")
		this->server_name = data.substr(13, data.length());
	if (data.substr(1, 6) == "listen") {
		this->port = atoi(data.substr(8, data.substr(8, data.length()).find(' ')).c_str());
		this->host = data.substr(data.substr(8, data.length()).find(' ') + 9, data.find(';')-1);
		std::cout << "port =" << this->port << ";\nhost="<<data.substr(data.substr(8, data.length()).find(' ') + 9, data.find(';')-1) << this->host << "==" << std::endl;
	}
}

void ServerBlock::init_server_block(const std::vector<std::string> &data)
{
	std::string token;
	std::vector<std::string> temp;

	u_short state = S_SERVER;
	std::string location_path = "";

	for (size_t i = 0 ; i < data.size() ; i++) {
		if (check_validate(data[i]) == ERROR)
			error_with_exit();
		if (state & S_SERVER) {
			if (data[i].substr(0, 9) == LOCATION_BLOCK_OPEN) {
				location_path = data[i].substr(10, data[i].find('{', 0) - 10);
                this->getMonitor().print(COLOR_GREEN, "new location block open");
				state <<= 1;
			} else {
			// TODO 여기는 Server Block Contents 처리하면됨 함수 만들면 될듯
                std::cout << "[ Server ]" << data[i] << std::endl;
				parse_server(data[i]);
			}
		} else if (state & S_LOCATION) {
			if (data[i].substr(0, 2) == LOCATION_BLOCK_CLOSE) {
				this->location.push_back(LocationBlock(location_path, temp));
                CLEAR_VECTOR_COMPLETLY(temp)
                // TO_SERVER(state)
				state >>= 1;
			} else {
				temp.push_back(data[i]);
			}
		}
	}
}
