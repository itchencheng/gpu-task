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

#include "helper.h"

int main(int argc, char** argv)
{
    cl_int status = CL_SUCCESS;    

	std::cout << "hello OpenCL!" << std::endl;

    ProgramCtx programCtx;
    programCtx.handle = (OclHandle*)calloc(1, sizeof(OclHandle));

    status = SetupOpenCL(programCtx.handle);

    // create program
    std::string filepath = argv[1];
    std::cout << "filepath: " << argv[1] << std::endl;
    CreateProgram(&programCtx, filepath.c_str());

    // create kernel
    CreateKernels(&programCtx, "kernel_add");

    // prepare buffer  
    float a[10];
    float b[10];
    float c[10];
    for (int i = 0; i < 10; i++) {
        a[i] = i;
        b[i] = i;    
    }

    CreateMemObjects(&programCtx, a, b);

    // run kernel
    RunKernel(&programCtx);


    // read mem
    status = clEnqueueReadBuffer(programCtx.handle->commandQueue,
                        programCtx.dst,
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

    ReleaseX(&programCtx);

    // release buffer
    status = ReleaseOpenCL(programCtx.handle);

    free(programCtx.handle);

	return 0;
}

