#ifndef LOCATIONBLOCKSTRUCT_HPP
#define LOCATIONBLOCKSTRUCT_HPP

#include <string>
#include <vector>

typedef struct {
  std::string anjwl;
  std::string url;
} CgiInfo;

typedef struct {
  int         code;
  std::string url;
} ErrorPage;

#endif
