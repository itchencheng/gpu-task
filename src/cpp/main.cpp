/*
 ******************************************************************************
 *       Filename:    main.cpp
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

int main(int argc, char ** argv)
{
    cl_int status = CL_SUCCESS;   

    Task1 gpu_task;

    status = gpu_task.SetupOpenCL();

    // create program
    std::string filepath = argv[1];
    std::cout << "filepath: " << argv[1] << std::endl;
    gpu_task.CreateProgram(filepath.c_str());

    // create kernel
    //gpu_task.CreateKernels("kernel_add");
    gpu_task.CreateKernels("kernel_mul");

    // prepare buffer  
    float a[10];
    float b[10];
    float c[10];
    for (int i = 0; i < 10; i++) {
        a[i] = i;
        b[i] = i;    
    }

    gpu_task.CreateMemObjects(a, b);

    // run kernel
    gpu_task.RunKernel();


    // read mem
    status = clEnqueueReadBuffer(gpu_task.commandQueue_,
                        gpu_task.dst_,
                        CL_TRUE,
                        0,
                        sizeof(float) * 10,
                        c,
                        0,
                        NULL,
                        NULL);
    if (CL_SUCCESS != status) {
        printf("Error: clEnqueueReadBuffer failed %d\n", status);    
    }
    
    printf("c: \n");
    for (int i = 0; i < 10; i++) {
        printf("%f ", c[i]);    
    }
    printf("\n");

    gpu_task.ReleaseX();

    // release buffer
    status = gpu_task.CleanOpenCL();

    return 0;    
}
