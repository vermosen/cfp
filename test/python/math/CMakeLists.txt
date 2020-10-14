cmake_minimum_required (VERSION 3.13)

add_test(
	NAME cfp.python.math.add_array
	COMMAND ${PYTHON_EXECUTABLE} -m unittest -v test.testMath.test_add_array
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set_tests_properties(cfp.python.math.add_array
    PROPERTIES ENVIRONMENT "PYTHONPATH=${INSTALL_LIB_DIR}/python"
	DEPENDS cfp.python.package.import
)

add_test(
	NAME cfp.python.math.sum_real
	COMMAND ${PYTHON_EXECUTABLE} -m unittest -v test.testMath.test_sum_real
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set_tests_properties(cfp.python.math.sum_real
    PROPERTIES ENVIRONMENT "PYTHONPATH=${INSTALL_LIB_DIR}/python"
	DEPENDS cfp.python.package.import
)

add_test(
	NAME cfp.python.math.sum_complex
	COMMAND ${PYTHON_EXECUTABLE} -m unittest -v test.testMath.test_sum_complex
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set_tests_properties(cfp.python.math.sum_complex
    PROPERTIES ENVIRONMENT "PYTHONPATH=${INSTALL_LIB_DIR}/python"
	DEPENDS cfp.python.package.import
)