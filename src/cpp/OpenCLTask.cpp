/*
 ******************************************************************************
 *       Filename:    OpenCLTask.cpp
 *    Description:    cpp file
 *       
 *        Version:    1.0
 *        Created:    2018-07-21
 *         Author:    chencheng
 *
 *        History:    initial draft
 ******************************************************************************
 */

#include "OpenCLTask.h"

#include <cstring>
#include <fstream>
#include <sstream>

#define VENDOR_NAME "NVIDIA"


OpenCLTask::OpenCLTask()
{
    /* platform */
    cl_uint             _platformNum = 0;
    cl_platform_id *    _platforms = NULL;
    cl_uint             _platformIdx = 0;

    /* device */
    cl_device_id **     _devices = NULL;
    cl_uint *           _deviceNum = NULL;
    cl_uint *           _deviceIdx = NULL;

    /* _context and command queue */
    cl_context          _context = NULL;
    cl_command_queue    _commandQueue = NULL;    
}


OpenCLTask::~OpenCLTask()
{

}


cl_int OpenCLTask::SetupOpenCL()
{
    cl_int status = CL_SUCCESS;
    
    /* get all platform IDs */
    status = clGetPlatformIDs(0, NULL, &(_platformNum));/* get platform number */
    if (CL_SUCCESS != status )
    {
        printf("Error: clGetPlatformIDs %d.\n", status);
        return status;
    }
    
    _platforms = (cl_platform_id *)malloc(_platformNum * sizeof(cl_platform_id)); /*malloc */
    _devices   = (cl_device_id **)malloc(_platformNum * sizeof(cl_device_id *));
    _deviceNum = (cl_uint *)malloc(_platformNum * sizeof(cl_uint));
    _deviceIdx = (cl_uint *)malloc(_platformNum * sizeof(cl_uint));

    status = clGetPlatformIDs(_platformNum, _platforms, NULL);/* get platform IDs */
    if (CL_SUCCESS != status)
    {
        printf("Error: clGetPlatformIDs %d.\n", status);
        return status;
    }

    /* get all _devices id for all _platforms */
    for (cl_uint i = 0; i < _platformNum; i++)
    {
        status = clGetDeviceIDs(_platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &(_deviceNum[i])); /* get device number */
        if (CL_SUCCESS != status) {
            printf("Error: clGetDeviceIDs %d.\n", status);
            return status;
        }

        _devices[i] = (cl_device_id *)malloc(_deviceNum[i] * sizeof(cl_device_id)); /* malloc */

        status = clGetDeviceIDs(_platforms[i], CL_DEVICE_TYPE_ALL, _deviceNum[i], _devices[i], NULL); /* get device IDs */
        if(CL_SUCCESS != status) {
            printf("Error: clGetDeviceIDs %d.\n", status);
            return status;
        }
    }

    /* choose a platform */
    if (1 == _platformNum) /* if only one platform, choose it */
    {
        _platformIdx = 0;
    }
    else /* if many platform, choose the vendor */
    {
        for (uint i = 0; i < _platformNum; i++)
        {
            char buf[100];
            status = clGetPlatformInfo( _platforms[i],
                                           CL_PLATFORM_VENDOR,
                                           100,
                                        buf,
                                        NULL);
            if (CL_SUCCESS != status) {
                printf("Error: clGetPlatformInfo %d.\n", status);
                return status;
            }
            
            if (NULL != strstr(buf, VENDOR_NAME)) {
                _platformIdx = i;
                break;
            }                    
        }
    }

    /* choose a device */
    for (uint i = 0; i < _deviceNum[_platformIdx]; i++)
    {
        cl_device_type deviceType;
        status = clGetDeviceInfo(_devices[_platformIdx][i],
                                 CL_DEVICE_TYPE,
                                 sizeof(cl_device_type),
                                 &deviceType,
                                 NULL
                                );
        if (CL_DEVICE_TYPE_GPU == deviceType) {
            _deviceIdx[_platformIdx] = i;
            break;
        }
    }
    
    /* create a _context */
    _context = clCreateContext(NULL,
                              _deviceNum[_platformIdx],
                              _devices[_platformIdx],
                              NULL,
                              NULL,
                              &status);
    if (CL_SUCCESS != status) {
        printf("Error: clCreateContext %d.\n", status);
        return status;
    }
    
    /* create a command queue */
    _commandQueue = clCreateCommandQueue(_context,    /* or using clCreate_commandQueueWithProperties */
                                            _devices[_platformIdx][_deviceIdx[_platformIdx]],
                                            CL_QUEUE_PROFILING_ENABLE,
                                            &status);
    if (CL_SUCCESS != status) {
        printf("Error: clCreateCommandQueue %d.\n", status);
        return status;
    }
    
    /* printf info */
    printf("OpenCL environment Setup!\n");
    printf("# %d platforms found.\n", _platformNum);
    printf("# %d devices found on platform-%d.\n", _deviceNum[_platformIdx], _platformIdx);
    printf("\n");
    
    return status;
}


cl_int OpenCLTask::CleanOpenCL()
{
    cl_int status = CL_SUCCESS;
    
    if (NULL != _platforms) {
        free(_platforms);
    }
    
    if (NULL != _devices) {
        for (int i = 0; i < _platformNum; i++) {
            free(_devices[i]);
        }
        free(_devices);
    }
    
    if (NULL != _deviceNum) {
        free(_deviceNum);
    }
    
    if (NULL != _deviceIdx) {
        free(_deviceIdx);
    }
    
    if (NULL != _context) {
        status = clReleaseContext(_context);
        if (CL_SUCCESS != status) {
            printf("Error: clRelease_context %d.\n", status);
            return status;
        }
    }
    
    if (NULL != _commandQueue) {
        status = clReleaseCommandQueue(_commandQueue);
        if (CL_SUCCESS != status) {
            printf("Error: clReleaseCommandQueue %d.\n", status);
            return status;
        }
    }
    
    printf("OpenCL environment Released!\n");
    return status;
}


std::string OpenCLTask::GetDirName(const char * filepath)
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


cl_int OpenCLTask::CreateProgram(const char * filepath)
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

    _program = clCreateProgramWithSource(_context,
                                       1,
                                       &srcStr,
                                       NULL,
                                       &status
                                       );
    if (NULL == _program) {
        std::cerr << "Failed to create program: " << status << std::endl;
    }

    status = clBuildProgram(_program, 0, NULL, GetDirName(filepath).c_str(), NULL, NULL);
    if (CL_SUCCESS != status) {
        std::cerr << "Failed to build program: " << status << std::endl;
        clReleaseProgram(_program);
    }

    return status;
}
