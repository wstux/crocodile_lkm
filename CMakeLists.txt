cmake_minimum_required (VERSION 3.10)

################################################################################
# Project definition
################################################################################

project(crok_lkm
        VERSION 0.0.1
        LANGUAGES CXX)

################################################################################
# Init cmake modules path
################################################################################

set(COMMON_CMAKE_DIR cmake)
set(COMMON_MAIN_CMAKE ${COMMON_CMAKE_DIR}/main.cmake)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/${COMMON_CMAKE_DIR} ${CMAKE_MODULE_PATH})

include(${COMMON_MAIN_CMAKE} PUBLIC)

################################################################################
# Include source code
################################################################################

add_subdirectory(libs/progopts)
add_subdirectory(drivers/crok)
add_subdirectory(src/crok_ioctl)
add_subdirectory(tests)

