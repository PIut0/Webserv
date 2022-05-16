#include "ServerManager.hpp"

ServerManager::ServerManager()
{
    init_server(DEFAULT_CONFIG_PATH);
}

ServerManager::ServerManager(std::string path)
{
    getMonitor().log("hi");
    init_server(path);
}

ServerManager::~ServerManager()
{

}

void ServerManager::init_server(std::string path)
{
    std::string     line;
    std::ifstream   ifs;
    std::vector<std::string> temp;
    u_short         state = S_DEFAULT;
    ifs.open(path);
    if (!ifs.is_open())
        exit(1);
    while (std::getline(ifs, line)) {
        if (state & S_DEFAULT && line == SERVER_BLOCK_OPEN) {
            this->getMonitor().print("new server block open");
            state <<= 1;
        } else if (state & S_SERVER && line == SERVER_BLOCK_CLOSE) {
            this->getMonitor().print("server block close");
            this->server.push_back(ServerBlock(temp));
            temp.clear();
            std::vector<std::string>().swap(temp);
            state >>= 1;
        } else if (state & S_SERVER && line.length() > 0) {
            temp.push_back(line);
        }
    }
}


