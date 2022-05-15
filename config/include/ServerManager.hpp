#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "ServerBlock.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#define DEFAULT_CONFIG_PATH "./config_example.config"
#define S_DEFAULT	1
#define S_SERVER	2
#define S_LOCATION	4

class ServerBlock;

class ServerManager
{
private:
	std::vector<ServerBlock> server;
	void init_server(std::string path);

public:
	ServerManager();
	ServerManager(std::string);
	~ServerManager();
};

#endif