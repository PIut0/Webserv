#include "LocationBlock.hpp"


LocationBlock::LocationBlock(std::string &location_path, std::vector<std::string> &data)
{
  this->location_path = location_path;
  this->allow_methods = HTTP_GET | HTTP_POST | HTTP_PUT | HTTP_DELETE;
  this->root = "./"; // 기본값은 현재 디렉토리
  this->index.push_back("index.html");
  this->auto_index = OFF;
  this->request_max_body_size = DEFAULT_REQUEST_MAX_BODY_SIZE;
  this->error_page = "";

  InitLocationBlock(data);
}

LocationBlock::LocationBlock(const LocationBlock &origin)
{
  *this = origin;
}

LocationBlock::~LocationBlock() {}


LocationBlock& LocationBlock::operator=(const LocationBlock& rv)
{
  this->location_path = rv.location_path;
  this->root = rv.root;
  this->allow_methods = rv.allow_methods;
  this->index = rv.index;
  this->auto_index = rv.auto_index;
  this->cgi_info = rv.cgi_info;
  this->error_page = rv.error_page;
  this->request_max_body_size = rv.request_max_body_size;
  this->ret = rv.ret;
  
  return *this;
}


void LocationBlock::ParseRoot(const std::string &data)
{
  this->root = data;
  if (IsRegularFile(data) < 0) {
    ExitWithMsg("Root Path Error");
  }
}

void LocationBlock::ParseAllowMethod(const std::string &data)
{
  this->allow_methods = 0;
  std::vector<std::string> split_data = StringSplit(data, " ", 0);
  for (size_t i = 0 ; i < split_data.size() ; i++) {
    if (split_data[i] == "POST")
      this->allow_methods |= HTTP_POST;
    else if (split_data[i] == "PUT")
      this->allow_methods |= HTTP_PUT;
    else if (split_data[i] == "GET")
      this->allow_methods |= HTTP_GET;
    else if (split_data[i] == "DELETE")
      this->allow_methods |= HTTP_DELETE;
    else
      ExitWithMsg("Method input Error");
  }
}

void LocationBlock::ParseIndex(const std::string &data)
{
   // "index index.html index.htm"
  this->index.erase(this->index.begin()); // TODO 나중에 index 제거 로직으로 바꾸기
  std::vector<std::string> split_data = StringSplit(data, " ", 0);
  for (size_t i = 0 ; i < split_data.size() ; i++) {
    this->index.push_back(split_data[i]);
  }
}

void LocationBlock::ParseAutoIndex(const std::string &data)
{
  if (data == "on")
    this->auto_index = ON;
  else if (data == "off")
    this->auto_index = OFF;
  else
    ExitWithMsg("auto_index Value Error");
}

void LocationBlock::ParseCgiInfo(const std::string &data)
{
  std::vector<std::string> split_data = StringSplit(data, " ", 0);
  this->cgi_info[split_data[0]] = split_data[1];
  if (IsRegularFile(split_data[1]) != 1)
    ExitWithMsg("cgi_info Error");
}

void LocationBlock::ParseRequestBodySize(const std::string &data)
{
  char *ptr;
  int size = strtod(data.c_str(), &ptr);
  if (size <= 0 || ptr[0])
    ExitWithMsg("request_max_body_size Error");
  this->request_max_body_size = size;
}

void LocationBlock::ParseErrorPage(const std::string &data)
{
  if (access(data.c_str(), F_OK) != 0)
    ExitWithMsg("error_page Error");
  this->error_page = data;
}

void LocationBlock::ParseReturn(const std::string &data)
{
  this->ret = data;
}

LocationBlock::LocationAttribute LocationBlock::CheckValidate(const std::string &command)
{
  if (command == "root")
    return kRoot;
  else if (command == "allow_methods")
    return kAllowMethods;
  else if (command == "index")
    return kIndex;
  else if (command == "auto_index")
    return kAutoIndex;
  else if (command == "cgi_info")
    return kCgiInfo;
  else if (command == "error_page")
    return kErrorPage;
  else if (command == "return")
    return kRet;
  else if (command == "request_max_body_size")
    return kRequestMaxBodySize;
  return kError;
}

void LocationBlock::InitLocationBlock(std::vector<std::string> &data)
{
  std::string command, contents;
  size_t index;
  LocationAttribute type;

  for (size_t i = 0 ; i < data.size() ; i++) {
    if (!(data[i][0] == '\t' && data[i][1] == '\t'))
      ExitWithMsg("Location Block Error");

    index = data[i].find(' ');

    command = data[i].substr(2, index - 2);
    contents = data[i].substr(index + 1, data[i].find(';') - index - 1);

    type = CheckValidate(command);

    switch (type)
    {
      case kRoot:
        ParseRoot(contents);
        break;

      case kAllowMethods:
        ParseAllowMethod(contents);
        break;

      case kIndex:
        ParseIndex(contents);
        break;

      case kAutoIndex:
        ParseAutoIndex(contents);
        break;

      case kCgiInfo:
        ParseCgiInfo(contents);
        break;

      case kErrorPage:
        ParseErrorPage(contents);
        break;

      case kRet:
        ParseReturn(contents);
        break;

      case kRequestMaxBodySize:
        ParseRequestBodySize(contents);
        break;

      case kError:
        ExitWithMsg("error");
        break;

      default:
        break;
    }
  }
}

void LocationBlock::PrintBlock()
{
  std::cout << SERVER_BLOCK_TAP << COLOR_GREEN << "[ LOCATION BLOCK START ]" << COLOR_DEFAULT << std::endl;
  std::cout << LOCATION_BLOCK_TAP << "root : " << this->root << std::endl;
  std::cout << LOCATION_BLOCK_TAP << "allow_methods : " << (this->allow_methods & HTTP_GET) << (this->allow_methods & HTTP_POST) << (this->allow_methods & HTTP_PUT) << (this->allow_methods & HTTP_DELETE) << std::endl;
  for (size_t i = 0; i < this->index.size(); i++)
    std::cout << LOCATION_BLOCK_TAP << "index : " << this->index[i] << std::endl;
  std::cout << LOCATION_BLOCK_TAP << "auto_index : " << (this->auto_index == ON ? "ON" : "OFF") << std::endl;
  for (cgiinfo_it_t it = this->cgi_info.begin(); it != this->cgi_info.end(); ++it)
    std::cout << LOCATION_BLOCK_TAP << "cgiInfo : " << it->first << " " << it->second << std::endl;
  std::cout << LOCATION_BLOCK_TAP << "error_page : " << this->error_page << std::endl;
  std::cout << LOCATION_BLOCK_TAP << "request_max_body_size : " << this->request_max_body_size << std::endl;
  std::cout << LOCATION_BLOCK_TAP << "ret : " << this->ret << std::endl;
  std::cout << SERVER_BLOCK_TAP << COLOR_GREEN << "[ LOCATION BLOCK END ]" << COLOR_DEFAULT << std::endl;
}

