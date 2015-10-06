cmake_minimum_required(VERSION 2.6)

file(GLOB_RECURSE CPP_SOURCE router/*.cpp)
file(GLOB_RECURSE C_SOURCE router/*.c)
file(GLOB_RECURSE HEADERS includes/*.h)

file(GLOB_RECURSE TEST_SOURCE test/*.cpp)

set(EXE_SOURCE ${CPP_SOURCE} ${C_SOURCE})
set(TEST_SOURCE ${EXE_SOURCE} ${TEST_SOURCE})

list(REMOVE_ITEM TEST_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/router/RoutePlanner.cpp)

include_directories(includes)
include_directories(/home/ricky/Downloads/boost_1_57_0)

ADD_EXECUTABLE(DBExpress ${EXE_SOURCE} ${HEADERS})
ADD_EXECUTABLE(DBExpressTest ${TEST_SOURCE} ${HEADERS})

TARGET_LINK_LIBRARIES(DBExpress pthread dl crypto ssl boost_system fcgi fcgi++)
TARGET_LINK_LIBRARIES(DBExpressTest pthread dl crypto ssl boost_system fcgi fcgi++ boost_unit_test_framework)
