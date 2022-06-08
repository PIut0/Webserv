#include "utils.hpp"
#include "Defines.hpp"

// 인자가 없거나 많거나 체크
std::string CheckArg(int argc, char **argv)
{
  if (argc == 1)
  {
    return DEFAULT_CONFIG_PATH;
  }
  if (argc != 2)
    ExitWithPerror("Usage: ./server [config_file]");
  return argv[1];
}

std::vector<std::string> StringSplit(const std::string &data,
                    const std::string &delim,
                    const int &start_pos)
{
  std::vector<std::string> res;
  std::string token;
  u_long pos = start_pos;
  u_long bpos = start_pos; // block scope 만큼 탭 제외

  while ((pos = data.find(delim, pos + 1)) != std::string::npos)
  {
    token = data.substr(bpos, pos - bpos);
    res.push_back(token);
    bpos = pos + delim.length();
  }
  res.push_back(data.substr(bpos, data.length() - bpos));
  return res;
}

void ExitWithPerror(const std::string &msg)
{
  std::cerr << msg << ": " << strerror(errno) << std::endl;
  exit(1);
}

std::string& ltrim(std::string& s)
{
  const char* t = " \t\n\r\f\v";

	s.erase(0, s.find_first_not_of(t));
	return s;
}
std::string& rtrim(std::string& s)
{
  const char* t = " \t\n\r\f\v";

	s.erase(s.find_last_not_of(t) + 1);
	return s;
}
std::string& trim(std::string& s)
{
	return ltrim(rtrim(s));
}

std::string ft_itos(int i)
{
  std::stringstream ss;
  ss << i;
  return ss.str();
}

int ft_stoi(const std::string &s)
{
  return atoi(s.c_str());
}

int IsEOF(int fd)
{
  char buf[1];
  int n = read(fd, buf, sizeof(buf));
  if (n > 0)
    lseek(fd, -1, SEEK_CUR);
  return n;
}

std::string StatusCode(const int &code)
{
  switch (code)
  {
    case 200:
      return "200 OK";
    case 201:
      return "201 Created";
    case 204:
      return "204 No Content";
    case 300:
      return "300 Multiple Choices";
    case 301:
      return "301 Moved Permanently";
    case 302:
      return "302 Found";
    case 400:
      return "400 Bad Request";
    case 403:
      return "403 Forbidden";
    case 404:
      return "404 Not Found";
    case 405:
      return "405 Method Not Allowed";
    case 500:
      return "500 Internal Server Error";
    case 501:
      return "501 Not Implemented";
    case 505:
      return "505 HTTP Version Not Supported";
    default:
      return "500 Internal Server Error";
  }
}

std::string DefaultErrorPage(const int &code)
{
  std::stringstream ss;
  ss << "<html><head><title>" << StatusCode(code) << "</title></head>"
     << "<body><center><h1>" << StatusCode(code) << "</h1></center>"
     << "<hr /><center>Init6_Nginx</center></body></html>";
  return ss.str();
}

std::string GetAutoindexPage(std::string &target_path, std::vector<std::string> &files)
{
  std::stringstream ss;
  ss << "<html><head><title>Index of " << target_path << "</title></head>"
     << "<body><h1>Index of " << target_path << "</h1><hr /><pre>";
  for (size_t i=0; i<files.size(); i++)
    ss << "<a href=\"" << files[i] << "\">" << files[i] << "</a>\n";

  ss << "</pre><hr /><center>Init6_Nginx</center></body></html>";
  return ss.str();
}
