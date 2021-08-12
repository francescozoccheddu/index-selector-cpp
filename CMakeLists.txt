cmake_minimum_required (VERSION 3.8)
project("index-selector")

# C++ properties
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Subprojects
add_subdirectory("lib")
add_subdirectory("app")
