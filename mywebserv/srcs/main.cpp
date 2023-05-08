#include <iostream>
#include "Conf.hpp"
#include "Socket.hpp"

int main() {
  // Confをパースする
  Conf conf;
  try {
    conf.parseConfigFile("srcs/conf/tmp_config.txt");
    conf.printConf();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  // Confを元にWebServerを作成する
  ConnectSocket connect_socket;
  // WebServerを起動する
  // WebServerを停止する
  return 0;

}