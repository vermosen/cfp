cmake_minimum_required (VERSION 3.13)

SET(TARGET_NAME cfp)

setup_component(TARGET ${TARGET_NAME})

file(GLOB_RECURSE TARGET_HEADERS CONFIGURE_DEPENDS "*.h")

add_library(${TARGET_NAME} INTERFACE)

target_include_directories(
    ${TARGET_NAME} 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}  
)

target_link_libraries(${TARGET_NAME} 
    INTERFACE 	
    CONAN_PKG::eigen
)

install_library(NAME ${TARGET_NAME})
