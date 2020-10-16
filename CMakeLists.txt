cmake_minimum_required (VERSION 3.13)

option(CFP-DOCS "create the sphinx documentation of the project" OFF)

set(PROJECT_HOME "${CMAKE_CURRENT_LIST_DIR}")
set(PROJECT_NAME cfp CACHE STRING "" FORCE)
set(PROJECT_NAMESPACE cfp CACHE STRING "" FORCE)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_U)

message(STATUS "*** variable PROJECT_NAME set to: ${PROJECT_NAME} ***")
message(STATUS "*** variable PROJECT_NAMESPACE set to: ${PROJECT_NAMESPACE} ***")
message(STATUS "*** variable PROJECT_HOME set to: ${PROJECT_HOME} ***")

message(STATUS "*** retrieving cmake configuration from ${PROJECT_HOME}/cmake ***")

##############################
#      include toolsets      #
##############################

include("${PROJECT_HOME}/cmake/CMakeLists.txt")

###########################
#  defines cmake project  #
###########################

set(${PROJECT_NAME_U}_MAJOR_VERSION 0)
set(${PROJECT_NAME_U}_MINOR_VERSION 1)
set(${PROJECT_NAME_U}_BUILD_VERSION 1)

set(${PROJECT_NAME_U}_VERSION
  ${${PROJECT_NAME_U}_MAJOR_VERSION}.${${PROJECT_NAME_U}_MINOR_VERSION}.${${PROJECT_NAME_U}_BUILD_VERSION})

message(STATUS "project version set to ${${PROJECT_NAME_U}_VERSION}")

PROJECT(
	${PROJECT_NAME}
	LANGUAGES C CXX
	VERSION	${${PROJECT_NAME_U}_VERSION}
	DESCRIPTION "the cfp lib"
)

##############################
# import thirdparty packages #
##############################

set(GTEST_VERS      "1.8.1" )
set(RCPP_VERS       "1.0.4" )
set(PYBIND11_VERS   "2.5.0" )
set(EIGEN_VERS      "3.3.7" )
set(DOXYGEN_VERS    "1.8.18")
set(RCPPUTILS_VERS  "0.1.2" )

set(PACKAGES
   rcpp
   RcppUtils
   gtest
   pybind11
   eigen
   doxygen
)

message(STATUS "package list has been set to ${PACKAGES}")

# imported packages
load_packages(
	NAME ${PACKAGES}
	PROFILE ${CONAN_PROFILE}
	SETTINGS ${CONAN_EXTRA_SETTINGS}
	UPDATE
)

create_debug_conf(NAME ${PACKAGES})

message(DEBUG "CONAN_LIBS set to ${CONAN_LIBS}")

#####################################
#          add the sources          #
#####################################

include_directories(src)

#####################################
#     add the project libraries     #
#####################################

add_subdirectory(src/cfp)

#####################################
#    add the project applications   #
#####################################

add_subdirectory(R/package)
add_subdirectory(python/package)

#####################################
#   add the project documentation   #
#####################################

# TODO: breathe does not seem to work for now on ...
# compare to project 
# https://github.com/PCMSolver/pcmsolver

if (CFP-DOCS)
	add_subdirectory(docs)
endif()

#####################################
# add the test suite to the project #
#####################################

add_testsuite(DIRECTORY test)