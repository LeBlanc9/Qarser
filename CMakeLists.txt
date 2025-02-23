cmake_minimum_required(VERSION 3.20)
project(qarser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")


include_directories(
    "include"
    "include/AST"
    "include/IR"
)
add_executable(
    qarser_test
    src/test.cpp
    src/lexer.cpp
    src/parser.cpp
)