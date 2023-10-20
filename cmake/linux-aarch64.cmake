# Toolchain for cross-compiling to Linux-aarch64 on a Linux-x86-64 host.
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR aarch64)

# # Set the target architecture
set(CMAKE_ANDROID_ARCH_ABI arm64-v8a)

# # Set the desired STL type
# set(CMAKE_ANDROID_STL_TYPE c++_shared)

# Set the target platform version
# set(CMAKE_ANDROID_NDK_PLATFORM android-21)

SET(CMAKE_C_COMPILER /home/joechu/toolchain/android-ndk-r25c/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android21-clang)
SET(CMAKE_CXX_COMPILER /home/joechu/toolchain/android-ndk-r25c/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android21-clang++)
