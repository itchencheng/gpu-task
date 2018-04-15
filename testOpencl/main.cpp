#include<CL/cl.h>

#include<iostream>
using namespace std;

struct OpenCLContext{
/* platform */
};

int main()
{
	cout<<"hello, opencl"<<endl;

	cl_int errNum;

	cl_uint numPlatforms;//cl_int ?
	cl_platform_id *platformIds;
	cl_platform_id chosenPlatform;
	cl_context context = NULL;

	/* platform */
	errNum = clGetPlatformIDs(0, NULL, &numPlatforms);
	if(CL_SUCCESS != errNum || numPlatforms <= 0 )
	{
		cerr << "Failded to find any OPENCL platforms" <<endl;
	}
	else
	{

		platformIds = new cl_platform_id[numPlatforms];

		clGetPlatformIDs(numPlatforms, platformIds, NULL);

		for(cl_uint i = 0; i < numPlatforms; i++)
		{
			cl_uint slen = 0;
			char *buf;

			clGetPlatformInfo(platformIds[i], CL_PLATFORM_NAME, 0, NULL, &slen );

			buf = new char [slen];

			clGetPlatformInfo(platformIds[i], CL_PLATFORM_NAME, slen, buf, NULL);
			cout<<"CL_PLATFORM_NAME: "<<buf<<endl;

			delete [] buf;
		}
	}

	chosenPlatform = platformIds[0];

	/* clcreateContext */
	cl_context_properties contextProperties[] = 
	{
		CL_CONTEXT_PLATFORM, (cl_context_properties)chosenPlatform,
		0
	};
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
			&errNum
			);
		if(CL_SUCCESS != errNum)
		{
			cerr << "Failed to create CPU context, too."<<endl;
			return NULL;
		}
	}

	/* choose a device, create command queue */
	size_t devicesNum;
	cl_device_id *devices;
	cl_device_id chosenDevice;

	/* get device number */
	errNum = clGetContextInfo(
		context,
		CL_CONTEXT_DEVICES,
		0,
		NULL,
		&deviceNum
		);
	if(CL_SUCCESS != errNum)
	{
		cerr << "Failed to get clGetContextInfo(,CL_CONTEXT_DEVICES,): "<< errNum <<endl;
	}
	/* get devices id */
	if(deviceNum <= 0)
	{
		cerr << "No device found !"<<endl;
	}
	else
	{
		errNum = clGetContextInfo(// if have clGetDeviceId?????
			devicesNum,


	}


	delete [] platformIds;


	return 0;
}