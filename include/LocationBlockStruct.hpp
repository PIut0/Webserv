#ifndef LOCATIONBLOCKSTRUCT_HPP
#define LOCATIONBLOCKSTRUCT_HPP

#include <string>
#include <vector>

struct CgiInfo {
  std::string anjwl;
  std::string url;
};

struct ErrorPage {
  int         code;
  std::string url;
};

#endif
