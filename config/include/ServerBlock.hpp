#ifndef SERVERBLOCK_HPP
#define SERVERBLOCK_HPP

#include <vector>
#include <string>
#include <iostream>
#include "LocationBlock.hpp"
#include "Dummy.hpp"
#include "System.hpp"

class LocationBlock;

class ServerBlock : public System
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
	void init_server_block(const std::vector<std::string>&);

public:
	ServerBlock();
	ServerBlock(const std::vector<std::string>&);
	~ServerBlock();
};

#endif
