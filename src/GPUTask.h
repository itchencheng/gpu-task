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

class GPUTask{
public:
    GPUTask();
    ~GPUTask();

    cl_int SetupOpenCL();
    cl_int CleanOpenCL();

public:
	/* platform */
    cl_uint             platformNum;
    cl_platform_id *    platforms;
    cl_uint             platformIdx;

    /* device */
    cl_device_id **     devices;
    cl_uint *           deviceNum;
    cl_uint *           deviceIdx;

    /* context and command queue */
    cl_context          context;
    cl_command_queue    commandQueue;
};


#endif //__GPUTask_H