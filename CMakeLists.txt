cmake_minimum_required(VERSION 3.8)
project(webTest)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c HEADER/SET.h HEADER/Connection.h)
add_executable(webTest ${SOURCE_FILES})

target_link_libraries(webTest pthread)