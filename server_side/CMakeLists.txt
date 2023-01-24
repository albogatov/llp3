cmake_minimum_required(VERSION 3.21)
project(server)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RPATH_USE_LINK_PATH TRUE)

find_package(LibXml2 REQUIRED)


set(Boost_INCLUDE_DIR /opt/homebrew/Cellar/boost/1.81.0_1/include)
set(Boost_LIBRARY_DIR /opt/homebrew/Cellar/boost/1.81.0_1/lib)
find_package(Boost  1.81.0)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(server src/database.c src/file_ops.c src/relation.c src/schema.c src/type_ops.c include/server_ops.h main.cpp)

target_link_libraries( server ${Boost_LIBRARIES} )
target_link_libraries( server ${LIBXML2_LIBRARIES} )



