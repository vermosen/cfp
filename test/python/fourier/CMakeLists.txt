cmake_minimum_required (VERSION 3.13)

add_test(
	NAME cfp.python.fft.convolve
	COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/convolve/main.py
	WORKING_DIRECTORY ${TEST_FILE_DIR}
)

set_tests_properties(cfp.python.fft.convolve
    PROPERTIES ENVIRONMENT "PYTHONPATH=${INSTALL_LIB_DIR}/python"
	DEPENDS cfp.python.package.import
)

add_test(
	NAME cfp.python.fft.fft
	COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/fft/main.py
	WORKING_DIRECTORY ${TEST_FILE_DIR}
)

set_tests_properties(cfp.python.fft.fft
    PROPERTIES ENVIRONMENT "PYTHONPATH=${INSTALL_LIB_DIR}/python"
	DEPENDS cfp.python.package.import
)

#########################
#       new tests       #
#########################
add_test(
	NAME cfp.python.fft.flag
	COMMAND ${PYTHON_EXECUTABLE} -m unittest -v test.testFft.test_flag
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set_tests_properties(cfp.python.fft.flag
    PROPERTIES ENVIRONMENT "PYTHONPATH=${INSTALL_LIB_DIR}/python"
	DEPENDS cfp.python.package.import
)