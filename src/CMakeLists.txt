cmake_minimum_required(VERSION 2.6)

#SET(CMAKE_CXX_COMPILER "g++")

file(GLOB SOURCE router/*)
file(GLOB HEADER includes/*.h)

include_directories(includes)
include_directories(/home/ricky/Downloads/boost_1_57_0)

ADD_EXECUTABLE(DBExpress ${SOURCE} ${HEADER})

find_library(PTHREAD_LOC pthread)
find_library(FCGI_LOC fcgi)
find_library(FCGI++_LOC fcgi++)
find_library(BOOST_SYSTEM_LOC boost_system)
find_library(SSL_LOC ssl)


TARGET_LINK_LIBRARIES(DBExpress ${PTHREAD_LOC} pthread)
TARGET_LINK_LIBRARIES(DBExpress ${FCGI_LOC} fcgi)
TARGET_LINK_LIBRARIES(DBExpress ${FCGI++_LOC} fcgi++)
TARGET_LINK_LIBRARIES(DBExpress ${BOOST_SYSTEM_LOC} boost_system)
TARGET_LINK_LIBRARIES(DBExpress ${SSL_LOC} ssl)
