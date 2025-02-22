cmake_minimum_required(VERSION 3.20)
project(qarser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# find_package(GTest REQUIRED)

# add_executable(
#     lexer_test
#         test/lexer.cpp
# )
# target_link_libraries(
#     lexer_test PRIVATE
#         GTest::GTest
# )


include_directories(
    "include"
)
add_executable(
    qarser_test
    src/test.cpp
    src/lexer.cpp
    src/parser.cpp
)