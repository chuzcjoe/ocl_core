phone_dir="/data/local/tmp/ocl"

rm -rf build
mkdir build
cd build

cmake ..

make

adb push test ${phone_dir}
adb push libocl.so ${phone_dir}
adb push ../kernels ${phone_dir}

ADB_RUN_CMD_STR="cd ${phone_dir}/ && LD_LIBRARY_PATH=${phone_dir} ${phone_dir}/test"

adb shell ${ADB_RUN_CMD_STR}