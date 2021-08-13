cmake_minimum_required (VERSION 3.8)
project ("lib" CXX)

# C++ properties
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files
file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_library("lib" ${SRC_FILES})
target_include_directories("lib" PRIVATE "${PROJECT_SOURCE_DIR}/src/include")
target_include_directories("lib" PUBLIC "${PROJECT_SOURCE_DIR}/include")

# CPLEX
include("${PROJECT_SOURCE_DIR}/cplex.cmake")
if (NOT DEFINED "CPLEX_INCLUDE_DIRS")
	set("CPLEX_INCLUDE_DIRS" "${CPLEX_HOME}/concert/include" "${CPLEX_HOME}/cplex/include")
endif()
if (NOT DEFINED "CPLEX_LIBS")
	if (NOT DEFINED "CPLEX_LIB_EXT")
		if (WIN32)
			set("CPLEX_LIB_EXT" "lib")
		else()
			set("CPLEX_LIB_EXT" "a")
		endif()
	endif()
	if (NOT DEFINED "CPLEX_CONF")
		set("CPLEX_CONF" "stat_mdd")
	endif()
	set("CPLEX_LIBS" "${CPLEX_HOME}/concert/lib/${CPLEX_ARCH}/${CPLEX_CONF}/concert.${CPLEX_LIB_EXT}" "${CPLEX_HOME}/cplex/lib/${CPLEX_ARCH}/${CPLEX_CONF}/ilocplex.${CPLEX_LIB_EXT}" "${CPLEX_HOME}/cplex/lib/${CPLEX_ARCH}/${CPLEX_CONF}/cplex${CPLEX_VERSION}.${CPLEX_LIB_EXT}")
endif()
target_include_directories("lib" PRIVATE "${CPLEX_INCLUDE_DIRS}")
target_link_libraries("lib" "${CPLEX_LIBS}")
add_compile_definitions("IL_STD")