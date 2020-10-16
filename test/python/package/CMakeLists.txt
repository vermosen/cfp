cmake_minimum_required (VERSION 3.13)

add_test(
	NAME cfp.python.package.import
	COMMAND ${PYTHON_EXECUTABLE} -c "from cfp import *"
	WORKING_DIRECTORY ${TEST_FILE_DIR}
)

set_tests_properties(cfp.python.package.import
    PROPERTIES ENVIRONMENT "PYTHONPATH=${INSTALL_LIB_DIR}/python")

add_test(
	NAME cfp.python.package.import.fails
	COMMAND ${PYTHON_EXECUTABLE} -c "from cfppp import *"
	WORKING_DIRECTORY ${TEST_FILE_DIR}
)

set_tests_properties(cfp.python.package.import.fails
    PROPERTIES ENVIRONMENT "PYTHONPATH=${INSTALL_LIB_DIR}/python" WILL_FAIL TRUE)

add_test(
	NAME cfp.python.package.check.version
	COMMAND ${PYTHON_EXECUTABLE} -c "from cfp import *"
	WORKING_DIRECTORY ${TEST_FILE_DIR}
)

set_tests_properties(cfp.python.package.check.version
    PROPERTIES ENVIRONMENT "PYTHONPATH=${INSTALL_LIB_DIR}/python" DEPENDS cfp.python.package.import)
