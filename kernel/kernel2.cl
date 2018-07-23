/*
 ******************************************************************************
 *       Filename:    kernel2.cl
 *    Description:    opencl kernel file
 *       
 *        Version:    1.0
 *        Created:    2018-07-21
 *         Author:    chencheng
 *
 *        History:    initial draft
 ******************************************************************************
 */
 
__kernel void kernel_mul(
	__global const float *a,
	__global const float *b,
	__global float *result
	)
{
	int gid = get_global_id(0);

	result[gid] = a[gid] * b[gid];
}
