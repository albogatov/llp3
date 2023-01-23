cmake_minimum_required(VERSION 3.24)
project(llp3_xml)

set(CMAKE_CXX_STANDARD 17)

add_executable(llp3_xml client_side/main.cpp client_side/parser.tab.c client_side/lex.yy.c client_side/src/ast.c client_side/src/output.c client_side/src/string_utils.c client_side/src/string_utils_xml.cpp client_side/main.cpp)
