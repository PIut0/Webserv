#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "ServerBlock.hpp"
#include <iostream>
#include <vector>
#include <fstream>

class ServerBlock;

class ServerManager
{
private:
	std::vector<ServerBlock> server;

public:
	ServerManager();
	ServerManager(std::string);
	~ServerManager();
};

#endif