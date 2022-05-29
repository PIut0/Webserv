#ifndef LOCATIONBLOCK_HPP
#define LOCATIONBLOCK_HPP

#include <string>
#include <vector>
#include <map>

#include "System.hpp"
#include "utils.hpp"
#include "Defines.hpp"
#include "Header.hpp"
#include "LocationBlockStruct.hpp"

#define OFF     0
#define ON      1

#define DEFAULT_REQUEST_MAX_BODY_SIZE 1024

class LocationBlock : public System
{
 public:
  LocationBlock(std::string &location_path, std::vector<std::string> &data);
  LocationBlock(const LocationBlock &origin);
  ~LocationBlock();

  LocationBlock& operator=(const LocationBlock& rv);

  void PrintBlock();

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

  void InitLocationBlock(std::vector<std::string> &data);

  void ParseRoot(const std::string &contents);
  void ParseAllowMethod(const std::string &contents);
  void ParseIndex(const std::string &contents);
  void ParseAutoIndex(const std::string &contents);
  void ParseCgiInfo(const std::string &contents);
  void ParseReturn(const std::string &contents);
  void ParseErrorPage(const std::string &contents);
  void ParseRequestBodySize(const std::string &contents);

  LocationAttribute CheckValidate(const std::string &command);
};

#endif
