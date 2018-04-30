
#include"setup_opencl.h"

#include<cstdio>
#include<cstdlib>
#include<cstring>

#define VENDOR_NAME "NVIDIA"


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
		if(CL_SUCCESS != status)
		{
			printf("Error: clGetDeviceIDs %d.\n", status);
			return status;
		}
	}

	/* choose a platform */
	cl_uint platformIdx = 100;
	if( 1 == platformNum ) /* if only one platform, choose it */
	{
		platformIdx = 0;
	}
	else /* if many platform, choose the vendor */
	{
		for(uint i = 0; i < platformNum; i++)
		{
			char buf[100];
			status = clGetPlatformInfo( platforms[i],
									   	CL_PLATFORM_VENDOR,
									   	100,
									    buf,
									    NULL);
			if(CL_SUCCESS != status)
			{
				printf("Error: clGetPlatformInfo %d.\n", status);
				return status;
			}
			
			if(NULL != strstr(buf, VENDOR_NAME))
			{
				platformIdx = i;
				break;
			}				    
		}
	}

	/* choose a device */
	for(uint i = 0; i < deviceNum[platformIdx]; i++)
	{
		cl_device_type deviceType;
		status = clGetDeviceInfo(devices[platformIdx][i],
								 CL_DEVICE_TYPE,
								 sizeof(cl_device_type),
								 &deviceType,
								 NULL
								);
		if(CL_DEVICE_TYPE_GPU == deviceType)
		{
			deviceIdx[platformIdx] = i;
			break;
		}
	}
	
	/* create a context */
	cl_context context;
	context = clCreateContext(NULL,
							  deviceNum[platformIdx],
							  devices[platformIdx],
							  NULL,
							  NULL,
							  &status);
	if(CL_SUCCESS != status)
	{
		printf("Error: clCreateContext %d.\n", status);
		return status;
	}
	
	/* create a command queue */
	cl_command_queue commandQueue;
	commandQueue = clCreateCommandQueue(context,	/* or using clCreateCommandQueueWithProperties */
										devices[platformIdx][deviceIdx[platformIdx]],
										CL_QUEUE_PROFILING_ENABLE,
										&status);
	if(CL_SUCCESS != status)
	{
		printf("Error: clCreateCommandQueue %d.\n", status);
		return status;
	}
	
	return status;
}


cl_int ReleaseOpenCL()
{
	cl_int status = CL_SUCCESS;

	return status;
}