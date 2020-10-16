cmake_minimum_required (VERSION 3.13)

SET(TARGET_NAME _cfp)

find_package(pybind11 REQUIRED 
	HINTS ${CONAN_PYBIND11_ROOT}/share/cmake/pybind11
)

import_python(HINT ${PYTHON_LOCATION_HINT}/bin)

pybind11_add_module(${TARGET_NAME} MODULE 
	module.cpp
	cfp.h cfp.cpp
)

target_link_libraries(${TARGET_NAME} PUBLIC cfp)

install(
	TARGETS ${TARGET_NAME}
	LIBRARY DESTINATION ${INSTALL_LIB_DIR}/python/cfp
)

add_custom_command(TARGET _cfp
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:_cfp> ${PYTHON_DOCS_TARGET}/cfp
	DEPENDS copy_python_files
)