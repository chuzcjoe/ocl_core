macro(LINK_OPENCV)
    set(opencv_dir ${CMAKE_CURRENT_SOURCE_DIR}/opencv)
    include_directories(${opencv_dir}/include)

    set(OPENCV_STATIC_LIBS ${opencv_dir}/libs/libcpufeatures.a
                                        ${opencv_dir}/libs/liblibjpeg-turbo.a
                                        ${opencv_dir}/libs/liblibtiff.a
                                        ${opencv_dir}/libs/libquirc.a
                                        ${opencv_dir}/libs//liblibpng.a
                                        ${opencv_dir}/libs//liblibwebp.a
                                        ${opencv_dir}/libs//libzlib.a
                                        ${opencv_dir}/libs//libopencv_world.a
                                        ${opencv_dir}/libs//libade.a
                                        ${opencv_dir}/libs//libtegra_hal.a)
    
    list(APPEND OPENCV_LINK_LIBS -Wl,--start-group ${OPENCV_STATIC_LIBS} -Wl,--end-group)
endmacro()
