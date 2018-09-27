/*
 ******************************************************************************
 *       Filename:    OpenCLTask.h
 *    Description:    cpp file
 *       
 *        Version:    1.0
 *        Created:    2018-07-21
 *         Author:    chencheng
 *
 *        History:    initial draft
 ******************************************************************************
 */

#ifndef __OpenCLTask_H
#define __OpenCLTask_H


#include <CL/cl.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

class OpenCLTask
{
public:
    OpenCLTask();
    ~OpenCLTask();

    cl_int SetupOpenCL();
    cl_int CleanOpenCL();
	cl_int CreateProgram(const char * filepath);

    std::string GetDirName(const char * filepath);

public:
	/* platform */
    cl_uint             platformNum_;
    cl_platform_id *    platforms_;
    cl_uint             platformIdx_;

    /* device */
    cl_device_id **     devices_;
    cl_uint *           deviceNum_;
    cl_uint *           deviceIdx_;

    /* context and command queue */
    cl_context          context_;
    cl_command_queue    commandQueue_;

    /* program */
    cl_program        program_;
};


#endif //__OpenCLTask_H
