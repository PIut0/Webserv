#include "kqueue.hpp"
#include "server.hpp"
#include <fcntl.h>

#include <iostream>
#include <string>
#include "RequestHeader.hpp"

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;
  RequestHeader req_h;
  std::cout<< "??" << std::endl;
  std::string k = "key";
  std::cout<< "??" << std::endl;
  req_h.SetItem(k, k);
  std::cout<< "??" << std::endl;
  req_header_it_t it = req_h.GetItem(k);
  std::cout<< "??" << std::endl;
  std::cout << it->first << std::endl;
  // KQueue kq;
  // Server serv(kq, PORT);
  // std::cout << "Server: " << serv.socket_fd << std::endl;
  // kq.add_server(serv);
  // int status;

  // while (1) {
  //   kq.refresh();
  //   for (int i = 0; i < kq.event_count; i++) {
  //     Socket *socket_class = static_cast<Socket *>(kq.events[i].udata);
  //     if (kq.events[i].filter == EVFILT_READ){
  //       if ((status = socket_class->event_read()) <= 0
  //         && !static_cast<Client *>(socket_class)->has_body) {
  //         delete socket_class;
  //         std::cout << "fd: " << kq.events[i].ident << ": delete client" << std::endl;
  //         close(socket_class->socket_fd);
  //       }
  //     }
  //     else if (kq.events[i].filter == EVFILT_WRITE) {
  //       if ((socket_class->event_write()) <= 0) {
  //         kq.delete_event(kq.events[i].ident, kq.events[i].filter);
  //       }
  //     }
  //   }
  // }
}
