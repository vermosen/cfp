cmake_minimum_required (VERSION 3.13)

add_test(
	NAME cfp.python.package.import
	COMMAND ${PYTHON_EXECUTABLE} -c "from cfp import *"
	WORKING_DIRECTORY ${TEST_FILE_DIR}
)

set_tests_properties(cfp.python.package.import
	PROPERTIES ENVIRONMENT "PYTHONPATH=${PYTHONPATH_TESTSUITE}"
)