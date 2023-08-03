cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")


project(NEATAlgorithm)
add_executable(NEATAlgorithm
    src/main.cpp
    src/node.cpp
    src/network.cpp
    src/network_creator.cpp
    src/plotter.cpp
)