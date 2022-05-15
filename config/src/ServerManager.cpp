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
    std::string     token;
    std::string     line;
    u_short         state = S_DEFAULT;

    ifs.open(path);
    if (!ifs.is_open())
        exit(1);
    while (std::getline(ifs, line)) {
        switch (state) {
            case S_DEFAULT:
                if (line == "server {") {
                    ServerBlock n;
                    this->server.push_back(n);
                    state <<= 1;
                }
                break;
            case S_SERVER:
                if (line == "}")
                    state >>= 1;
                std::istringstream ss(line);
                while (std::getline(ss, token, ' ')) {
                    if (token == "\tlocation") {
                        state <<= 1;
                    }
                }

                break;
            case S_LOCATION:

                break;
            default:
                break;
        }
    }
}