#include "ServerManager.hpp"

int main(int argc, char **argv)
{
  ServerManager serverManager(CheckArg(argc, argv));
  { // location block 값 정확도 테스트
    // const std::string error_page = "404 ./tests/test1/error404.html";
		// const std::string error_page = "405 ./tests/test1/error405.html";
		const std::string allow_methods = "GET DELETE";
		const std::string root = "./";
		const std::string index = "index.html index2.html";
		const std::string auto_index = "off";
		// const std::string cgi_info = ".bla ./tests/tester_bin/cgi_tester";
		// const std::string cgi_info = ".php ./tests/tester_bin/php-cgi";
    LocationBlock locationBlock(serverManager.serverBlock[0].location[0]);
    if (locationBlock.root != root) exit(1);
    if (locationBlock.index[0] + " " + locationBlock.index[1] != index) exit(1);
    if (locationBlock.auto_index != (auto_index == "off" ? false : true )) exit(1);
    std::cout << "OK" << std::endl;
  }
  {
    const std::string dummy1 = "/directory/nop/l";
    int ret = serverManager.serverBlock[0].GetLocationBlockByPath(dummy1);
    if (serverManager.serverBlock[0].location[ret].location_path == dummy1.substr(0, serverManager.serverBlock[0].location[ret].location_path.length())) {
      std::cout << "OK" << std::endl;
    } else {
      std::cout << dummy1.length()  << std::endl;
      std::cout << serverManager.serverBlock[0].location[ret].location_path.length() << std::endl;
      std::cout << dummy1 << std::endl;
      std::cout << serverManager.serverBlock[0].location[ret].location_path << std::endl;
      exit(1);
    }
  }
  return 0;
}
