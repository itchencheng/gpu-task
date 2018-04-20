
#include"setup_opencl.h"

#include<cstdio>
#include<cstdlib>

cl_int SetupOpenCL()
{
	printf("Begin to setup OpenCL environment!\n");

	cl_int status = CL_SUCCESS;
	
	/* get all platform IDs */
	cl_uint platformNum;
	status = clGetPlatformIDs(0, NULL, &platformNum);/* get platform number */
	if(CL_SUCCESS != status || platformNum < 1)
	{
		printf("Error: clGetPlatformIDs %d.\n", status);
		return status;
	}
	if(platformNum < 1)
	{
		printf("Error: platform number < 1.\n");
		return -1;
	}

	cl_platform_id *platforms;
	cl_device_id   **devices;
	cl_uint        *deviceNum;
	cl_uint        *deviceIdx;
	
	platforms = (cl_platform_id*)malloc(platformNum * sizeof(cl_platform_id)); /*malloc */
	devices   = (cl_device_id**)malloc(platformNum * sizeof(cl_device_id*));
	deviceNum = (cl_uint*)malloc(platformNum * sizeof(cl_uint));
	deviceIdx = (cl_uint*)malloc(platformNum * sizeof(cl_uint));

	status = clGetPlatformIDs(platformNum, platforms, NULL);/* get platform IDs */
	if(CL_SUCCESS != status)
	{
		printf("Error: clGetPlatformIDs %d.\n", status);
		return status;
	}

	/* get all devices id for all platforms */
	for(cl_uint i = 0; i < platformNum; i++)
	{
		status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceNum[i]); /* get device number */
		if(CL_SUCCESS != status)
		{
			printf("Error: clGetDeviceIDs %d.\n", status);
			return status;
		}
		if(deviceNum[i] < 1)
		{
			printf("Error: device number < 1.\n");
			return -1;
		}

		devices[i] = (cl_device_id*)malloc(deviceNum[i] * sizeof(cl_device_id)); /* malloc */

		status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, deviceNum[i], devices[i], NULL); /* get device IDs */
	}

	/* choose a platform */

	/* choose a device */

	/* create a context */

	/* create a command queue */

	return status;
}


cl_int ReleaseOpenCL()
{
	cl_int status = CL_SUCCESS;

	return status;
}