#include "Defines.hpp"
#include "utils.hpp"

// 인자가 없거나 많거나 체크
std::string check_arg(int argc, char **argv) {
    if (argc == 1) {
        return DEFAULT_CONFIG_PATH;
    }
    if (argc != 2)
        exit_with_perror("Usage: ./server [config_file]");
    return argv[1];
}

std::vector<std::string> string_split(const std::string &data, const std::string &delim, const int &start_pos)
{
    std::vector<std::string> res;
    std::string token;
    u_long pos = start_pos;
    u_long bpos = start_pos; // block scope 만큼 탭 제외

    while ((pos = data.find(delim, pos + 1)) != std::string::npos)
    {
        token = data.substr(bpos, pos - bpos);
        res.push_back(token);
        bpos = pos + delim.length();
    }
    res.push_back(data.substr(bpos, data.length() - bpos));
    return res;
}

void exit_with_perror(const std::string &msg)
{
	std::cerr << msg << ": " << strerror(errno) << std::endl;
	exit(1);
}


