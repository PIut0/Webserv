#ifndef UTILS_HPP
#define UTILS_HPP
#include <string>
#include <iostream>
#include <vector>

std::string CheckArg(int argc, char **argv);
std::vector<std::string> StringSplit(const std::string &data, const std::string &delim, const int &start_pos);
void ExitWithPerror(const std::string &msg);

#endif

