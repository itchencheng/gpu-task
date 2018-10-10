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
    cl_uint             _platformNum;
    cl_platform_id *    _platforms;
    cl_uint             _platformIdx;

    /* device */
    cl_device_id **     _devices;
    cl_uint *           _deviceNum;
    cl_uint *           _deviceIdx;

    /* context and command queue */
    cl_context          _context;
    cl_command_queue    _commandQueue;

    /* program */
    cl_program        _program;
};


#endif //__OpenCLTask_H
