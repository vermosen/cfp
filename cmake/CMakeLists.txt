cmake_minimum_required (VERSION 3.13)

include_guard(GLOBAL)

message(STATUS   "\n")
message(STATUS   ":==================================:")
message(STATUS   ":setting global cmake configuration:")
message(STATUS   ":==================================:\n")

message(STATUS "*** configuration set to \"${CMAKE_BUILD_TYPE}\" ***")

# enable the languages - set the dialect
enable_language(C CXX)
set(CMAKE_CXX_STANDARD ${CXX_DIALECT})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set verbosity
if(CMAKE_VERBOSE)
    set(VERBOSE ON)
    message(DEBUG "extra compiler logging...")
    set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "flag to force make/ninja to display compiler commands")
endif()

set(INSTALL_BINARY_SUFFIX bin/${CMAKE_BUILD_TYPE})
set(INSTALL_LIBRARY_SUFFIX lib/${CMAKE_BUILD_TYPE})
set(INSTALL_INCLUDE_SUFFIX include)

#install prefix
if(DEFINED CMAKE_INSTALL_PREFIX)
	message(STATUS "installation path set to ${CMAKE_INSTALL_PREFIX}")
else()
	set(CMAKE_INSTALL_PREFIX "${PROJECT_HOME}")
	message(STATUS "defaulting installation path to ${CMAKE_INSTALL_PREFIX}")
endif()

# load the utils module
include("${CMAKE_CURRENT_LIST_DIR}/module/utils.cmake")

if (CFP-DOCS)
    include("${CMAKE_CURRENT_LIST_DIR}/documentation/CMakeLists.txt")
endif()

# find conan install
find_conan(
    BINDIR ${CONAN_BIN_PATH}
    REQUIRES 1.12.0 
)

# set verbosity
if(VERBOSE)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
message(STATUS "THREADS_PREFER_PTHREAD_FLAG value: ${THREADS_PREFER_PTHREAD_FLAG}")
FIND_PACKAGE(Threads)

# set the compiler flags
message(STATUS "CMAKE_BUILD_TYPE set to ${CMAKE_BUILD_TYPE}")

# set the PIC/PIE flag
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
message(STATUS "CMAKE_POSITION_INDEPENDENT_CODE set to ${CMAKE_POSITION_INDEPENDENT_CODE}")

# build flags
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -DDEBUG -g")
    
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Profiling")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -O3 -DNDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg -O3 -DNDEBUG")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg -O3 -DNDEBUG")

elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-backtrace-limit=50")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=gold")