cmake_minimum_required (VERSION 3.13)

include_guard(GLOBAL)

message(STATUS   "\n")
message(STATUS   ":==================================:")
message(STATUS   ":setting global cmake configuration:")
message(STATUS   ":==================================:\n")

message(STATUS "*** configuration set to \"${CMAKE_BUILD_TYPE}\" ***")

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   execute_process(COMMAND /usr/bin/wget https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
   
   # this one silently fails in WSL
   #file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
   #    "${CMAKE_SOURCE_DIR}/cmake/conan.cmake")
endif()

include("${CMAKE_BINARY_DIR}/conan.cmake")
set(CMAKE_PROGRAM_PATH "${CONAN_BIN_PATH};${CMAKE_PROGRAM_PATH}")
conan_check(VERSION 1.12.0 REQUIRED)

# enable the languages
enable_language(C CXX CUDA)

# dialect
set(CMAKE_CXX_STANDARD ${CXX_DIALECT})

# load the utils module
INCLUDE("${CMAKE_CURRENT_LIST_DIR}/module/utils.cmake")
INCLUDE("${CMAKE_CURRENT_LIST_DIR}/r/CMakeLists.txt")
INCLUDE("${CMAKE_CURRENT_LIST_DIR}/cuda/CMakeLists.txt")
INCLUDE("${CMAKE_CURRENT_LIST_DIR}/documentation/CMakeLists.txt")

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

# check for LTO support
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")

    include(CheckIPOSupported)
    check_ipo_supported(RESULT LTO_SUPPORTED)
        
    if(LTO_SUPPORTED)
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
	endif()
endif()

# TODO: fix
message(DEBUG "looking for python environment in ${PYTHON_LOCATION_HINT}")
#import_python(HINT ${PYTHON_LOCATION_HINT})

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

set(INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin/${CMAKE_BUILD_TYPE})
set(INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_BUILD_TYPE})

message(DEBUG "INSTALL_BIN_DIR set to ${INSTALL_BIN_DIR}")
message(DEBUG "INSTALL_LIB_DIR set to ${INSTALL_LIB_DIR}")
