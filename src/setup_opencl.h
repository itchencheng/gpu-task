#ifndef __SETUP_OPENCL
#define __SETUP_OPENCL

#include<CL/cl.h>
/*
 * 0: no C++ OOP version yet 
 */
#define CPP_STYLE 0

#if CPP_STYLE
/* C++ style interface */

#else

/* C style interface */
struct OclHandle{
	/* platform */
	cl_uint platformNum;
	cl_platform_id *platforms;
	cl_uint platformIdx;
	/* device */
	cl_device_id   **devices;
	cl_uint        *deviceNum;
	cl_uint        *deviceIdx;
	/* context and command queue */
	cl_context context;
	cl_command_queue commandQueue;	
};

cl_int SetupOpenCL(OclHandle* handle);
cl_int ReleaseOpenCL(OclHandle* handle);



#endif //CPP_STYLE



#endif //__SETUP_OPENCL