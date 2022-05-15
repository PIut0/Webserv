#ifndef SERVERBLOCK_HPP
#define SERVERBLOCK_HPP

#include "LocationBlock.hpp"
#include <vector>

class LocationBlock;

class ServerBlock
{
private:
	// std::vector<LocationBlock> location;
	// std::string server_name; //
	// unsigned int port;		 // uintshort?
	// std::string host;		 //

	// unsigned int limit_client_body_size; //
	// unsigned int req_uri_limit_size;	 //
	// unsigned int req_header_limit_size;	 //
	// std::string default_err_page;

public:
	ServerBlock(/* args */);
	~ServerBlock();
};

#endif