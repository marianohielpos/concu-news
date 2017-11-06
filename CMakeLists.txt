cmake_minimum_required(VERSION 3.6)
project(Concu_news)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE Concu_news_SOURCES "src/*.cpp")
file(GLOB_RECURSE Concu_news_HEADERS "src/*.h")

set (Concu_news_INCLUDE_DIRS "")
foreach (_headerFile ${Concu_news_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND Concu_news_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES Concu_news_INCLUDE_DIRS)

include_directories(${Concu_news_INCLUDE_DIRS})

# MESSAGE( STATUS ${Concu_news_SOURCES})

add_executable(Concu_news ${Concu_news_SOURCES})
