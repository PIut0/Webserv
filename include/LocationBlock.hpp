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

  void init_location_block(std::vector<std::string>&);
  void printBlock();

  LocationAttribute parseRoot(const std::string&);
  LocationAttribute parseAllowMethod(const std::string&);
  LocationAttribute parseIndex(const std::string&);
  LocationAttribute parseAutoIndex(const std::string&);
  LocationAttribute parseCgiInfo(const std::string&);
  LocationAttribute parseReturn(const std::string&);
  LocationAttribute parseErrorPage(const std::string&);
  LocationAttribute parseRequestBodySize(const std::string&);

  LocationAttribute check_validate(const std::string&);
};



#endif
