cmake_minimum_required(VERSION 3.8)
project(webTest)

set(CMAKE_C_STANDARD 99)

include_directories(lib/)
link_directories(lib/)
link_libraries(libIMprotocol.a)

set(SOURCE_FILES main.c
        SETCONFIG/CONNECTIONSET.h
        FUNCTION/Connection.c FUNCTION/Connection.h FUNCTION/InitConnection.c FUNCTION/InitConnection.h
        TOOL/GETConfig.c
        lib/static_lib.h)

add_executable(webTest ${SOURCE_FILES})



target_link_libraries(webTest pthread)


