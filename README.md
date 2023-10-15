# opencl-vulkan-snippet

A code example for OpenCL/Vulkan programming.


### 1. OpenCL example

#### a. build

build for x86 (need libOpenCL.so)

```shell
mkdir build
cd build
cmake ..
make
```

build for Android (need Android NDK, armv8 libOpenCL.so)

```shell
mkdir build-android
cd build-android
cmake -DCMAKE_TOOLCHAIN_FILE=$NDK_ROOT/build/cmake/android.toolchain.cmake \
-DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-23 -DCMAKE_C_FLAGS=-march=armv8.4a+dotprod ..
make
```

#### b. run

A simple code in `kernel/kernel.cl` (multiply each element in one array with 2).

```shell
./main ../kernel/kernel.cl
```
