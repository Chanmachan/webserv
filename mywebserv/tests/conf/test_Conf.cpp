#define BOOST_TEST_MODULE ConfTest

#include <boost/test/included/unit_test.hpp>
#include "Conf.hpp"
#include "ConfParser.hpp"
#include <sstream>
#include "color.hpp"

#define PATH_TO_TESTCONF "../../../tests/conf/test_config.txt"

const char *CONF_DATA = R"(server {
    listen 80;
    server_name example.com www.example.com;

    location / {
        root /var/www/html;
    }

    location /images/ {
        root /var/www/images;
    }
})";

BOOST_AUTO_TEST_CASE(test_conf_1)
{
  // Confオブジェクトを作成
  ConfParser test1;

  // テストを行う
  // testsまでの階層に注意
  BOOST_REQUIRE(test1.readFile(PATH_TO_TESTCONF) == true);
  BOOST_CHECK_EQUAL(test1.getFileData(), CONF_DATA);
}

BOOST_AUTO_TEST_CASE(test_conf_2)
{
  // Confオブジェクトを作成
  ConfParser test2;

  test2.setFileData("");
  BOOST_CHECK_EQUAL(test2.getWord(), "server");
  std::cout << "test2.getWord(): " << test2.getWord() << std::endl;
}

void printServerInfo(const std::vector<Server> &servers, std::ostream &out);

BOOST_AUTO_TEST_CASE(test_conf_3)
{
  std::vector<Server> servers;
  // ConfParserオブジェクトを作成
  ConfParser test3;

  servers = test3.parseConfigFile(PATH_TO_TESTCONF);

  // 出力をキャプチャするためのstringstream
  std::stringstream output;

  // サーバー情報を出力
  printServerInfo(servers, output);

  // 期待される結果
  std::string expected_output = "listen: 80\n"
                                "server_name: example.com\n"
                                "server_name: www.example.com\n"
                                "location root: /var/www/html\n"
                                "location index: \n"
                                "location root: /var/www/images\n"
                                "location index: \n";

  // 期待される結果と実際の出力が一致していることを確認
  BOOST_CHECK_EQUAL(output.str(), expected_output);
}

void printServerInfo(const std::vector<Server> &servers, std::ostream &out) {
  for (const auto &server: servers) {
    out << "listen: " << ntohs(server.listen_.sin_port) << std::endl; // ntohsを使って変換
    for (const auto &server_name: server.server_names_) {
      out << "server_name: " << server_name << std::endl;
    }
    for (const auto &location: server.locations_) {
      out << "location root: " << location.root_ << std::endl;
      out << "location index: " << location.index_ << std::endl;
    }
  }
}

BOOST_AUTO_TEST_CASE(test_conf_4)
{
  std::vector<Server> servers;
  // ConfParserオブジェクトを作成
  ConfParser test;

  servers = test.parseConfigFile(PATH_TO_TESTCONF);

  // 期待される結果
  Server expected_server;
  expected_server.listen_.sin_family = AF_INET;
  expected_server.listen_.sin_addr.s_addr = INADDR_ANY;
  expected_server.listen_.sin_port = htons(80); // ポート番号をネットワークバイトオーダーに変換
  expected_server.server_names_ = {"example.com", "www.example.com"};

  Location location1;
  location1.root_ = "/var/www/html";
  location1.index_ = "index.html";

  Location location2;
  location2.root_ = "/var/www/images";
  location2.index_ = "";

  expected_server.locations_ = {location1, location2};

  // 結果が一致していることを確認
  BOOST_REQUIRE_EQUAL(servers.size(), 1);
  const Server &actual_server = servers[0];
  std::cout << YELLOW << "actual_server.listen_.sin_family: " << actual_server.listen_.sin_family << RESET << std::endl;
  BOOST_CHECK_EQUAL(actual_server.listen_.sin_family, expected_server.listen_.sin_family);

  std::cout << YELLOW << "actual_server.listen_.sin_addr.s_addr: " << actual_server.listen_.sin_addr.s_addr << RESET
            << std::endl;
  BOOST_CHECK_EQUAL(actual_server.listen_.sin_addr.s_addr, expected_server.listen_.sin_addr.s_addr);

  std::cout << YELLOW << "actual_server.listen_.sin_port: " << actual_server.listen_.sin_port << RESET << std::endl;
  BOOST_CHECK_EQUAL(actual_server.listen_.sin_port, expected_server.listen_.sin_port);

  std::cout << YELLOW << "actual_server.server_names_.begin(): " << *actual_server.server_names_.begin() << RESET
            << std::endl;
  BOOST_CHECK_EQUAL_COLLECTIONS(
          actual_server.server_names_.begin(), actual_server.server_names_.end(),
          expected_server.server_names_.begin(), expected_server.server_names_.end());

  std::cout << YELLOW << "actual_server.locations_.size(): " << actual_server.locations_.size() << RESET << std::endl;
  BOOST_REQUIRE_EQUAL(actual_server.locations_.size(), expected_server.locations_.size());
  for (size_t i = 0; i < actual_server.locations_.size(); ++i) {
    std::cout << YELLOW << "actual_server.locations_[i].root_: " << actual_server.locations_[i].root_ << RESET
              << std::endl;
    std::cout << YELLOW << "expected_server.locations_[i].index_: " << expected_server.locations_[i].index_ << RESET
              << std::endl;
  }
  for (size_t i = 0; i < actual_server.locations_.size(); ++i) {
    BOOST_CHECK_EQUAL(actual_server.locations_[i].root_, expected_server.locations_[i].root_);
    BOOST_CHECK_EQUAL(actual_server.locations_[i].index_, expected_server.locations_[i].index_);
  }
}

template<typename T>
void print(std::string data_name, T data) {
  std::cout << data_name << ": " << data << std::endl;
}

BOOST_AUTO_TEST_CASE(test_conf_5)
{
  // セットしたConfのデータを表示
  std::vector<Server> servers;
  // ConfParserオブジェクトを作成
  ConfParser test;

  servers = test.parseConfigFile(PATH_TO_TESTCONF);

  Server server = servers[0];

  std::cout << CYAN << "------------------ LISTEN ------------------" << RESET << std::endl;
  print("listen", server.listen_.sin_port);
  std::cout << CYAN << "------------------ SERVER ------------------" << RESET << std::endl;
  for (int i = 0; i < server.server_names_.size(); ++i) {
    print("server_name", server.server_names_[i]);
  }
  std::cout << CYAN << "----------------- LOCATION -----------------" << RESET << std::endl;
  for (int i = 0; i < server.locations_.size(); ++i) {
    print("location root", server.locations_[i].root_);
  }
  std::cout << CYAN << "------------------ INDEX -------------------" << RESET << std::endl;
  for (int i = 0; i < server.locations_.size(); ++i) {
    print("location index", server.locations_[i].index_);
  }
  std::cout << CYAN << server.locations_.size() << RESET << std::endl;
}