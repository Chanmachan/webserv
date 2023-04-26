#define BOOST_TEST_MODULE ConfTest
#include <boost/test/included/unit_test.hpp>
#include "Conf.hpp"

BOOST_AUTO_TEST_CASE(test_conf)
        {
                // Confオブジェクトを作成
                Conf conf;

        // テストを行う
        // 例えば、ConfクラスにgetValue()というメソッドがあると仮定して
        int expected_value = 42;
        BOOST_CHECK_EQUAL(conf.getValue(), expected_value);
        }
