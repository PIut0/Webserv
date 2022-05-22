#include "ServerBlock.hpp"

ServerBlock::ServerBlock(const std::vector<std::string> &data)
{
	init_server_block(data);
	// printBlock();
}

ServerBlock::~ServerBlock()
{
}

ServerBlock::ServerAttribute ServerBlock::parseListen(const std::string &data)
{
	std::vector<std::string> split_data = string_split(data, " ", 0);
	this->port = atoi(split_data[0].c_str());
	this->host = split_data[1];
	return LISTEN_;
}
ServerBlock::ServerAttribute ServerBlock::parseServerName(const std::string &data)
{
	this->server_name = data;
	return SERVER_NAME_;
}

ServerBlock::ServerAttribute ServerBlock::check_validate(const std::string &data)
{

	int index = data.find(' ');
	std::string command = data.substr(1, index - 1);
	std::string contents = data.substr(index + 1, data.find(';') - index - 1);

	if (data[0] != '\t')
		return ERROR_;
	if (command == "server_name")
		return parseServerName(contents);
	else if (command == "listen")
		return parseListen(contents);
	else if (command == "location")
		return LOCATION_;
	return ERROR_;
}

void ServerBlock::init_server_block(const std::vector<std::string> &data)
{
	std::vector<std::string> element;
	ServerAttribute type;

	std::string location_path = "";
	u_short state = S_SERVER;

	for (size_t i = 0 ; i < data.size() ; i++) {
		switch (state)
		{
		case S_SERVER:
			if ((type = check_validate(data[i])) == ERROR_)
				exit_with_perror("error");
			if (type == LOCATION_) {
				location_path = data[i].substr(10, data[i].find('{', 0) - 10);
				state <<= 1;
			} else {
				if ((type = check_validate(data[i])) == ERROR_)
					exit_with_perror("error");
			}
			break;
		case S_LOCATION:
			if (data[i].substr(0, 2) == LOCATION_BLOCK_CLOSE) {
				this->location.push_back(LocationBlock(location_path, element));
                CLEAR_VECTOR_COMPLETLY(element);
				state >>= 1;
			} else {
				element.push_back(data[i]);
			}
		default:
			break;
		}
	}
}

void ServerBlock::printBlock()
{
	std::cout << "server_name : " << this->server_name << std::endl;
	std::cout << "host : " << this->host << std::endl;
	std::cout << "port : " << this->port << std::endl;
}
