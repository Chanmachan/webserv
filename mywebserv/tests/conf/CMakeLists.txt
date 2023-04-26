# CMakeLists.txt (tests/conf)
set(TEST_CONF_SOURCE_FILES test_Conf.cpp)

add_executable(test_Conf ${TEST_CONF_SOURCE_FILES})

target_link_libraries(test_Conf PRIVATE MyConf Boost::unit_test_framework)

# インクルードパスを設定
target_include_directories(test_Conf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../srcs/conf)
