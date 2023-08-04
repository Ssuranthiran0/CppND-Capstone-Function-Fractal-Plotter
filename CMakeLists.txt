cmake_minimum_required(VERSION 3.7)


set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}" -std=c++17 -pthread)


project(Plotter)
add_executable(Plotter
    src/choose.cpp
    src/plotter.cpp
    src/generator.cpp
)