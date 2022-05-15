#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "ServerBlock.hpp"
#include "Dummy.hpp"

#define S_DEFAULT	1
#define S_SERVER	2

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