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

#include "GPUTask.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Task1 : public GPUTask
{
public:
	Task1();
	~Task1();

	cl_int CreateProgram(const char * filepath);
	cl_int CreateKernels(const char * kernel_name);
	cl_int CreateMemObjects(float * a, float * b);
	cl_int RunKernel();
	cl_int ReleaseX();

private:
    std::string GetDirName(const char * filepath);

public:
    cl_program        program_;
    cl_kernel         kernel1_;
    cl_kernel         kernel2_;

    cl_mem            src1_;
    cl_mem            src2_;
    cl_mem            dst_;
};


#endif
