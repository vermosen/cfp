cmake_minimum_required (VERSION 3.13)

set(PYTHONPATH_TESTSUITE "${CMAKE_INSTALL_PREFIX}/${INSTALL_LIBRARY_SUFFIX}/python")
message(DEBUG "PYTHONPATH_TESTSUITE set to ${PYTHONPATH_TESTSUITE}")

#add_subdirectory(cfp)
add_subdirectory(debug)
add_subdirectory(package)