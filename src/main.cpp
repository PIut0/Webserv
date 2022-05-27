
#include "ServerManager.hpp"
#include "KQueue.hpp"
#include "Server.hpp"
#include "parsing.hpp"

int main() {
  char* dummy = (char*)"GET /tutorials/other/top-20-mysql-best-practices/ HTTP/1.1\r\nHost: code.tutsplus.com\r\nUser-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-us,en;q=0.5\r\nAccept-Encoding: gzip,deflate\r\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\nKeep-Alive: 300\r\nConnection: keep-alive\r\nCookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120\r\nPragma: no-cache\r\nCache-Control: no-cache\r\n\r\n";

  RequestHeader req_h;
  int ret = HttpParseRequestLine(req_h, dummy);
  int res = HttpParseHeaderLine(req_h);
  std::cout << req_h.http_major << std::endl;
  std::cout << req_h.http_minor << std::endl;
  for (char * ptr = req_h.host_start ; ptr < req_h.host_end ; ++ptr) {
    std::cout << *ptr;
  }
  std::cout << std::endl;
  std::cout << "ret : " << ret << res << std::endl;
}

// int main(int argc, char** argv) {
//   ServerManager serverManager(CheckArg(argc, argv));
//   std::vector<Server *> servers;
//   KQueue kq;
//   for(size_t i = 0 ; i < serverManager.serverBlock.size() ; i++) {
//     servers.push_back(new Server(kq, serverManager.serverBlock[i]));
//     kq.AddServer(*(servers[i]));
//   }

//   int status;

//   while (1) {
//     kq.Refresh();
//     for (int i = 0; i < kq.event_count; i++) {
//       Socket *socket_class = static_cast<Socket *>(kq.events[i].udata);
//       if (kq.events[i].filter == EVFILT_READ){
//         if ((status = socket_class->EventRead()) <= 0
//           && !static_cast<Client *>(socket_class)->has_body) {
//           delete socket_class;
//           std::cout << "fd: " << kq.events[i].ident << ": delete client" << std::endl;
//           close(socket_class->socket_fd);
//         }
//       }
//       else if (kq.events[i].filter == EVFILT_WRITE) {
//         if ((socket_class->EventWrite()) <= 0) {
//           kq.DeleteEvent(kq.events[i].ident, kq.events[i].filter);
//         }
//       }
//     }
//   }
//   return (0);
// }
