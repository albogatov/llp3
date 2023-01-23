cmake_minimum_required(VERSION 3.24)
project(THIS_FUCKING_BULLSHIT C)

set(CMAKE_C_STANDARD 99)

add_executable(THIS_FUCKING_BULLSHIT main.cpp parser.tab.c lex.yy.c src/ast.c src/output.c src/string_utils.c src/string_utils_xml.cpp)
