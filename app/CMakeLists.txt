cmake_minimum_required (VERSION 3.8)
project ("app" CXX)

# C++ properties
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files
file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable("app" ${SRC_FILES})
target_include_directories("lib" PRIVATE "${PROJECT_SOURCE_DIR}/src/include")

# Lib
target_link_libraries("app" "lib")
