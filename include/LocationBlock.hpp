#ifndef LOCATIONBLOCK_HPP
#define LOCATIONBLOCK_HPP

#include <string>
#include <vector>
#include <map>

#include "System.hpp"
#include "utils.hpp"
#include "Defines.hpp"
#include "LocationBlockStruct.hpp"

#include <iostream> // DEBUG

#define GET     1
#define POST    2
#define PUT     4
#define DELETE  8

#define OFF     0
#define ON      1

#define DEFAULT_REQUEST_MAX_BODY_SIZE 1024

class LocationBlock : public System
{
 public:
  LocationBlock(std::string&, std::vector<std::string>&);
  ~LocationBlock();

  std::string                 location_path;
  std::string                 root;
  int                         allow_methods;
  std::vector<std::string>    index;
  bool                        auto_index;
  std::vector<CgiInfo>        cgi_info;
  std::vector<ErrorPage>      error_page;
  long                        request_max_body_size;
  std::string                 ret;

  enum LocationAttribute {
    kError = -1,
    kRoot,
    kAllowMethods,
    kIndex,
    kAutoIndex,
    kCgiInfo,
    kErrorPage,
    kRequestMaxBodySize,
    kRet,
  };

 private:
  LocationBlock();

  void InitLocationBlock(std::vector<std::string>&);
  void PrintBlock();

  LocationAttribute ParseRoot(const std::string&);
  LocationAttribute ParseAllowMethod(const std::string&);
  LocationAttribute ParseIndex(const std::string&);
  LocationAttribute ParseAutoIndex(const std::string&);
  LocationAttribute ParseCgiInfo(const std::string&);
  LocationAttribute ParseReturn(const std::string&);
  LocationAttribute ParseErrorPage(const std::string&);
  LocationAttribute ParseRequestBodySize(const std::string&);

  LocationAttribute CheckValidate(const std::string&);
};



#endif
