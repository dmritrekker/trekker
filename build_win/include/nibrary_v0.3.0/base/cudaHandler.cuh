#pragma once

#ifdef HAVE_CUDA

#include "base/nibr.h"
#include <cuda_runtime_api.h>
#include <cuda.h>

namespace NIBR 
{

    namespace CUDAHANDLER 
    {

        extern bool             cuda_initialized;
        extern cudaDeviceProp   cuda_dev_prop;
        extern int              cuda_maxThreadsPerBlock;
        extern int              cuda_maxThreadsDim[3];
        extern int              cuda_maxGridSize[3];

        extern void CUDA_INIT();
        extern void CUDA_EXIT();
        extern void CUDA_PRINT_ERROR(cudaError_t cudaFunction);
        extern void CUDA_PRINT_INFO();

    }

}

#endif