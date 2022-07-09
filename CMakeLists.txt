cmake_minimum_required(VERSION 3.9)
project(hetker C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -fopenmp")

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})

#file(DOWNLOAD "https://raw.githubusercontent.com/vector-of-bool/cmrc/master/CMakeRC.cmake"
#        "${CMAKE_BINARY_DIR}/CMakeRC.cmake")
#include("${CMAKE_BINARY_DIR}/CMakeRC.cmake")
include(CMakeRC.cmake)

cmrc_add_resource_library(
        hetker-resources

        ALIAS app1::rc
        NAMESPACE app1

        kernels/image.cl
        kernels/list.cl
        kernels/matrix.cl
        kernels/text.cl
)

add_executable(hetker main.c init.c hetker_lib.h hetker.h util.c util.h strings.c strings.h)

target_include_directories (hetker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (hetker ${OpenCL_LIBRARY} app1::rc)
