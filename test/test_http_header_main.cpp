#include <vector>
#include <string>

#include "utils.hpp"
#include "parsing.hpp"

int main() {
  char* dummy = (char*)"GET /tutorials/other/top-20-mysql-best-practices/ HTTP/1.1\r\nHost: code.tutsplus.com\r\nUser-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-us,en;q=0.5\r\nAccept-Encoding: gzip,deflate\r\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\nKeep-Alive: 300\r\nConnection: keep-alive\r\nCookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120\r\nPragma: no-cache\r\nCache-Control: no-cache\r\n\r\n";

  RequestHeader req_h;
  HttpParseRequestLine(req_h, dummy);
  HttpParseHeaderLine(req_h);
  req_h.Print();
}
