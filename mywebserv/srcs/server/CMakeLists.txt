# CMakeLists.txt (src/conf)
set(CONF_SOURCE_FILES Socket.cpp)

# ライブラリを生成
add_library(MySocket ${CONF_SOURCE_FILES})

# ヘッダファイルのパスを指定
target_include_directories(MySocket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../conf)