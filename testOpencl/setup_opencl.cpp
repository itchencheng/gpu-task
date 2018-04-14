
#include"setup_opencl.h"

using namespace std;

cl_int CreateContext()
{
	cl_int errNum;
	
	/* get platform info */
	cl_uint numPlatforms;
	cl_platform_id firstPlatformId;
	errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
	if(CL_SUCCESS != errNum)
	{
		cerr << "Failed to find andy OpenCL platforms." <<endl;
		return NULL;
	}

	/* create Context */
	cl_context context = NULL; /* cl_context 使用NULL来初始化 ? */
	/* GPU */
	cl_context_properties contextProperties[] = 
	{
		CL_CONTEXT_PLATFORM, (cl_context_properties)firstPlatformId,
		0
	}
	context = clCreateContextFromType( 
		contextProperties,
		CL_DEVICE_TYPE_GPU,
		NULL,
		NULL,
		&errNum
		);
	if(CL_SUCCESS != errNum)
	{
		cout << "could not create GPU context, trying CPU..."<<endl;
		/* CPU */
		context = clCreateContextFromType(
			contextProperties,
			CL_DEVICE_TYPE_GPU,
			NULL,
			NULL,
			&errNUm
			);
		if(CL_SUCCESS != errNum)
		{
			cerr << "Failed to create CPU context, too."<<end;
			return NULL;
		}
	}
}


/* pick a device, create a command queue */
cl_command_queue CreateCommandQueue(
	cl_context context,
	cl_device_id *device
)
{
}