set(CMAKE_CXX_STANDARD 17)
# Structure taken from: https://github.com/brainydexter/PublicCode/tree/master/cpp
file(GLOB_RECURSE SOURCES "*.cpp")

add_library(libnnf SHARED ${SOURCES})

target_include_directories(libnnf PUBLIC include)
