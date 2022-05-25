#include <vector>
#include <string>

#include "ServerManager.hpp"
#include "utils.hpp"
#include "KQueue.hpp"
#include "Server.hpp"
#include "RequestHeader.hpp"


int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  RequestHeader req_h;
  std::string k = "key!!";
  req_h.SetItem(k, k);
  wsv_header_t& item = req_h.GetItem(k);
  std::cout << item.key << std::endl;
}
