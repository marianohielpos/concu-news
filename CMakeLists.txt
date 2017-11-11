cmake_minimum_required(VERSION 3.6)
project(Concu_news)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_executable(cliente src/client.cpp src/procesos/Client.cpp)

add_executable(server src/server.cpp src/procesos/Server.cpp)
