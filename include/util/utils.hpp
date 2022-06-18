#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_SOCKET_LIFE_TIME 100

static std::map<std::string, std::string> _mimes;

int CheckSocketAlive(clock_t socketStartTime);
void ThrowException(const std::string &msg);
std::string CheckArg(int argc, char **argv);
std::vector<std::string> StringSplit(const std::string &data, const std::string &delim, const int &start_pos);

std::string& ltrim(std::string& s);
std::string& rtrim(std::string& s);
std::string& trim(std::string& s);

std::string ft_itos(int i);
int ft_stoi(const std::string &s);
int ft_hextoi(const std::string &s);

int IsEOF(int fd);
int IsRegularFile(const std::string &path);

void CloseFd(int fd);
void SetMime();

std::string ToLower(const std::string &origin);
std::string GetDate();
std::string MimeType(const std::string &ext);
std::string StatusCode(const int &code);
std::string DefaultErrorPage(const int &code);
std::string GetAutoindexPage(std::string &target_path, std::vector<std::string> &files);

void break_point();

#endif

