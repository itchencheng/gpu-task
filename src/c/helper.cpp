/*
 ******************************************************************************
 *       Filename:    helper.cpp
 *    Description:    cpp file
 *       
 *        Version:    1.0
 *        Created:    2018-07-21
 *         Author:    chencheng
 *
 *        History:    initial draft
 ******************************************************************************
 */

#include "helper.h"


cl_int CreateProgram(ProgramCtx * pCtx, const char * filepath)
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

    printf("%s\n", srcStr);    

    pCtx->program = clCreateProgramWithSource(pCtx->handle->context,
                                       1,
                                       &srcStr,
                                       NULL,
                                       NULL
                                       );
    if (NULL == pCtx->program) {
        std::cerr << "Failed to create program " << std::endl;
    }

    status = clBuildProgram(pCtx->program, 0, NULL, NULL, NULL, NULL);
    if (CL_SUCCESS != status) {
        std::cerr << "Failed to build program " << std::endl;
        clReleaseProgram(pCtx->program);
    }

    return status;
}


/* kernel */
cl_int CreateKernels(ProgramCtx * pCtx, const char * kernel_name)
{
    cl_int status = CL_SUCCESS;

    pCtx->kernel1 = clCreateKernel(pCtx->program, kernel_name, &status);
    if (CL_SUCCESS == status) {
        return status;
    }

    return status;
}



/* memObj */
cl_int CreateMemObjects(ProgramCtx * pCtx, float * a, float * b)
{
    cl_int status = CL_SUCCESS;

    pCtx->src1 = clCreateBuffer(pCtx->handle->context,
                                CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                sizeof(float) * 10,
                                a, &status);

    pCtx->src2 = clCreateBuffer(pCtx->handle->context,
                                CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                sizeof(float) * 10,
                                b, &status);

    pCtx->dst  = clCreateBuffer(pCtx->handle->context,
                                CL_MEM_READ_WRITE,
                                sizeof(float) * 10,
                                a, &status);

    return status;
}


/* run kernel */
cl_int RunKernel(ProgramCtx * pCtx)
{
    cl_int status = CL_SUCCESS;

    status  = clSetKernelArg(pCtx->kernel1, 0, sizeof(cl_mem), &(pCtx->src1));
    status |= clSetKernelArg(pCtx->kernel1, 1, sizeof(cl_mem), &(pCtx->src2));
    status |= clSetKernelArg(pCtx->kernel1, 2, sizeof(cl_mem), &(pCtx->dst));
    if (CL_SUCCESS != status) {
        printf("Error: clSetKernelArg failed %d.\n", status);
        return status;
    }

    size_t globalSize[2] = {10, 1}; 
    size_t localSize[2]  = {1,  1};

    status = clEnqueueNDRangeKernel(pCtx->handle->commandQueue,
                                    pCtx->kernel1,
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


cl_int ReleaseX(ProgramCtx * pCtx)
{
    cl_int status = CL_SUCCESS;

    if (NULL != pCtx->kernel1) {
        clReleaseKernel(pCtx->kernel1);
    } 

    if (NULL != pCtx->program) {
        clReleaseProgram(pCtx->program);
    }    

    clReleaseMemObject(pCtx->src1);
    clReleaseMemObject(pCtx->src2);
    clReleaseMemObject(pCtx->dst );

    return status;
}
