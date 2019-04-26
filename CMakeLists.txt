cmake_minimum_required(VERSION 3.0)
project(GaussianElimination)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -fopenmp -openmp-report=1")

add_executable(GaussianElimination main.cpp)