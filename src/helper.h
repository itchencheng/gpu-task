/*
 ******************************************************************************
 *       Filename:    helper.h
 *    Description:    header file
 *       
 *        Version:    1.0
 *        Created:    2018-07-21
 *         Author:    chencheng
 *
 *        History:    initial draft
 ******************************************************************************
 */

#ifndef __HELPER_H
#define __HELPER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "setup_opencl.h"

struct ProgramCtx {
    OclHandle *       handle;
    
    cl_program        program;
    cl_kernel         kernel1;
    cl_kernel         kernel2;

    cl_mem            src1;
    cl_mem            src2;
    cl_mem            dst;
};


cl_int CreateProgram(ProgramCtx * pCtx, const char * filepath);

cl_int CreateKernels(ProgramCtx * pCtx, const char * kernel_name);

cl_int CreateMemObjects(ProgramCtx * pCtx, float * a, float * b);

cl_int RunKernel(ProgramCtx * pCtx);

cl_int ReleaseX(ProgramCtx * pCtx);


#endif /* __HELPER_H */
