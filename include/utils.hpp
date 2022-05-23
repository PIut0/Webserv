#ifndef UTILS_HPP
#define UTILS_HPP
#include <string>
#include <iostream>
#include <vector>

std::string check_arg(int argc, char **argv);
std::vector<std::string> string_split(const std::string &data, const std::string &delim, const int &start_pos);
void exit_with_perror(const std::string &msg);

#endif
