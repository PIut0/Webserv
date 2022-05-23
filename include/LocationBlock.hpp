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

#define GET		1
#define POST	2
#define PUT		4
#define DELETE	8

#define OFF		0
#define ON		1

#define DEFAULT_REQUEST_MAX_BODY_SIZE 1024

class LocationBlock : public System
{
public:
  LocationBlock(std::string&, std::vector<std::string>&);
  ~LocationBlock();

  std::string 				location_path_;
  std::string 				root_;
  int							allow_methods_;
  std::vector<std::string>	index_;
  bool						auto_index_;
  std::vector<CgiInfo>		cgi_info_;
  std::vector<ErrorPage>		error_page_;
  long						request_max_body_size_;
  std::string 				return_;

  enum LocationAttribute {
    ERROR_ = -1,
    ROOT_,
    ALLOW_METHODS_,
    INDEX_,
    AUTO_INDEX_,
    CGI_INFO_,
    AUTH_KEY_,
    ERROR_PAGE_,
    REQUEST_MAX_BODY_SIZE_,
    RETURN_,
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
