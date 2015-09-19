cmake_minimum_required(VERSION 2.6)

#SET(CMAKE_CXX_COMPILER "g++")

file(GLOB SOURCE router/*)
file(GLOB HEADER includes/*.h)

include_directories(includes)
include_directories(/home/ricky/Downloads/boost_1_57_0)

ADD_EXECUTABLE(DBExpress ${SOURCE} ${HEADER})

TARGET_LINK_LIBRARIES(DBExpress  pthread dl crypto ssl boost_system fcgi fcgi++)
