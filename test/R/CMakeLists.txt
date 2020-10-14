
cmake_minimum_required (VERSION 3.13)

set(PACKAGE_DIR "${CMAKE_BINARY_DIR}/R/package/build")

message(DEBUG "looking for cfp build folder location in ${PACKAGE_DIR}")

macro(add_r_test)

    set(options)
	set(oneValueArgs FILE)
	set(multiValueArgs)

	cmake_parse_arguments(
		ADD_R_TEST "${options}"
		"${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	string(CONCAT CMD
		"res <- as.data.frame(devtools::test(filter='${ADD_R_TEST_FILE}'))\;"
        "quit(status = (sum(res$failed) + sum(res$error) + sum(res$skipped) != 0))"
	)

	add_test(NAME cfp.r.test.${ADD_R_TEST_FILE}
		COMMAND ${R_EXEC} -e ${CMD}
		WORKING_DIRECTORY ${PACKAGE_DIR}/cfp
	)

endmacro()

add_r_test(FILE rolling	)
add_r_test(FILE cfp   	)
add_r_test(FILE ewm   	)
add_r_test(FILE fft   	)
add_r_test(FILE eigen   )
add_r_test(FILE signals )