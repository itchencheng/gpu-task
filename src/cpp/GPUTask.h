/*
 ******************************************************************************
 *       Filename:    GPUTask.h
 *    Description:    cpp file
 *       
 *        Version:    1.0
 *        Created:    2018-07-21
 *         Author:    chencheng
 *
 *        History:    initial draft
 ******************************************************************************
 */

#ifndef __GPUTask_H
#define __GPUTask_H


#include <CL/cl.h>


class GPUTask
{
public:
    GPUTask();
    ~GPUTask();

    cl_int SetupOpenCL();
    cl_int CleanOpenCL();

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
};


#endif //__GPUTask_H