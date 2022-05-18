#include "LocationBlock.hpp"

LocationBlock::LocationBlock()
{
}

LocationBlock::LocationBlock(std::string &location_path, std::vector<std::string> &data)
{
	init_location_block(location_path, data);
}

LocationBlock::~LocationBlock()
{
}

LocationBlock::LocationAttribute LocationBlock::check_validate(std::string &data)
{
	if (data[0] != '\t' && data[1] != '\t')
		return ERROR;
	std::string tmp = data.substr(2, data.length() - 2);
	while()
}


void LocationBlock::init_location_block(std::string &location_path, std::vector<std::string> &data)
{
	std::cout << "lo_path : " << location_path <<  std::endl;
	for (size_t i = 0 ; i < data.size() ; i++) {
		// TODO location block Contents 처리하기
		if (check_validate(data[i]) == ERROR)
			error_with_exit();
		std::cout << "[ Location ]" << data[i] << std::endl;
	}
	this->getMonitor().print(COLOR_RED, "location block close");
}
