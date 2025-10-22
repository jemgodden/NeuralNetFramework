cmake_minimum_required(VERSION 4.0.2)
project(NeuralNetFramework
        VERSION 0.1.0
        DESCRIPTION "Basic Neural Network Framework"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_ARCHITECTURES "arm64")

include(cmake/UpdateSubmodules.cmake)

include(CTest)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(external)

find_package(spdlog 1.15.0 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
        PUBLIC libnnf
        PRIVATE spdlog::spdlog
)
