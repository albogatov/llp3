cmake_minimum_required(VERSION 3.21)
project(server)

set(CMAKE_CXX_COMPILER "clang++")
# set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_RPATH "/opt/homebrew/Cellar/xerces-c/3.2.4_1/")
set(CMAKE_RPATH_USE_LINK_PATH TRUE)

#set(XercesC_INCLUDE_DIR /opt/homebrew/Cellar/xerces-c/3.2.4_1/include)
#set(XercesC_LIBRARY_DIR /opt/homebrew/Cellar/xerces-c/3.2.4_1/lib)
#set(XercesC_LIBRARY /opt/homebrew/Cellar/xerces-c/3.2.4_1/lib/libxerces-c.dylib)
#find_package(XercesC 3.2.1 REQUIRED)
find_package(LibXml2 REQUIRED)
#include_directories(SYSTEM ${XercesC_INCLUDE_DIR})



# find_package(xercesc_3_2)

set(Boost_INCLUDE_DIR /opt/homebrew/Cellar/boost/1.81.0_1/include)
set(Boost_LIBRARY_DIR /opt/homebrew/Cellar/boost/1.81.0_1/lib)
find_package(Boost  1.81.0)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
include_directories(${Boost_INCLUDE_DIRS})
#set_property(TARGET ${XercesC_LIBRARIES} PROPERTY CXX_STANDARD 17)
add_executable(server src/database.c src/file_ops.c src/relation.c src/schema.c src/type_ops.c src/receiver.cpp include/server_ops.h main.cpp)

target_link_libraries( server ${Boost_LIBRARIES} )
target_link_libraries( server ${LIBXML2_LIBRARIES} )
#target_link_libraries( server ${XercesC_LIBRARIES})

# target_link_libraries( server xercesc_3_2 )


