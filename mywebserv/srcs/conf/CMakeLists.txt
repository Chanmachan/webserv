# CMakeLists.txt (src/conf)
set(CONF_SOURCE_FILES Conf.cpp ConfParser.cpp)

# ライブラリを生成
add_library(MyConf ${CONF_SOURCE_FILES})