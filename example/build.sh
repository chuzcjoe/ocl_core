phone_dir="/data/local/tmp/ocl"
test_module="test_gaussianFilter"

rm -rf build
mkdir build
cd build

cmake ..

make

adb push test_* ${phone_dir}
adb push libocl.so ${phone_dir}
adb push ../img ${phone_dir}
adb push ../../kernels ${phone_dir}

adb shell mkdir -p ${phone_dir}/result

ADB_RUN_CMD_STR="cd ${phone_dir}/ && LD_LIBRARY_PATH=${phone_dir} ${phone_dir}/${test_module}"

adb shell ${ADB_RUN_CMD_STR}

adb pull ${phone_dir}/result ./