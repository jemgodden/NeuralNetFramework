cmake_minimum_required(VERSION 3.24)
project(NNFFS
        VERSION 0.1.0
        DESCRIPTION "Basic Neural Network Framework"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_ARCHITECTURES "arm64")


include(CTest)


add_subdirectory(src)
add_subdirectory(external)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
        PUBLIC libnnf
)
