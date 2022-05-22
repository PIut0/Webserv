#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "ServerBlock.hpp"
#include "Defines.hpp"
#include "System.hpp"

class ServerBlock;

class ServerManager : public System
{
private:
	std::vector<ServerBlock> serverBlock;
	void init_server(std::string path);

public:
	ServerManager();
	ServerManager(std::string);
	~ServerManager();
};

#endif
