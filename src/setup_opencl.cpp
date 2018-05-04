
#include"setup_opencl.h"

#include<cstdio>
#include<cstdlib>
#include<cstring>

#define VENDOR_NAME "NVIDIA"


cl_int SetupOpenCL(OclHandle* handle)
{
	cl_int status = CL_SUCCESS;
	
	/* get all platform IDs */
	status = clGetPlatformIDs(0, NULL, &(handle->platformNum));/* get platform number */
	if(CL_SUCCESS != status )
	{
		printf("Error: clGetPlatformIDs %d.\n", status);
		return status;
	}
	
	handle->platforms = (cl_platform_id*)malloc(handle->platformNum * sizeof(cl_platform_id)); /*malloc */
	handle->devices   = (cl_device_id**)malloc(handle->platformNum * sizeof(cl_device_id*));
	handle->deviceNum = (cl_uint*)malloc(handle->platformNum * sizeof(cl_uint));
	handle->deviceIdx = (cl_uint*)malloc(handle->platformNum * sizeof(cl_uint));

	status = clGetPlatformIDs(handle->platformNum, handle->platforms, NULL);/* get platform IDs */
	if(CL_SUCCESS != status)
	{
		printf("Error: clGetPlatformIDs %d.\n", status);
		return status;
	}

	/* get all devices id for all platforms */
	for(cl_uint i = 0; i < handle->platformNum; i++)
	{
		status = clGetDeviceIDs(handle->platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &(handle->deviceNum[i])); /* get device number */
		if(CL_SUCCESS != status)
		{
			printf("Error: clGetDeviceIDs %d.\n", status);
			return status;
		}

		handle->devices[i] = (cl_device_id*)malloc(handle->deviceNum[i] * sizeof(cl_device_id)); /* malloc */

		status = clGetDeviceIDs(handle->platforms[i], CL_DEVICE_TYPE_ALL, handle->deviceNum[i], handle->devices[i], NULL); /* get device IDs */
		if(CL_SUCCESS != status)
		{
			printf("Error: clGetDeviceIDs %d.\n", status);
			return status;
		}
	}

	/* choose a platform */
	if( 1 == handle->platformNum ) /* if only one platform, choose it */
	{
		handle->platformIdx = 0;
	}
	else /* if many platform, choose the vendor */
	{
		for(uint i = 0; i < handle->platformNum; i++)
		{
			char buf[100];
			status = clGetPlatformInfo( handle->platforms[i],
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
				handle->platformIdx = i;
				break;
			}				    
		}
	}

	/* choose a device */
	for(uint i = 0; i < handle->deviceNum[handle->platformIdx]; i++)
	{
		cl_device_type deviceType;
		status = clGetDeviceInfo(handle->devices[handle->platformIdx][i],
								 CL_DEVICE_TYPE,
								 sizeof(cl_device_type),
								 &deviceType,
								 NULL
								);
		if(CL_DEVICE_TYPE_GPU == deviceType)
		{
			handle->deviceIdx[handle->platformIdx] = i;
			break;
		}
	}
	
	/* create a context */
	handle->context = clCreateContext(NULL,
									  handle->deviceNum[handle->platformIdx],
									  handle->devices[handle->platformIdx],
									  NULL,
							  			NULL,
							  			&status);
	if(CL_SUCCESS != status)
	{
		printf("Error: clCreateContext %d.\n", status);
		return status;
	}
	
	/* create a command queue */
	handle->commandQueue = clCreateCommandQueue(handle->context,	/* or using clCreateCommandQueueWithProperties */
											handle->devices[handle->platformIdx][handle->deviceIdx[handle->platformIdx]],
											CL_QUEUE_PROFILING_ENABLE,
											&status);
	if(CL_SUCCESS != status)
	{
		printf("Error: clCreateCommandQueue %d.\n", status);
		return status;
	}
	
	/* printf info */
	printf("OpenCL environment Setup!\n");
	printf("# %d platforms found.\n", handle->platformNum);
	printf("# %d devices found on platform-%d.\n", handle->deviceNum[handle->platformIdx], handle->platformIdx);
	printf("\n");
	
	return status;
}


cl_int ReleaseOpenCL(OclHandle* handle)
{
	cl_int status = CL_SUCCESS;
	
	if(NULL != handle->platforms)
	{
		free(handle->platforms);
	}
	
	if(NULL != handle->devices)
	{
		for(int i = 0; i< handle->platformNum; i++)
		{
			free(handle->devices[i]);
		}
		free(handle->devices);
	}
	
	if(NULL != handle->deviceNum)
	{
		free(handle->deviceNum);
	}
	
	if(NULL != handle->deviceIdx)
	{
		free(handle->deviceIdx);
	}
	
	if(NULL != handle->context)
	{
		status = clReleaseContext(handle->context);
		if(CL_SUCCESS != status)
		{
			printf("Error: clReleaseContext %d.\n", status);
			return status;
		}
	}
	
	if(NULL != handle->commandQueue)
	{
		status = clReleaseCommandQueue(handle->commandQueue);
		if(CL_SUCCESS != status)
		{
			printf("Error: clReleaseCommandQueue %d.\n", status);
			return status;
		}
	}
	
	printf("OpenCL environment Released!\n");
	return status;
}