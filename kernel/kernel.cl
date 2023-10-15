
__kernel void kernel_test(
        __global const float *src,
        __global float *dst)
{
    int x = get_global_id(0);
    dst[x] = src[x] * 2;
}
