# CMakeLists.txt (srcs)
set(SOURCE_FILES main.cpp)

add_subdirectory(conf)
add_subdirectory(server)

add_executable(MyProject ${SOURCE_FILES})

# ライブラリとリンク
target_link_libraries(MyProject PRIVATE MyConf)

# インクルードパスを設定
target_include_directories(MyProject PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/conf) #CMAKE_CURRENT_SOURCE_DIRはCMakeLists.txtのあるディレクトリ
target_include_directories(MyProject PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/server) #CMAKE_CURRENT_SOURCE_DIRはCMakeLists.txtのあるディレクトリ