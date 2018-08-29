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


GPUTask::GPUTask()
{
    /* platform */
    cl_uint             platformNum = 0;
    cl_platform_id *    platforms = nullptr;
    cl_uint             platformIdx = 0;

    /* device */
    cl_device_id **     devices = nullptr;
    cl_uint *           deviceNum = nullptr;
    cl_uint *           deviceIdx = nullptr;

    /* context and command queue */
    cl_context          context = nullptr;
    cl_command_queue    commandQueue = nullptr;    
}


GPUTask::~GPUTask()
{

}


cl_int GPUTask::SetupOpenCL()
{
    cl_int status = CL_SUCCESS;
    
    /* get all platform IDs */
    status = clGetPlatformIDs(0, NULL, &(platformNum));/* get platform number */
    if (CL_SUCCESS != status )
    {
        printf("Error: clGetPlatformIDs %d.\n", status);
        return status;
    }
    
    platforms = (cl_platform_id *)malloc(platformNum * sizeof(cl_platform_id)); /*malloc */
    devices   = (cl_device_id **)malloc(platformNum * sizeof(cl_device_id *));
    deviceNum = (cl_uint *)malloc(platformNum * sizeof(cl_uint));
    deviceIdx = (cl_uint *)malloc(platformNum * sizeof(cl_uint));

    status = clGetPlatformIDs(platformNum, platforms, NULL);/* get platform IDs */
    if (CL_SUCCESS != status)
    {
        printf("Error: clGetPlatformIDs %d.\n", status);
        return status;
    }

    /* get all devices id for all platforms */
    for (cl_uint i = 0; i < platformNum; i++)
    {
        status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &(deviceNum[i])); /* get device number */
        if (CL_SUCCESS != status) {
            printf("Error: clGetDeviceIDs %d.\n", status);
            return status;
        }

        devices[i] = (cl_device_id *)malloc(deviceNum[i] * sizeof(cl_device_id)); /* malloc */

        status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, deviceNum[i], devices[i], NULL); /* get device IDs */
        if(CL_SUCCESS != status) {
            printf("Error: clGetDeviceIDs %d.\n", status);
            return status;
        }
    }

    /* choose a platform */
    if (1 == platformNum) /* if only one platform, choose it */
    {
        platformIdx = 0;
    }
    else /* if many platform, choose the vendor */
    {
        for (uint i = 0; i < platformNum; i++)
        {
            char buf[100];
            status = clGetPlatformInfo( platforms[i],
                                           CL_PLATFORM_VENDOR,
                                           100,
                                        buf,
                                        NULL);
            if (CL_SUCCESS != status) {
                printf("Error: clGetPlatformInfo %d.\n", status);
                return status;
            }
            
            if (NULL != strstr(buf, VENDOR_NAME)) {
                platformIdx = i;
                break;
            }                    
        }
    }

    /* choose a device */
    for (uint i = 0; i < deviceNum[platformIdx]; i++)
    {
        cl_device_type deviceType;
        status = clGetDeviceInfo(devices[platformIdx][i],
                                 CL_DEVICE_TYPE,
                                 sizeof(cl_device_type),
                                 &deviceType,
                                 NULL
                                );
        if (CL_DEVICE_TYPE_GPU == deviceType) {
            deviceIdx[platformIdx] = i;
            break;
        }
    }
    
    /* create a context */
    context = clCreateContext(NULL,
                              deviceNum[platformIdx],
                              devices[platformIdx],
                              NULL,
                              NULL,
                              &status);
    if (CL_SUCCESS != status) {
        printf("Error: clCreateContext %d.\n", status);
        return status;
    }
    
    /* create a command queue */
    commandQueue = clCreateCommandQueue(context,    /* or using clCreateCommandQueueWithProperties */
                                            devices[platformIdx][deviceIdx[platformIdx]],
                                            CL_QUEUE_PROFILING_ENABLE,
                                            &status);
    if (CL_SUCCESS != status) {
        printf("Error: clCreateCommandQueue %d.\n", status);
        return status;
    }
    
    /* printf info */
    printf("OpenCL environment Setup!\n");
    printf("# %d platforms found.\n", platformNum);
    printf("# %d devices found on platform-%d.\n", deviceNum[platformIdx], platformIdx);
    printf("\n");
    
    return status;
}


cl_int GPUTask::CleanOpenCL()
{
    cl_int status = CL_SUCCESS;
    
    if (NULL != platforms) {
        free(platforms);
    }
    
    if (NULL != devices) {
        for (int i = 0; i < platformNum; i++) {
            free(devices[i]);
        }
        free(devices);
    }
    
    if (NULL != deviceNum) {
        free(deviceNum);
    }
    
    if (NULL != deviceIdx) {
        free(deviceIdx);
    }
    
    if (NULL != context) {
        status = clReleaseContext(context);
        if (CL_SUCCESS != status) {
            printf("Error: clReleaseContext %d.\n", status);
            return status;
        }
    }
    
    if (NULL != commandQueue) {
        status = clReleaseCommandQueue(commandQueue);
        if (CL_SUCCESS != status) {
            printf("Error: clReleaseCommandQueue %d.\n", status);
            return status;
        }
    }
    
    printf("OpenCL environment Released!\n");
    return status;
}