/*
 ******************************************************************************
 *       Filename:    Task1.h
 *    Description:    header file
 *       
 *        Version:    1.0
 *        Created:    2018-07-21
 *         Author:    chencheng
 *
 *        History:    initial draft
 ******************************************************************************
 */

#ifndef __TASK1_H
#define __TASK1_H

#include "OpenCLTask.h"

class Task1 : public OpenCLTask
{
public:
	Task1();
	~Task1();

	cl_int CreateKernels(const char * kernel_name);
	cl_int CreateMemObjects(float * a, float * b);
	cl_int RunKernel();
	cl_int ReleaseX();

public:
    cl_kernel         _kernel1;
    cl_kernel         _kernel2;

    cl_mem            _src1;
    cl_mem            _src2;
    cl_mem            _dst;
};


#endif
