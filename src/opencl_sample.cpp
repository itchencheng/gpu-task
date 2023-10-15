/*
 ******************************************************************************
 *       Filename:    main.cpp
 *    Description:    cpp file
 *       
 *        Version:    1.0
 *        Created:    2018-07-21
 *         Revise:    2023-10-14
 *         Author:    chencheng
 *
 *        History:    initial draft
 ******************************************************************************
 */
#include <iostream>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <vector>
#include <CL/cl.h>


#define PRINTE printf
#define PRINTD printf

struct CLEnvironment {
    cl_platform_id platform;
    cl_device_id device;
	cl_context context;
	cl_command_queue commandQueue;

    CLEnvironment() {
        platform = 0;
        device = 0;
        context = NULL;
        commandQueue = NULL;
    }
};

int setupOpenCL(CLEnvironment *env) {
    cl_int status = CL_SUCCESS;
    
    /* get all platform IDs */
    cl_uint num;
    status = clGetPlatformIDs(0, NULL, &num);
    if (CL_SUCCESS != status) {
        PRINTE("Error: clGetPlatformIDs %d.\n", status);
        return -1;
    }
    if (num == 0) {
        PRINTE("no platform found!\n");
        return -1;
    }

    std::vector<cl_platform_id> platforms(num);
    status = clGetPlatformIDs(num, platforms.data(), NULL);
    if (CL_SUCCESS != status) {
        PRINTE("Error: clGetPlatformIDs %d.\n", status);
        return -1;
    }
    
    // choose first platform
    cl_platform_id platform = platforms[0];

    char buf[64];
    status = clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, 64, buf, NULL);
    if (CL_SUCCESS != status) {
        PRINTE("Error: clGetPlatformInfo %d.\n", status);
        return -1;
    }

    // get gpu devices
    status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &num);
    if (CL_SUCCESS != status) {
        PRINTE("Error: clGetDeviceIDs %d.\n", status);
        return -1;
    }

    std::vector<cl_device_id> devices(num);
    status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, num, devices.data(), NULL);
    if(CL_SUCCESS != status) {
        PRINTE("Error: clGetDeviceIDs %d.\n", status);
        return -1;
    }

    // choose first device
    cl_device_id device = devices[0];
    
    /* create a context */
    cl_context context;
    context = clCreateContext(
            NULL, num, devices.data(), NULL, NULL, &status);
    if (CL_SUCCESS != status) {
        PRINTE("Error: clCreateContext %d.\n", status);
        return -1;
    }
    
    /* create a command queue */
    cl_command_queue commandQueue;
    commandQueue = clCreateCommandQueue(
            context, device, CL_QUEUE_PROFILING_ENABLE, &status);
    if (CL_SUCCESS != status) {
        PRINTE("Error: clCreateCommandQueue %d.\n", status);
        return -1;
    }
    
    /* printf info */
    printf("OpenCL environment Setup!\n");
    env->platform = platform;
    env->device = device;
    env->context = context;
    env->commandQueue = commandQueue;
    return 0;
}


int releaseOpenCL(CLEnvironment* env) {
    cl_int status = CL_SUCCESS;

    if (NULL != env->commandQueue) {
        status = clReleaseCommandQueue(env->commandQueue);
        if (CL_SUCCESS != status) {
            PRINTE("Error: clReleaseCommandQueue %d.\n", status);
            return -1;
        }
    }

    cl_uint num;
    status = clGetContextInfo(env->context,
            CL_CONTEXT_REFERENCE_COUNT, sizeof(cl_uint), &num, NULL);
    if (CL_SUCCESS == status) {
        PRINTD("context: ref count = %u\n", num);
        if (num != 1) { // seems only effective on ARM mali?
            PRINTE("context: ref count = %u\n, not 1! May have unreleased objects", num);
        }
    }

    if (NULL != env->context) {
        status = clReleaseContext(env->context);
        if (CL_SUCCESS != status) {
            PRINTE("Error: clReleaseContext %d.\n", status);
            return -1;
        }
    }
    PRINTE("OpenCL environment Released!\n");
    return 0;
}


