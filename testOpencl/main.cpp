#include<CL/cl.h>

#include<iostream>
using namespace std;

int main()
{
	cout<<"hello, opencl"<<endl;

	cl_uint errNum;
	cl_uint numPlatforms;
	errNum = clGetPlatformIDs(0, NULL, &numPlatforms);
	if(CL_SUCCESS != errNum || numPlatforms <= 0 )
	{
		cerr << "Failded to find any OPENCL platforms" <<endl;
	}
	else
	{
		cl_platform_id * platformIds;
		platformIds = new cl_platform_id[numPlatforms];

		clGetPlatformIDs(numPlatforms, platformIds, NULL);

		for(cl_uint i = 0; i < numPlatforms; i++)
		{
			cl_uint slen = 0;
			char *buf;

			clGetPlatformInfo(platformIds[i], CL_PLATFORM_NAME, 0, NULL, &slen );
			cout<<"buf length: "<<slen<<endl;

			buf = new char [slen];

			clGetPlatformInfo(platformIds[i], CL_PLATFORM_NAME, slen, buf, NULL);
			cout<<"buf content: "<<buf<<endl;

			delete [] buf;
		}

		delete [] platformIds;

		
	}

	return 0;
}