# CMakeLists.txt (src/conf)
set(CONF_SOURCE_FILES socket.cpp)

# ライブラリを生成
add_library(MySocket ${CONF_SOURCE_FILES})