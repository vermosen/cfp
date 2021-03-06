# This code sets the following variables
# R_EXEC      - path to R executable
# R_LIBS_USER - path to directory of user's R packages (defined only if R is found)
#
# It also defines the following help functions if R is found:
# FIND_R_PACKAGE(package)            - sets R_<PACKAGE> to ON if package is installed
# INSTALL_R_PACKAGE(package)         - installs package in ${R_LIBS_USER}
# FIND_OR_INSTALL_R_PACKAGE(package) - finds package and installs it, if not found
message(DEBUG "looking for R executable at ${R_LOCATION_HINT}/bin ...")

include(FindPackageHandleStandardArgs)

# note: for some reason, the R binary in r-cran always preceed the hint/path without the overrides
find_program(R_EXEC R HINTS ${R_LOCATION_HINT}/bin
	NO_DEFAULT_PATH
    NO_PACKAGE_ROOT_PATH
    NO_CMAKE_PATH
    NO_CMAKE_ENVIRONMENT_PATH
    NO_SYSTEM_ENVIRONMENT_PATH
    NO_CMAKE_SYSTEM_PATH
)

if (R_EXEC)
	message(STATUS "found R executable at ${R_EXEC}")
	# R_LIBS_USER is always defined within R, even if it is not explicitly set by the user
	execute_process(COMMAND "${R_EXEC}" "--slave" "-e"
		"print(Sys.getenv(\"R_LIBS_USER\"))"
		OUTPUT_VARIABLE _rlibsuser
		OUTPUT_STRIP_TRAILING_WHITESPACE)
	string(REGEX REPLACE "[ ]*(R_LIBS_USER)[ ]*\n\"(.*)\"" "\\2"
		R_LIBS_USER ${_rlibsuser})

	function(find_r_package package)
		string(TOUPPER ${package} package_upper)
		if(NOT R_${package_upper})
			if(ARGC GREATER 1 AND ARGV1 STREQUAL "REQUIRED")
				set(${package}_FIND_REQUIRED TRUE)
			endif()
			execute_process(COMMAND "${R_EXEC}" "--slave" "-e"
				"library('${package}')"
				RESULT_VARIABLE _${package}_status
				ERROR_QUIET OUTPUT_QUIET)
			if(NOT _${package}_status)
				set(R_${package_upper} TRUE CACHE BOOL
					"Whether the R package ${package} is installed")
			endif(NOT _${package}_status)
		endif(NOT R_${package_upper})
		find_package_handle_standard_args(R_${package} DEFAULT_MSG R_${package_upper})
	endfunction(find_r_package)

	function(install_r_package package)
		message(STATUS "Installing R package ${package}...")
		execute_process(COMMAND "${R_EXEC}" "--slave" "-e"
			"install.packages('${package}','${R_LIBS_USER}','http://cran.r-project.org')"
			ERROR_QUIET OUTPUT_QUIET)
		message(STATUS "R package ${package} has been installed in ${R_LIBS_USER}")
	endfunction(install_r_package)

	function(find_or_install_r_package package)
		find_r_package(${package})
		string(TOUPPER ${package} package_upper)
		if(NOT R_${package_upper})
			install_r_package(${package})
		endif()
	endfunction(find_or_install_r_package)

	# find the Rscript command
	message(DEBUG "looking for Rscript launcher at ${R_LOCATION_HINT}/bin ...")

	find_program(R_SCRIPT Rscript PATH ${R_LOCATION_HINT}/bin)

	if(R_SCRIPT)
		message(STATUS "found R script launcher")
	else()
		message(FATAL_ERROR "cannot find Rscript launcher")
	endif(R_SCRIPT)

else()
	message(FATAL_ERROR "cannot find R executable")
endif(R_EXEC)

find_package_handle_standard_args(R DEFAULT_MSG R_EXEC)
