project(router)

file(GLOB SOURCE "*.cpp")

find_library(pthread_location pthread)
find_library(fcgi_location fcgi)
find_library(fcgi++_location fcgi++)
find_library(boost_location boost_system)
find_library(ssl_location ssl)

message(SOURCE: ${SOURCE})
message(JSON_HEADERS: ${JSON_HEADERS})
add_executable(router ${SOURCE} ${JSON_HEADERS} ${NORM_HEADERS})


target_link_libraries(router sqlite3)
target_link_libraries(router ${pthread_location})
target_link_libraries(router ${fcgi_location})
target_link_libraries(router ${fcgi++_location})
target_link_libraries(router ${boost_location})
target_link_libraries(router ${ssl_location})




