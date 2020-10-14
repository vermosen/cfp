cmake_minimum_required (VERSION 3.13)

set(CURRENT_DATE "")
get_current_date(
	OUT CURRENT_DATE
	FORMAT "+%Y-%m-%d"
)

# TODO
#configure_file(
#	${CMAKE_CURRENT_SOURCE_DIR}/cfp/setup.py.in
#	${PROJECT_BIN_PATH}/../setup.py
#)

# in order to build the documentation in cmake we must gather 
# all the package ressources in the same folder
set(PYTHON_DOCS_TARGET ${CMAKE_BINARY_DIR}/docs/package)

file(MAKE_DIRECTORY ${PYTHON_DOCS_TARGET}/cfp)

add_custom_target(copy_python_files ALL 
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/cfp 
	${PYTHON_DOCS_TARGET}/cfp
)

install(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cfp
	DESTINATION ${INSTALL_LIB_DIR}/python
)

add_subdirectory(src)
