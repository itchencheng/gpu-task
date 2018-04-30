/* test opencl
 * @author: chencheng
 * @date  : 20180420
 */

#include<iostream>
#include"setup_opencl.h"
using namespace std;

int main(int argc, char** argv)
{
	cout << "hello OpenCL!" <<endl;
	SetupOpenCL();
	return 0;
}