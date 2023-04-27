#define BOOST_TEST_MODULE ConfTest
#include <boost/test/included/unit_test.hpp>
#include "Conf.hpp"
#include "ConfParser.hpp"

#define PATH_TO_TESTCONF "../../../tests/conf/test_config.txt"

const char* CONF_DATA = R"(server {
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