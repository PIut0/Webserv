#include "ServerManager.hpp"

ServerManager::ServerManager()
{
    init_server(DEFAULT_CONFIG_PATH);
}

ServerManager::ServerManager(std::string path)
{
    init_server(path);
}

ServerManager::~ServerManager()
{

}

void ServerManager::init_server(std::string path)
{
    std::ifstream   ifs;
    std::string     line;
    std::vector<std::string> temp;
    u_short         state = S_DEFAULT;

    ifs.open(path);
    if (!ifs.is_open())
        exit(1);
    while (std::getline(ifs, line)) {
        if (state & S_DEFAULT && line == "server {") {
            state <<= 1;
        } else if (state & S_SERVER && line == "}") {
            this->server.push_back(ServerBlock(temp));
            temp.clear();
            std::vector<std::string>().swap(temp);
            state >>= 1;
        } else if (state & S_SERVER && line.length() > 0) {
            temp.push_back(line);
        }
    }
}