cl_program createProgram(CLEnvironment *env, const char *filepath) {
    cl_int status = CL_SUCCESS;

    // get file content
    std::ifstream kernelFile(filepath, std::ios::in);
    if (!kernelFile.is_open()) {
        PRINTE("Error: failed to open %s\n", filepath);
        return NULL;
    }
    std::ostringstream oss;
    oss << kernelFile.rdbuf();
    std::string srcStdStr = oss.str();
    const char * srcStr   = srcStdStr.c_str();
    PRINTD("%s\n", srcStr);    

    // create program
    cl_program program;
    program = clCreateProgramWithSource(
            env->context, 1, &srcStr, NULL, NULL);
    if (NULL == program) {
        PRINTE("Error: failed to create program\n");
        return NULL;
    }

    status = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (CL_SUCCESS != status) {
        PRINTE("Error: failed to build program, status=%d\n", status);
        size_t logSize;
        status = clGetProgramBuildInfo(program, env->device,
                CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
        std::vector<char> log(logSize);
        status = clGetProgramBuildInfo(program, env->device,
                CL_PROGRAM_BUILD_LOG, logSize, log.data(), NULL);
        PRINTE("build log: %s\n", log.data());
        clReleaseProgram(program);
        return NULL;
    }
    return program;
}

cl_kernel createKernels(cl_program program, const char *kernelName) {
    cl_kernel kernel;
    cl_int status = CL_SUCCESS;
    kernel = clCreateKernel(program, kernelName, &status);
    if (CL_SUCCESS != status) {
        PRINTE("create kernel failed! status=%d\n", status);
        return NULL;
    }
    return kernel;
}

int main(int argc, char **argv) {
    CLEnvironment env;

    // setup platform/device/context/command queue
    setupOpenCL(&env);

    // create program
    std::string filepath = argv[1];
    std::cout << "filepath: " << argv[1] << std::endl;

    cl_program program = createProgram(&env, filepath.c_str());

    // create kernel
    cl_kernel kernel = createKernels(program, "kernel_test");

    // prepare buffer  
    std::vector<float> a;
    for (int i = 0; i < 10; i++) {
        a.push_back(i);
    }
    std::vector<float> b(a.size());

    cl_int status = CL_SUCCESS;
    cl_mem src = clCreateBuffer(env.context,
            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(float) * a.size(), a.data(), &status);
    cl_mem dst = clCreateBuffer(env.context,
            CL_MEM_READ_WRITE,
            sizeof(float) * a.size(), NULL, &status);

    // set arg
    status  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &src);
    status |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &dst);

    // run kernel
    size_t globalSize[1] = {10};
    status = clEnqueueNDRangeKernel(env.commandQueue,
            kernel, 1, NULL,
            globalSize, NULL,
            0, NULL, NULL);
    if (CL_SUCCESS != status) {
        PRINTE("Error: clEnqueueNDRangeKernel failed %d.\n", status);
    }

    // get result sync
    status = clEnqueueReadBuffer(env.commandQueue,
            dst, CL_TRUE, 0, 
            sizeof(float) * b.size(), b.data(),
            0, NULL, NULL);
    if (CL_SUCCESS != status) {
        PRINTE("Error: clEnqueueReadBuffer failed %d\n", status);    
    }

    // printf    
    PRINTD("result: \n");
    for (int i = 0; i < 10; i++) {
        PRINTD("%f ", b[i]);    
    }
    PRINTD("\n");


    // release
    status = clReleaseKernel(kernel);
    if (CL_SUCCESS != status) {
        PRINTE("Error: clReleaseKernel failed %d\n", status);    
    }

    status = clReleaseProgram(program);
    if (CL_SUCCESS != status) {
        PRINTE("Error: clReleaseProgram failed %d\n", status);    
    }

    clReleaseMemObject(src);
    clReleaseMemObject(dst);

    releaseOpenCL(&env);

    PRINTD("ok\n");
	return 0;
}

