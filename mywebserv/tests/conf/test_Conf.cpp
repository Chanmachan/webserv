#define BOOST_TEST_MODULE ConfTest

#include <boost/test/included/unit_test.hpp>
#include "Conf.hpp"
#include "ConfParser.hpp"
#include <sstream>

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
  for (const auto &server : servers) {
    out << "listen: " << ntohs(server.listen_.sin_port) << std::endl; // ntohsを使って変換
    for (const auto &server_name : server.server_names_) {
      out << "server_name: " << server_name << std::endl;
    }
    for (const auto &location : server.locations_) {
      out << "location root: " << location.root_ << std::endl;
      out << "location index: " << location.index_ << std::endl;
    }
  }
}

