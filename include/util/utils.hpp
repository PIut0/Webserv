#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <iostream>

void ExitWithPerror(const std::string &msg);
std::string CheckArg(int argc, char **argv);
std::vector<std::string> StringSplit(const std::string &data, const std::string &delim, const int &start_pos);

std::string& ltrim(std::string& s);
std::string& rtrim(std::string& s);
std::string& trim(std::string& s);

#endif

