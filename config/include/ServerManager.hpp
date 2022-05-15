#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "ServerBlock.hpp"

class ServerManager
{
private:
	ServerBlock server[];

public:
	ServerManager();
	~ServerManager();
};

#endif