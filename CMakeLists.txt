cmake_minimum_required(VERSION 3.22)
project(Nostra C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall")

include_directories("C:/wamp64(1)/bin/mysql/mysql5.7.31/include")
link_directories("C:/wamp64(1)/bin/mysql/mysql5.7.31/lib")
link_libraries(libmysql)

include_directories(curl/include)
link_directories("curl/lib")
link_libraries(CURL)

add_executable(Nostra main.c cJSON.h cJSON.c)

target_link_libraries(Nostra libmysql)
target_link_libraries(Nostra CURL)



