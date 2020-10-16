
cmake_minimum_required (VERSION 3.13)

set(PACKAGE_DIR "${CMAKE_BINARY_DIR}/R/package/build")

message(DEBUG "looking for cfp build folder location in ${PACKAGE_DIR}")

add_r_test(
	FILE cfp 
	MODULE cfp 
	WORKING_DIRECTORY ${PACKAGE_DIR}/cfp
)