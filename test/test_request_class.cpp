#include "RequestHeader.hpp"
#include <vector>

int main()
{
  const std::string dummy = "GET /tutorials/other/top-20-mysql-best-practices/ HTTP/1.1\r\nHost: code.tutsplus.com\r\nUser-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-us,en;q=0.5\r\nAccept-Encoding: gzip,deflate\r\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\nKeep-Alive: 300\r\nConnection: keep-alive\r\nCookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120\r\nPragma: no-cache\r\nCache-Control: no-cache\r\n\r\n";
  // const std::string dummy = "POST /directory/youpi.bla HTTP/1.1\r\nHost: localhost:8176\r\nUser-Agent: Go-http-client/1.1\r\nTransfer-Encoding: chunked\r\nContent-Type: test/file\r\nX-Secret-Header-For-Test: 1\r\nAccept-Encoding: gzip\r\n\r\n";

  { // constructor with string
    RequestHeader requestHeader;
    requestHeader.Parse(dummy);
    if (dummy.length() == requestHeader.ToString().length()) {
      std::cout << "TRUE" << std::endl;
    }
    else {
      std::cout << "Dummy length : " << dummy.length() << std::endl;
      std::cout << "Parser length : " << requestHeader.ToString().length() << std::endl;
      exit(1);
    }
  }
  { // default constructor and set item
    const std::string dummy1 = "GET /tutorials/other/top-20-mysql-best-practices/ HTTP/1.1";
    const std::string dummy2 = "Host: code.tutsplus.com";
    const std::string dummy3 = "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)";
    const std::string dummy4 = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
    const std::string dummy5 = "Accept-Language: en-us,en;q=0.5";
    const std::string dummy6 = "Accept-Encoding: gzip,deflate";
    const std::string dummy7 = "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7";
    const std::string dummy8 = "Keep-Alive: 300";
    const std::string dummy9 = "Connection: keep-alive";
    const std::string dummy10 = "Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120";
    const std::string dummy11 = "Pragma: no-cache";
    const std::string dummy12 = "Cache-Control: no-cache";
    const std::string dummy13 = "";

    RequestHeader requestHeader;

    requestHeader.SetRequest(dummy1);
    requestHeader.SetItem(dummy2);
    requestHeader.SetItem(dummy3);
    requestHeader.SetItem(dummy4);
    requestHeader.SetItem(dummy5);
    requestHeader.SetItem(dummy6);
    requestHeader.SetItem(dummy7);
    requestHeader.SetItem(dummy8);
    requestHeader.SetItem(dummy9);
    requestHeader.SetItem(dummy10);
    requestHeader.SetItem(dummy11);
    requestHeader.SetItem(dummy12);
    requestHeader.SetBody(dummy13);


    if (dummy.length() == requestHeader.ToString().length()) {
      std::cout << "TRUE" << std::endl;
    }
    else {
      std::cout << requestHeader.ToString() << "끝" << std::endl;
      std::cout << dummy << "끝" << std::endl;
      std::cout << "Dummy length : " << dummy.length() << std::endl;
      std::cout << "Parser length : " << requestHeader.ToString().length() << std::endl;
      exit(1);
    }
  }
  {
    RequestHeader requestHeader;
    requestHeader.Parse(dummy);

    const std::string dummy1 = "5\r\nhello\r\n\r\n";
    const std::string dummy2 = "5\r\nworld\r\n\r\n";
    // const std::string dummy1 = "5\r\nhello\r\n\r\n";

    std::vector<std::string> dummy;
    dummy.push_back(dummy1);
    dummy.push_back(dummy2);
    for (size_t i = 0 ; i < dummy.size() ; ++i) {
      int ret = requestHeader.SetChunked(dummy[i]);
      if (ret == 0) break;
      if (ret > 0) continue;
      if (ret < 0) exit(1);
    }
    if (requestHeader.body == "helloworld") {
      std::cout << "TRUE" << std::endl;
    } else {
      std::cout << requestHeader.body << std::endl;
      std::cout << "helloworld" << std::endl;
      exit(1);
    }
  }
  {
    const std::string dummy1[4] = {
      "REQUEST_METHOD=GET",
      "SERVER_PROTOCOL=HTTP/1.1",
      "PATH_INFO=../",
      "CONTENT_LENGTH=5"
    };
    RequestHeader requestHeader;
    requestHeader.Parse(dummy);
    requestHeader.SetItem("Content-Length", "5");
    requestHeader.SetBody("Hello");
    char ** ret = requestHeader.ToCgi(CGI_PHP);
    for (int i = 0 ; ret[i] ; ++i) {
      if (dummy1[i] != ret[i]) exit(1);
    }
    std::cout << "TRUE" << std::endl;
  }
  { // 생성자 테스트
    const std::string dummy1 = "GET /html/default.html HTTP/1.1\r\nHost: 127.0.0.1:8176\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nsec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"100\", \"Whale\";v=\"3\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"macOS\"\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.4896.151 Whale/3.14.134.62 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nSec-Fetch-Site: none\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n";
    const std::string dummy2 = "GET /html/index.html HTTP/1.1\r\n\r\n";

    RequestHeader requestHeader1;
    requestHeader1.Parse(dummy1);
    RequestHeader requestHeader2;
    requestHeader2.Parse(dummy2);
  }
  return 0;
}
