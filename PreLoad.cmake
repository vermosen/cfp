cmake_minimum_required(VERSION 3.12)

# default compiler profile
if(NOT DEFINED CMAKE_PROFILE)
    set(CMAKE_PROFILE gcc84)
endif()

string(TOUPPER ${CMAKE_PROFILE} CMAKE_PROFILE_UC)

if(CMAKE_PROFILE_UC STREQUAL "GCC84")

        set(DEVTOOLS_ROOT "/opt/gcc84")

	    set(CXX_DIALECT          "17"						    CACHE INTERNAL "" FORCE)
        set(LINKER_EXTRA_FLAG    "-fuse-ld=gold"                CACHE INTERNAL "" FORCE)
        set(R_LOCATION_HINT      "/opt/r40"                     CACHE INTERNAL "" FORCE)

        set(CONAN_FLAGS ${CONAN_FLAGS} "cppstd=${CXX_DIALECT}"      )
    
        set(CONAN_EXTRA_SETTINGS ${CONAN_FLAGS}                 CACHE INTERNAL "" FORCE)

else()
	message(FATAL_ERROR "Error: ${CMAKE_PROFILE} - Unknown configuration passed. Aborting ... ")
endif()
