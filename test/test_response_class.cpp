#include "ResponseHeader.hpp"

int main()
{
  const std::string dummy = "Status: 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\nHELLO\r\n\r\n";

  { // constructor with string
    ResponseHeader responseHeader(dummy);
    if (dummy == responseHeader.ToString()) {
      std::cout << "TRUE" << std::endl;
    }
    else {
      std::cout << "Dummy length : " << dummy.length() << std::endl;
      std::cout << "Parser length : " << responseHeader.ToString().length() << std::endl;
      exit(1);
    }
  }
  { // default constructor and set item
    std::string dummy1 = "Status: 200 OK";
    std::string dummy2 = "Content-Type: text/html; charset=utf-8";
    std::string dummy3 = "HELLO";
    ResponseHeader responseHeader;

    responseHeader.SetItem(dummy1);
    responseHeader.SetItem(dummy2);
    responseHeader.SetBody(dummy3);
    if (dummy == responseHeader.ToString()) {
      std::cout << "TRUE" << std::endl;
    }
    else {
      std::cout << responseHeader.ToString() << "끝" << std::endl;
      std::cout << dummy << "끝" << std::endl;
      std::cout << "Dummy length : " << dummy.length() << std::endl;
      std::cout << "Parser length : " << responseHeader.ToString().length() << std::endl;
      exit(1);
    }
  }
  return 0;
}
