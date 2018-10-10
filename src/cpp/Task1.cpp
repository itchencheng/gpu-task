/*
 ******************************************************************************
 *       Filename:    Task1.cpp
 *    Description:    cpp file
 *       
 *        Version:    1.0
 *        Created:    2018-07-21
 *         Author:    chencheng
 *
 *        History:    initial draft
 ******************************************************************************
 */

#include "Task1.h"


Task1::Task1()
{
    _program = NULL;
    _kernel1 = NULL;
    _kernel2 = NULL;

    _src1 = NULL;
    _src2 = NULL;
    _dst = NULL;
}


Task1::~Task1()
{

}



/* kernel */
cl_int Task1::CreateKernels(const char * kernel_name)
{
    cl_int status = CL_SUCCESS;

    _kernel1 = clCreateKernel(_program, kernel_name, &status);
    if (CL_SUCCESS == status) {
        return status;
    }

    return status;
}



/* memObj */
cl_int Task1::CreateMemObjects(float * a, float * b)
{
    cl_int status = CL_SUCCESS;

    _src1 = clCreateBuffer(_context,
                                CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                sizeof(float) * 10,
                                a, &status);

    _src2 = clCreateBuffer(_context,
                                CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                sizeof(float) * 10,
                                b, &status);

    _dst  = clCreateBuffer(_context,
                                CL_MEM_READ_WRITE,
                                sizeof(float) * 10,
                                a, &status);

    return status;
}


/* run kernel */
cl_int Task1::RunKernel()
{
    cl_int status = CL_SUCCESS;

    status  = clSetKernelArg(_kernel1, 0, sizeof(cl_mem), &_src1);
    status |= clSetKernelArg(_kernel1, 1, sizeof(cl_mem), &_src2);
    status |= clSetKernelArg(_kernel1, 2, sizeof(cl_mem), &_dst);
    if (CL_SUCCESS != status) {
        printf("Error: clSetKernelArg failed %d.\n", status);
        return status;
    }

    size_t globalSize[2] = {10, 1}; 
    size_t localSize[2]  = {1,  1};

    status = clEnqueueNDRangeKernel(_commandQueue,
                                    _kernel1,
                                    1,
                                    NULL,
                                    globalSize, 
                                    localSize,
                                    0,
                                    NULL,
                                    NULL
                                     );
    if (CL_SUCCESS != status) {
        printf("Error: clEnqueueNDRangeKernel failed %d.\n", status);
        return status;
    }

    return status;
}


cl_int Task1::ReleaseX()
{
    cl_int status = CL_SUCCESS;

    if (NULL != _kernel1) {
        clReleaseKernel(_kernel1);
    } 

    if (NULL != _program) {
        clReleaseProgram(_program);
    }    

    clReleaseMemObject(_src1);
    clReleaseMemObject(_src2);
    clReleaseMemObject(_dst);

    return status;
}
