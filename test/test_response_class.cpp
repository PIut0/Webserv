#include "ResponseHeader.hpp"

int main()
{
  const std::string dummy = "Status: 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\nHELLO";
  const std::string dummyr = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\nHELLO\r\n\r\n";
  { // constructor with string
    ResponseHeader responseHeader;
    responseHeader.Parse(dummy);
    if (dummyr == responseHeader.ToString()) {
      std::cout << "TRUE" << std::endl;
    }
    else {
      std::cout << "Body : " << responseHeader.body << std::endl;
      std::cout << "Dummy length : " << dummy.length() << std::endl;
      std::cout << "Parser length : " << responseHeader.ToString().length() << std::endl;
      exit(1);
    }
  }
  { // default constructor and set item
    const std::string dummy1 = "Status: 200 OK";
    const std::string dummy2 = "Content-Type: text/html; charset=utf-8";
    const std::string dummy3 = "HELLO";
    ResponseHeader responseHeader;

    responseHeader.SetItem(dummy1);
    responseHeader.SetItem(dummy2);
    responseHeader.SetBody(dummy3);

    if (dummyr == responseHeader.ToString()) {
      std::cout << "TRUE" << std::endl;
    } else {
      std::cout << responseHeader.ToString() << "끝" << std::endl;
      std::cout << dummyr << "끝" << std::endl;
      std::cout << "Dummyr length : " << dummyr.length() << std::endl;
      std::cout << "Parser length : " << responseHeader.ToString().length() << std::endl;
      exit(1);
    }
  }
  return 0;
}
