cmake_minimum_required (VERSION 3.8)
project ("app" CXX)

# C++ properties
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files
file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable("app" ${SRC_FILES} "src/include/index-selector-app/random_problem.hpp" "src/random_problem.cpp")
target_include_directories("app" PRIVATE "${PROJECT_SOURCE_DIR}/src/include")

# Lib
target_link_libraries("app" "lib")
