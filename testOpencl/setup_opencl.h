#ifndef __SETUP_OPENCL
#define __SETUP_OPENCL

#include<CL/cl.h>
/*
 * 0: no C++ OOP version yet 
 */
#define CPP_STYLE 0



#if CPP_STYLE
/* C++ style interface */

#else

/* C style interface */
struct CtxHandle{
	
};


#endif //CPP_STYLE



#endif //__SETUP_OPENCL