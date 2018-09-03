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
    program_ = NULL;
    kernel1_ = NULL;
    kernel2_ = NULL;

    src1_ = NULL;
    src2_ = NULL;
    dst_ = NULL;
}


Task1::~Task1()
{

}


std::string Task1::GetDirName(const char * filepath)
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


cl_int Task1::CreateProgram(const char * filepath)
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


/* kernel */
cl_int Task1::CreateKernels(const char * kernel_name)
{
    cl_int status = CL_SUCCESS;

    kernel1_ = clCreateKernel(program_, kernel_name, &status);
    if (CL_SUCCESS == status) {
        return status;
    }

    return status;
}



/* memObj */
cl_int Task1::CreateMemObjects(float * a, float * b)
{
    cl_int status = CL_SUCCESS;

    src1_ = clCreateBuffer(context_,
                                CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                sizeof(float) * 10,
                                a, &status);

    src2_ = clCreateBuffer(context_,
                                CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                sizeof(float) * 10,
                                b, &status);

    dst_  = clCreateBuffer(context_,
                                CL_MEM_READ_WRITE,
                                sizeof(float) * 10,
                                a, &status);

    return status;
}


/* run kernel */
cl_int Task1::RunKernel()
{
    cl_int status = CL_SUCCESS;

    status  = clSetKernelArg(kernel1_, 0, sizeof(cl_mem), &src1_);
    status |= clSetKernelArg(kernel1_, 1, sizeof(cl_mem), &src2_);
    status |= clSetKernelArg(kernel1_, 2, sizeof(cl_mem), &dst_);
    if (CL_SUCCESS != status) {
        printf("Error: clSetKernelArg failed %d.\n", status);
        return status;
    }

    size_t globalSize[2] = {10, 1}; 
    size_t localSize[2]  = {1,  1};

    status = clEnqueueNDRangeKernel(commandQueue_,
                                    kernel1_,
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

    if (NULL != kernel1_) {
        clReleaseKernel(kernel1_);
    } 

    if (NULL != program_) {
        clReleaseProgram(program_);
    }    

    clReleaseMemObject(src1_);
    clReleaseMemObject(src2_);
    clReleaseMemObject(dst_);

    return status;
}
