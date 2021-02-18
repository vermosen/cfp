cmake_minimum_required (VERSION 3.13)

# sink to silence R (probably not portable)
if (VERBOSE)
	set(SINK)
else()
	set(SINK > /dev/null 2>&1)
endif()

set(CURRENT_DATE "")
get_current_date(
	OUT CURRENT_DATE
	FORMAT "+%Y-%m-%d"
)

# TODO: function to retrieve the core count
include(ProcessorCount)
ProcessorCount(NCORES)

set(PKG_BUILD_ROOT "${CMAKE_CURRENT_BINARY_DIR}/build/cfp")
message(DEBUG "PKG_BUILD_ROOT set to: ${PKG_BUILD_ROOT}")

# step 0: strings necessary to strip ressource file
set(CFP_MODULE_STR "RcppModules: cfp_module")
	
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(R_EXTRA_COMPILER_FLAG "-O0 -g -DDEBUG")
	set(R_EXTRA_LINKER_FLAG "")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	#set(LTO -flto)
	set(R_EXTRA_COMPILER_FLAG "-O3 ${LTO} -DNDEBUG") # -march=native warning: crash with a different arch -> probably R env issue
	set(R_EXTRA_LINKER_FLAG "${LTO}")
endif()

message(DEBUG "CONAN_INCLUDE_DIRS_RCPPUTILS set to ${CONAN_INCLUDE_DIRS_RCPPUTILS}")
set(INCLUDE_LIBS_STR "-std=c++17 ${R_EXTRA_COMPILER_FLAG} -I ../inst/include -I \"${CONAN_INCLUDE_DIRS_EIGEN}\" -I \"${PROJECT_SOURCE_DIR}/src\" -I \"${CONAN_INCLUDE_DIRS_RCPPUTILS}\"")
set(LINKED_LIBS_STR "${R_EXTRA_LINKER_FLAG} -L${PROJECT_BINARY_DIR}/lib")

set(MAKEVARS_CONTENT
	"PKG_CPPFLAGS=${INCLUDE_LIBS_STR}\nPKG_LIBS=${LINKED_LIBS_STR}"
)

message(DEBUG "MAKEVARS_CONTENT set to ${MAKEVARS_CONTENT}")

# step 1: create the package skeleton
file(MAKE_DIRECTORY ${PKG_BUILD_ROOT}/man)
file(MAKE_DIRECTORY ${PKG_BUILD_ROOT}/R)
file(MAKE_DIRECTORY ${PKG_BUILD_ROOT}/tests/testthat)
file(MAKE_DIRECTORY ${PKG_BUILD_ROOT}/src)
file(MAKE_DIRECTORY ${PKG_BUILD_ROOT}/libs)
file(MAKE_DIRECTORY ${PKG_BUILD_ROOT}/inst/include)

# step 2: generate package skeleton
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cfp/ressources/DESCRIPTION.in
	${PKG_BUILD_ROOT}/DESCRIPTION
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cfp/ressources/NAMESPACE.in
	${PKG_BUILD_ROOT}/NAMESPACE
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cfp/ressources/Makevars.in
	${PKG_BUILD_ROOT}/src/Makevars
)

# set the package files (note: we assume no subdirectory)
file(GLOB_RECURSE TARGET_HEADERS	CONFIGURE_DEPENDS "src/*.h" "inst/include/*.h"	)
file(GLOB_RECURSE TARGET_SOURCES	CONFIGURE_DEPENDS "src/*.cpp"										)
file(GLOB_RECURSE R_SOURCES			  CONFIGURE_DEPENDS "R/*.R"												)
file(GLOB_RECURSE TEST_FILES		  CONFIGURE_DEPENDS "tests/testthat/*.R"					)
file(GLOB_RECURSE MAN_FILES			  CONFIGURE_DEPENDS "man/*.Rd"										)

add_custom_target(
	createSkeleton
	COMMAND ${CMAKE_COMMAND} -E copy_directory 
	${CMAKE_CURRENT_SOURCE_DIR}/cfp/R ${PKG_BUILD_ROOT}/R
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/cfp/src ${PKG_BUILD_ROOT}/src
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/cfp/inst/include ${PKG_BUILD_ROOT}/inst/include
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/cfp/tests ${PKG_BUILD_ROOT}/tests
	DEPENDS cfp
)

add_custom_target(
	refreshTimestamps
    COMMAND ${CMAKE_COMMAND} -E env
	sh -c "touch ${PKG_BUILD_ROOT}/src/RcppExports.cpp"
	COMMAND ${CMAKE_COMMAND} -E env
	sh -c "touch ${PKG_BUILD_ROOT}/NAMESPACE"
	DEPENDS createSkeleton
)

add_custom_target(
	compileAttributeRcpp
  COMMAND ${CMAKE_COMMAND} -E env "MAKE=${DEVTOOLS_ROOT}/bin/make -j${NCORES}" 
	${R_EXEC} -e "library(Rcpp);Rcpp::compileAttributes(pkgdir='.',TRUE)" #${SINK}
	WORKING_DIRECTORY ${PKG_BUILD_ROOT}
	DEPENDS refreshTimestamps
	VERBATIM
)

# build the lib and the documentation
add_custom_target(roxygenExport
	COMMAND ${CMAKE_COMMAND} -E env "MAKE=${DEVTOOLS_ROOT}/bin/make -j${NCORES}" 
	"PATH=${R_LOCATION_HINT}/bin:$ENV{PATH}"
	${R_EXEC} -e "library(roxygen2);roxygen2::roxygenize()" #${SINK}
	WORKING_DIRECTORY ${PKG_BUILD_ROOT}
	DEPENDS compileAttributeRcpp 
	VERBATIM
)

# build the package
add_custom_target(installPackage ALL
	COMMAND ${CMAKE_COMMAND} -E env "MAKE=${DEVTOOLS_ROOT}/bin/make -j${NCORES}" 
	"PATH=${R_LOCATION_HINT}/bin:$ENV{PATH}"
	${R_EXEC} -e "library(devtools);devtools::install(build=FALSE, dependencies=FALSE)" #${SINK}
	WORKING_DIRECTORY ${PKG_BUILD_ROOT}
	DEPENDS roxygenExport
	VERBATIM
)