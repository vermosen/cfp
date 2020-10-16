cmake_minimum_required (VERSION 3.13)

# sphinx
if(NOT DEFINED SPHINX_THEME)
    set(SPHINX_THEME sphinx_rtd_theme)
endif()
 
if(NOT DEFINED SPHINX_THEME_DIR)
    set(SPHINX_THEME_DIR)
endif()

# sphinx configuration 
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
    "${CMAKE_CURRENT_BINARY_DIR}/conf.py"
    @ONLY
)

file(GLOB SPHINX_DOCS "${CMAKE_CURRENT_SOURCE_DIR}/*.rst")

file(COPY ${SPHINX_DOCS}
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
)
 
add_custom_target(cfp_sphinx_docs ALL
    ${SPHINX_EXECUTABLE}
    -q -b html
    -Dbreathe_projects.cfp=${DOXYGEN_OUTPUT_DIR}/xml
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}/html"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Building HTML documentation with Sphinx"
    DEPENDS cfp_doxygen_docs _cfp
)