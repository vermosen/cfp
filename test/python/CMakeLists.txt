cmake_minimum_required (VERSION 3.13)

set(PYTHON_EXECUTABLE ${PYTHON_LOCATION_HINT}/bin/python)
message(DEBUG "python test environment set to ${PYTHON_EXECUTABLE}")

set(PYTHONPATH_TESTSUITE "${INSTALL_LIB_DIR}/python")
message(DEBUG "test pythonpath set to ${PYTHONPATH_TESTSUITE}")

add_subdirectory(cfp)
add_subdirectory(package)