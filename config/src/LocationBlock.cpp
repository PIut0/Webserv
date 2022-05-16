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

void LocationBlock::init_location_block(std::string &location_path, std::vector<std::string> &data)
{
	(void)location_path;

	for (size_t i = 0 ; i < data.size() ; i++) {
		// TODO location block Contents 처리하기
	}
}
