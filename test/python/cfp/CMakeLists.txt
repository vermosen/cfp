cmake_minimum_required (VERSION 3.13)

add_test(
	NAME cfp.python.cfp.print_param
	COMMAND ${PYTHON_EXECUTABLE} -m unittest -v test.testCfp.test_print_params
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set_tests_properties(cfp.python.cfp.print_param
    PROPERTIES ENVIRONMENT "PYTHONPATH=${INSTALL_LIB_DIR}/python"
	DEPENDS cfp.python.package.import
)

add_test(
	NAME cfp.python.cfp.simulate
	COMMAND ${PYTHON_EXECUTABLE} -m unittest -v test.testCfp.test_simulate
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set_tests_properties(cfp.python.cfp.simulate
    PROPERTIES ENVIRONMENT "PYTHONPATH=${INSTALL_LIB_DIR}/python"
	DEPENDS cfp.python.package.import
)

add_test(
	NAME cfp.python.cfp.filter
	COMMAND ${PYTHON_EXECUTABLE} -m unittest -v test.testCfp.test_filter
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set_tests_properties(cfp.python.cfp.filter
    PROPERTIES ENVIRONMENT "PYTHONPATH=${INSTALL_LIB_DIR}/python"
	DEPENDS cfp.python.package.import
)

add_test(
	NAME cfp.python.cfp.smoother
	COMMAND ${PYTHON_EXECUTABLE} -m unittest -v test.testCfp.test_smoother
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set_tests_properties(cfp.python.cfp.smoother
    PROPERTIES ENVIRONMENT "PYTHONPATH=${INSTALL_LIB_DIR}/python"
	DEPENDS cfp.python.package.import
)

add_test(
	NAME cfp.python.cfp.emax
	COMMAND ${PYTHON_EXECUTABLE} -m unittest -v test.testCfp.test_emax
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set_tests_properties(cfp.python.cfp.emax
    PROPERTIES ENVIRONMENT "PYTHONPATH=${INSTALL_LIB_DIR}/python"
	DEPENDS cfp.python.package.import
)

add_test(
	NAME cfp.python.cfp.emax.debug
	COMMAND ${PYTHON_EXECUTABLE} -m unittest -v test.testCfp.test_emax_debug
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set_tests_properties(cfp.python.cfp.emax.debug
    PROPERTIES ENVIRONMENT "PYTHONPATH=${INSTALL_LIB_DIR}/python"
	DEPENDS cfp.python.package.import
)

add_test(
	NAME cfp.python.cfp.predict
	COMMAND ${PYTHON_EXECUTABLE} -m unittest -v test.testCfp.test_predict
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set_tests_properties(cfp.python.cfp.predict
    PROPERTIES ENVIRONMENT "PYTHONPATH=${INSTALL_LIB_DIR}/python"
	DEPENDS cfp.python.package.import
)

