/*
 ******************************************************************************
 *       Filename:    GPUTask.cpp
 *    Description:    cpp file
 *       
 *        Version:    1.0
 *        Created:    2018-07-21
 *         Author:    chencheng
 *
 *        History:    initial draft
 ******************************************************************************
 */

#include "GPUTask.h"

#include <cstring>
#include <fstream>
#include <sstream>

#define VENDOR_NAME "NVIDIA"


GPUTask::GPUTask()
{
    /* platform */
    cl_uint             platformNum_ = 0;
    cl_platform_id *    platforms_ = NULL;
    cl_uint             platformIdx_ = 0;

    /* device */
    cl_device_id **     devices_ = NULL;
    cl_uint *           deviceNum_ = NULL;
    cl_uint *           deviceIdx_ = NULL;

    /* context_ and command queue */
    cl_context          context_ = NULL;
    cl_command_queue    commandQueue_ = NULL;    
}


GPUTask::~GPUTask()
{

}


cl_int GPUTask::SetupOpenCL()
{
    cl_int status = CL_SUCCESS;
    
    /* get all platform IDs */
    status = clGetPlatformIDs(0, NULL, &(platformNum_));/* get platform number */
    if (CL_SUCCESS != status )
    {
        printf("Error: clGetPlatformIDs %d.\n", status);
        return status;
    }
    
    platforms_ = (cl_platform_id *)malloc(platformNum_ * sizeof(cl_platform_id)); /*malloc */
    devices_   = (cl_device_id **)malloc(platformNum_ * sizeof(cl_device_id *));
    deviceNum_ = (cl_uint *)malloc(platformNum_ * sizeof(cl_uint));
    deviceIdx_ = (cl_uint *)malloc(platformNum_ * sizeof(cl_uint));

    status = clGetPlatformIDs(platformNum_, platforms_, NULL);/* get platform IDs */
    if (CL_SUCCESS != status)
    {
        printf("Error: clGetPlatformIDs %d.\n", status);
        return status;
    }

    /* get all devices_ id for all platforms_ */
    for (cl_uint i = 0; i < platformNum_; i++)
    {
        status = clGetDeviceIDs(platforms_[i], CL_DEVICE_TYPE_ALL, 0, NULL, &(deviceNum_[i])); /* get device number */
        if (CL_SUCCESS != status) {
            printf("Error: clGetDeviceIDs %d.\n", status);
            return status;
        }

        devices_[i] = (cl_device_id *)malloc(deviceNum_[i] * sizeof(cl_device_id)); /* malloc */

        status = clGetDeviceIDs(platforms_[i], CL_DEVICE_TYPE_ALL, deviceNum_[i], devices_[i], NULL); /* get device IDs */
        if(CL_SUCCESS != status) {
            printf("Error: clGetDeviceIDs %d.\n", status);
            return status;
        }
    }

    /* choose a platform */
    if (1 == platformNum_) /* if only one platform, choose it */
    {
        platformIdx_ = 0;
    }
    else /* if many platform, choose the vendor */
    {
        for (uint i = 0; i < platformNum_; i++)
        {
            char buf[100];
            status = clGetPlatformInfo( platforms_[i],
                                           CL_PLATFORM_VENDOR,
                                           100,
                                        buf,
                                        NULL);
            if (CL_SUCCESS != status) {
                printf("Error: clGetPlatformInfo %d.\n", status);
                return status;
            }
            
            if (NULL != strstr(buf, VENDOR_NAME)) {
                platformIdx_ = i;
                break;
            }                    
        }
    }

    /* choose a device */
    for (uint i = 0; i < deviceNum_[platformIdx_]; i++)
    {
        cl_device_type deviceType;
        status = clGetDeviceInfo(devices_[platformIdx_][i],
                                 CL_DEVICE_TYPE,
                                 sizeof(cl_device_type),
                                 &deviceType,
                                 NULL
                                );
        if (CL_DEVICE_TYPE_GPU == deviceType) {
            deviceIdx_[platformIdx_] = i;
            break;
        }
    }
    
    /* create a context_ */
    context_ = clCreateContext(NULL,
                              deviceNum_[platformIdx_],
                              devices_[platformIdx_],
                              NULL,
                              NULL,
                              &status);
    if (CL_SUCCESS != status) {
        printf("Error: clCreateContext %d.\n", status);
        return status;
    }
    
    /* create a command queue */
    commandQueue_ = clCreateCommandQueue(context_,    /* or using clCreatecommandQueue_WithProperties */
                                            devices_[platformIdx_][deviceIdx_[platformIdx_]],
                                            CL_QUEUE_PROFILING_ENABLE,
                                            &status);
    if (CL_SUCCESS != status) {
        printf("Error: clCreateCommandQueue %d.\n", status);
        return status;
    }
    
    /* printf info */
    printf("OpenCL environment Setup!\n");
    printf("# %d platforms found.\n", platformNum_);
    printf("# %d devices found on platform-%d.\n", deviceNum_[platformIdx_], platformIdx_);
    printf("\n");
    
    return status;
}


cl_int GPUTask::CleanOpenCL()
{
    cl_int status = CL_SUCCESS;
    
    if (NULL != platforms_) {
        free(platforms_);
    }
    
    if (NULL != devices_) {
        for (int i = 0; i < platformNum_; i++) {
            free(devices_[i]);
        }
        free(devices_);
    }
    
    if (NULL != deviceNum_) {
        free(deviceNum_);
    }
    
    if (NULL != deviceIdx_) {
        free(deviceIdx_);
    }
    
    if (NULL != context_) {
        status = clReleaseContext(context_);
        if (CL_SUCCESS != status) {
            printf("Error: clReleasecontext_ %d.\n", status);
            return status;
        }
    }
    
    if (NULL != commandQueue_) {
        status = clReleaseCommandQueue(commandQueue_);
        if (CL_SUCCESS != status) {
            printf("Error: clReleaseCommandQueue %d.\n", status);
            return status;
        }
    }
    
    printf("OpenCL environment Released!\n");
    return status;
}


std::string GPUTask::GetDirName(const char * filepath)
{
    std::string dir_str;
    std::string path_str = std::string(filepath);
    int i = path_str.size() - 1;
    for (; i >= 0; i-- ) {
        if (path_str[i] == '/') {
            break;
        }
    }
    if (i < 0) {
        dir_str = std::string("./");
    }
    else {
        dir_str = path_str.substr(0, i+1);
    }

    return  std::string("-I") + dir_str;
}


cl_int GPUTask::CreateProgram(const char * filepath)
{
    cl_int status = CL_SUCCESS;

    std::ifstream kernelFile(filepath, std::ios::in);
    if (!kernelFile.is_open()) {
        std::cerr << "Failed to open " << filepath << std::endl;
    }

    std::ostringstream oss;
    oss << kernelFile.rdbuf();

    std::string srcStdStr = oss.str();
    const char * srcStr   = srcStdStr.c_str();

    //printf("%s\n", srcStr);    

    program_ = clCreateProgramWithSource(context_,
                                       1,
                                       &srcStr,
                                       NULL,
                                       &status
                                       );
    if (NULL == program_) {
        std::cerr << "Failed to create program: " << status << std::endl;
    }

    status = clBuildProgram(program_, 0, NULL, GetDirName(filepath).c_str(), NULL, NULL);
    if (CL_SUCCESS != status) {
        std::cerr << "Failed to build program: " << status << std::endl;
        clReleaseProgram(program_);
    }

    return status;
}
