cmake_minimum_required(VERSION 3.13)
project("opencl sample" C CXX)

set(TARGET main)

add_executable(${TARGET} src/opencl_sample.cpp)

target_include_directories(${TARGET} PUBLIC
    /usr/local/cuda/include 
)

if (BUILD_ANDROID)
target_link_libraries(${TARGET} PUBLIC
    deps/opencl/libOpenCL.so
)
else()
target_link_libraries(${TARGET} PUBLIC
    /usr/local/cuda/lib64/libOpenCL.so
)
endif()