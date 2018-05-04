/* test opencl
 * @author: chencheng
 * @date  : 20180420
 */

#include<iostream>
#include"setup_opencl.h"
using namespace std;

int main(int argc, char** argv)
{
    cl_int status;
    
	cout << "hello OpenCL!" <<endl;

    OclHandle *handle;
    handle = (OclHandle*)calloc(1, sizeof(OclHandle));
    
    status = SetupOpenCL(handle);
    
    status = ReleaseOpenCL(handle);
    
    free(handle);
	return 0;
}