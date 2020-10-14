cmake_minimum_required (VERSION 3.13)

# find sphinx
find_program(SPHINX_EXECUTABLE NAMES sphinx-build
    HINTS ${PYTHON_LOCATION_HINT}/bin
    PATH_SUFFIXES bin
    DOC "Sphinx documentation generator"
    REQUIRED
)

include(FindPackageHandleStandardArgs)
 
find_package_handle_standard_args(Sphinx DEFAULT_MSG
    SPHINX_EXECUTABLE
)

mark_as_advanced(SPHINX_EXECUTABLE)