#include "utils.hpp"

// 인자가 없거나 많거나 체크
std::string check_arg(int argc, char **argv) {
    if (argc == 1) {
        return "./config_example.config";
    }
    if (argc != 2)
        return "";
    return argv[1];
}

