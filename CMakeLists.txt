cmake_minimum_required(VERSION 3.0)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/linux-aarch64.cmake)

project(oclcore)
set(CMAKE_CXX_STANDARD 11)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/opencv.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/CImg)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/utils)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/oclWrapper)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/Qualcomm)

file(GLOB SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/oclWrapper/*.cpp)

add_library(ocl SHARED ${SRC_LIST})
LINK_OPENCV()
target_link_libraries(ocl PUBLIC ${OPENCV_LINK_LIBS})

add_definitions("-DCL_TARGET_OPENCL_VERSION=200")

add_executable(test main.cpp)

target_link_libraries(test PRIVATE OpenCL ocl)