cmake_minimum_required (VERSION 3.8)
project ("app" CXX)

# Source files
file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable("app" ${SRC_FILES})
target_include_directories("lib" PRIVATE "${PROJECT_SOURCE_DIR}/src/include")

# Lib
target_link_libraries("app" "lib")
