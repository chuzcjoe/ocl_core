cmake_minimum_required(VERSION 3.0)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/linux-aarch64.cmake)

project(oclcore)
set(CMAKE_CXX_STANDARD 11)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/opencv.cmake)

set(root_dir ${CMAKE_CURRENT_SOURCE_DIR}/../)

include_directories(${root_dir}/include)
include_directories(${root_dir}/include/CImg)
include_directories(${root_dir}/utils)
include_directories(${root_dir}/src/oclWrapper)
link_directories(${root_dir}/lib/Qualcomm)

file(GLOB SRC_LIST ${root_dir}/utils/*.cpp
                    ${root_dir}/src/oclWrapper/*.cpp)

add_library(ocl SHARED ${SRC_LIST})
LINK_OPENCV()
target_link_libraries(ocl PUBLIC ${OPENCV_LINK_LIBS})

add_definitions("-DCL_TARGET_OPENCL_VERSION=200")

set(TEST_FILES test_vecAdd
                test_opencv
                test_gaussianFilter)

foreach(program ${TEST_FILES})
    add_executable(${program} ${program}.cpp)
    target_link_libraries(${program} OpenCL ocl log)
endforeach(program)