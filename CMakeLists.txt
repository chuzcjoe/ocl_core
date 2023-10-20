cmake_minimum_required(VERSION 3.0)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/linux-aarch64.cmake)

project(opencl)
set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_definitions("-DCL_TARGET_OPENCL_VERSION=200")

add_executable(test main.cpp)

target_link_libraries(test OpenCL)