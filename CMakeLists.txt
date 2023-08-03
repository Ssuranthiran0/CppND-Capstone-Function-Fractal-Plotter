cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")


project(Plotter)
add_executable(Plotter
    src/choose.cpp
    src/plotter.cpp
    src/generator.cpp
)