rm -rf build
mkdir build
cd build

# cmake .. \
# -DCMAKE_TOOLCHAIN_FILE=/home/joechu/toolchain/android-ndk-r25c/build/cmake/android.toolchain.cmake \
# -DANDROID_ABI="arm64-v8a" \
# -DANDROID_NATIVE_API_LEVEL=android-33

cmake ..

make

adb push test /data/local/tmp/ocl

adb shell /data/local/tmp/ocl/test