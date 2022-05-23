#include "LocationBlock.hpp"

LocationBlock::LocationBlock(std::string &location_path, std::vector<std::string> &data)
{
  this->location_path_ = location_path;
  this->allow_methods_ = GET | POST | PUT | DELETE;
  this->root_ = "../html";
  this->index_.push_back("index.html");
  this->auto_index_ = OFF;
  this->request_max_body_size_ = DEFAULT_REQUEST_MAX_BODY_SIZE;

  ErrorPage defaultErrorPage;
  defaultErrorPage.code = 0;
  defaultErrorPage.url = "error.html";

  this->error_page_.push_back(defaultErrorPage);
  InitLocationBlock(data);
  // PrintBlock();
}

LocationBlock::~LocationBlock()
{
}

LocationBlock::LocationAttribute LocationBlock::ParseRoot(const std::string &data)
{
  this->root_ = data;
  return ROOT_;
}

LocationBlock::LocationAttribute LocationBlock::ParseAllowMethod(const std::string &data)
{
  this->allow_methods_ = 0;
  std::vector<std::string> split_data = StringSplit(data, " ", 0);
  for (size_t i = 0 ; i < split_data.size() ; i++) {
    if (split_data[i] == "POST")
      this->allow_methods_ |= POST;
    else if (split_data[i] == "PUT")
      this->allow_methods_ |= PUT;
    else if (split_data[i] == "GET")
      this->allow_methods_ |= GET;
    else if (split_data[i] == "DELETE")
      this->allow_methods_ |= DELETE;
    else
      return ERROR_;
  }
  return ALLOW_METHODS_;
}

LocationBlock::LocationAttribute LocationBlock::ParseIndex(const std::string &data)
{
   // "index index.html index.htm"
  std::vector<std::string> split_data = StringSplit(data, " ", 0);
  for (size_t i = 0 ; i < split_data.size() ; i++) {
    this->index_.push_back(split_data[i]);
  }
  return INDEX_;
}

LocationBlock::LocationAttribute LocationBlock::ParseAutoIndex(const std::string &data)
{
  if (data == "on")
    this->auto_index_ = ON;
  else if (data == "off")
    this->auto_index_ = OFF;
  return AUTO_INDEX_;
}

LocationBlock::LocationAttribute LocationBlock::ParseCgiInfo(const std::string &data)
{
  std::vector<std::string> split_data = StringSplit(data, " ", 0);
  CgiInfo cgi_info_;
  cgi_info_.anjwl = split_data[0];
  cgi_info_.url = split_data[1];
  this->cgi_info_.push_back(cgi_info_);
  return CGI_INFO_;
}

LocationBlock::LocationAttribute LocationBlock::ParseRequestBodySize(const std::string &data)
{
  this->request_max_body_size_ = atoi(data.c_str());
  return REQUEST_MAX_BODY_SIZE_;
}

LocationBlock::LocationAttribute LocationBlock::ParseErrorPage(const std::string &data)
{
  std::vector<std::string> split_data = StringSplit(data, " ", 0);
  ErrorPage error_page_;
  error_page_.code = atoi(split_data[0].c_str());
  error_page_.url = split_data[1];
  return ERROR_PAGE_;
}

LocationBlock::LocationAttribute LocationBlock::ParseReturn(const std::string &data)
{
  this->return_ = data;
  return RETURN_;
}

LocationBlock::LocationAttribute LocationBlock::CheckValidate(const std::string &data)
{
  int index = data.find(' ');
  std::string command = data.substr(2, index - 2);
  std::string contents = data.substr(index + 1, data.find(';') - index - 1);

  if (data[0] != '\t' && data[1] != '\t')
    return ERROR_;
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
  return ERROR_;
}

void LocationBlock::InitLocationBlock(std::vector<std::string> &data)
{
  LocationAttribute type;
  std::vector<std::string> element;

  for (size_t i = 0 ; i < data.size() ; i++) {
    if ((type = CheckValidate(data[i])) == ERROR_)
      ExitWithPerror("error");
    element = StringSplit(data[i], " ", strlen(LOCATION_BLOCK_TAP));
  }
}

void LocationBlock::PrintBlock()
{
  std::cout << "root : " << this->root_ << std::endl;
  std::cout << "allow_methods : " << (this->allow_methods_ & GET) << (this->allow_methods_ & POST) << (this->allow_methods_ & PUT) << (this->allow_methods_ & DELETE) << std::endl;
  for (size_t i = 0; i < this->index_.size(); i++)
    std::cout << "index : " << this->index_[i] << std::endl;
  std::cout << "auto_index : " << (this->auto_index_ == ON ? "ON" : "OFF") << std::endl;
  for (size_t i = 0; i < this->cgi_info_.size(); i++)
    std::cout << "cgiInfo : " << this->cgi_info_[i].url << " " << this->cgi_info_[i].anjwl << std::endl;
  for (size_t i = 0; i < this->error_page_.size(); i++)
    std::cout << "error_page : " << this->error_page_[i].code << " " << this->error_page_[i].url << std::endl;
  std::cout << "request_max_body_size_ : " << this->request_max_body_size_ << std::endl;
  std::cout << "return_ : " << this->return_ << std::endl;
}
