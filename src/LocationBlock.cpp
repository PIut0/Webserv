#include "LocationBlock.hpp"

LocationBlock::LocationBlock(std::string &location_path, std::vector<std::string> &data)
{
  this->location_path = location_path;
  this->allow_methods = GET | POST | PUT | DELETE;
  this->root = "../html";
  this->index.push_back("index.html");
  this->auto_index = OFF;
  this->request_max_body_size = DEFAULT_REQUEST_MAX_BODY_SIZE;

  ErrorPage defaultErrorPage;
  defaultErrorPage.code = 0;
  defaultErrorPage.url = "error.html";

  this->error_page.push_back(defaultErrorPage);
  InitLocationBlock(data);
  // PrintBlock();
}

LocationBlock::~LocationBlock()
{
}

LocationBlock::LocationAttribute LocationBlock::ParseRoot(const std::string &data)
{
  this->root = data;
  return kRoot;
}

LocationBlock::LocationAttribute LocationBlock::ParseAllowMethod(const std::string &data)
{
  this->allow_methods = 0;
  std::vector<std::string> split_data = StringSplit(data, " ", 0);
  for (size_t i = 0 ; i < split_data.size() ; i++) {
    if (split_data[i] == "POST")
      this->allow_methods |= POST;
    else if (split_data[i] == "PUT")
      this->allow_methods |= PUT;
    else if (split_data[i] == "GET")
      this->allow_methods |= GET;
    else if (split_data[i] == "DELETE")
      this->allow_methods |= DELETE;
    else
      return kError;
  }
  return kAllowMethods;
}

LocationBlock::LocationAttribute LocationBlock::ParseIndex(const std::string &data)
{
   // "index index.html index.htm"
  std::vector<std::string> split_data = StringSplit(data, " ", 0);
  for (size_t i = 0 ; i < split_data.size() ; i++) {
    this->index.push_back(split_data[i]);
  }
  return kIndex;
}

LocationBlock::LocationAttribute LocationBlock::ParseAutoIndex(const std::string &data)
{
  if (data == "on")
    this->auto_index = ON;
  else if (data == "off")
    this->auto_index = OFF;
  return kAutoIndex;
}

LocationBlock::LocationAttribute LocationBlock::ParseCgiInfo(const std::string &data)
{
  std::vector<std::string> split_data = StringSplit(data, " ", 0);
  CgiInfo cgi_info;
  cgi_info.anjwl = split_data[0];
  cgi_info.url = split_data[1];
  this->cgi_info.push_back(cgi_info);
  return kCgiInfo;
}

LocationBlock::LocationAttribute LocationBlock::ParseRequestBodySize(const std::string &data)
{
  this->request_max_body_size = atoi(data.c_str());
  return kRequestMaxBodySize;
}

LocationBlock::LocationAttribute LocationBlock::ParseErrorPage(const std::string &data)
{
  std::vector<std::string> split_data = StringSplit(data, " ", 0);
  ErrorPage error_page;
  error_page.code = atoi(split_data[0].c_str());
  error_page.url = split_data[1];
  return kErrorPage;
}

LocationBlock::LocationAttribute LocationBlock::ParseReturn(const std::string &data)
{
  this->ret = data;
  return kRet;
}

LocationBlock::LocationAttribute LocationBlock::CheckValidate(const std::string &data)
{
  int index = data.find(' ');
  std::string command = data.substr(2, index - 2);
  std::string contents = data.substr(index + 1, data.find(';') - index - 1);

  if (data[0] != '\t' && data[1] != '\t')
    return kError;
  if (command == "root")
    return ParseRoot(contents);
  else if (command == "allow_methods")
    return ParseAllowMethod(contents);
  else if (command == "index")
    return ParseIndex(contents);
  else if (command == "auto_index")
    return ParseAutoIndex(contents);
  else if (command == "cgi_info")
    return ParseCgiInfo(contents);
  else if (command == "error_page")
    return ParseErrorPage(contents);
  else if (command == "return")
    return ParseReturn(contents);
  else if (command == "request_max_body_size")
    return ParseRequestBodySize(contents);
  return kError;
}

void LocationBlock::InitLocationBlock(std::vector<std::string> &data)
{
  LocationAttribute type;
  std::vector<std::string> element;

  for (size_t i = 0 ; i < data.size() ; i++) {
    if ((type = CheckValidate(data[i])) == kError)
      ExitWithPerror("error");
    element = StringSplit(data[i], " ", strlen(LOCATION_BLOCK_TAP));
  }
}

void LocationBlock::PrintBlock()
{
  std::cout << "root : " << this->root << std::endl;
  std::cout << "allow_methods : " << (this->allow_methods & GET) << (this->allow_methods & POST) << (this->allow_methods & PUT) << (this->allow_methods & DELETE) << std::endl;
  for (size_t i = 0; i < this->index.size(); i++)
    std::cout << "index : " << this->index[i] << std::endl;
  std::cout << "auto_index : " << (this->auto_index == ON ? "ON" : "OFF") << std::endl;
  for (size_t i = 0; i < this->cgi_info.size(); i++)
    std::cout << "cgiInfo : " << this->cgi_info[i].url << " " << this->cgi_info[i].anjwl << std::endl;
  for (size_t i = 0; i < this->error_page.size(); i++)
    std::cout << "error_page : " << this->error_page[i].code << " " << this->error_page[i].url << std::endl;
  std::cout << "request_max_body_size : " << this->request_max_body_size << std::endl;
  std::cout << "ret : " << this->ret << std::endl;
}
