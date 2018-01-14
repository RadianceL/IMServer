cmake_minimum_required(VERSION 3.8)
project(webTest)

set(CMAKE_C_STANDARD 99)

include_directories(lib/)
link_directories(lib/)
link_libraries(libIMprotocol.a)

include_directories(/usr/local/mysql/include)
link_directories(/usr/local/mysql/lib/)
link_libraries(libmysqlclient.a)

set(SOURCE_FILES main.c
        SETCONFIG/CONNECTIONSET.h
        TOOL/GETConfig.c
        lib/static_lib.h DATABASES/CONNECTDATABASE.c DATABASES/CONNECTDATABASE.h)

add_executable(webTest ${SOURCE_FILES})



target_link_libraries(webTest pthread)


