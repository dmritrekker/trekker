#pragma once

#include "base/nibr.h"

#ifdef HAVE_CUDA

typedef unsigned int uint;

#include "interpolationCuda.cuh"


// TODO: CUDA template functions
// Template function decleration turned out to be problematic in this case.
// Since the CUDA part is still under development, let's keep it like this for now.

namespace NIBR
{

    // template<typename T1,typename T2, typename T_OUT>
    // void imgResample(CUDAImage<T1>* img, T2** M, CUDAImage<T_OUT>* imgOut);

    void imgResample(CUDAImage<float>* img, float** M, CUDAImage<float>* imgOut);
    
}


/*
template<typename T1,typename T2, typename T_OUT>
__device__ void dCUDA_imgResample(NIBR::CUDAImage<T1>* img, uint* ijk, T2** M, NIBR::CUDAImage<T_OUT>* imgOut)
{
    // We will loop ijk of the output image. So we will compute a value for all the voxels in the output.
    if (ijk[0]>=imgOut->imgDims[0]) return;
    if (ijk[1]>=imgOut->imgDims[1]) return;
    if (ijk[2]>=imgOut->imgDims[2]) return;

    int64_t ind = ijk[0]*imgOut->s2i[0] + ijk[1]*imgOut->s2i[1] + ijk[2]*imgOut->s2i[2];

    // Convert ijk of the output to xyz
    float pOut[3];
    pOut[0] = imgOut->ijk2xyz[0][0]*ijk[0] + imgOut->ijk2xyz[0][1]*ijk[1] + imgOut->ijk2xyz[0][2]*ijk[2] + imgOut->ijk2xyz[0][3];
    pOut[1] = imgOut->ijk2xyz[1][0]*ijk[0] + imgOut->ijk2xyz[1][1]*ijk[1] + imgOut->ijk2xyz[1][2]*ijk[2] + imgOut->ijk2xyz[1][3];
    pOut[2] = imgOut->ijk2xyz[2][0]*ijk[0] + imgOut->ijk2xyz[2][1]*ijk[1] + imgOut->ijk2xyz[2][2]*ijk[2] + imgOut->ijk2xyz[2][3];

    // Apply the transform
    float pInp[3];
    pInp[0] = pOut[0]*M[0][0] + pOut[1]*M[0][1] + pOut[2]*M[0][2] + M[0][3];
    pInp[1] = pOut[0]*M[1][0] + pOut[1]*M[1][1] + pOut[2]*M[1][2] + M[1][3];
    pInp[2] = pOut[0]*M[2][0] + pOut[1]*M[2][1] + pOut[2]*M[2][2] + M[2][3];
        
    // Interpolate the output value
    imgOut->cudaData[ind] = (*img)(pInp);

}


template<typename T1,typename T2, typename T_OUT>
__global__ void gCUDA_imgResample(NIBR::CUDAImage<T1>* img, T2** M, NIBR::CUDAImage<T_OUT>* imgOut) 
{

    uint ijk[3];
    ijk[0]      = blockIdx.x * blockDim.x + threadIdx.x;
    ijk[1]      = blockIdx.y * blockDim.y + threadIdx.y;
    ijk[2]      = blockIdx.z * blockDim.z + threadIdx.z;
    
    dCUDA_imgResample(img, ijk, M, imgOut);
}

namespace NIBR
{

    template<typename T1,typename T2, typename T_OUT>
    void imgResample(CUDAImage<T1>* img, T2** M, CUDAImage<T_OUT>* imgOut) {    


        T2** dPointerInDevice;
        T2** dPointerInHost = (T2**)malloc(3*sizeof(T2*));
        cudaMalloc((void**)&dPointerInDevice,3*sizeof(T2*));
        for (int i = 0; i < 3; i++) {
            cudaMalloc((void **)&(dPointerInHost[i]),4*sizeof(T2));
            cudaMemcpy(dPointerInHost[i],M[i],4*sizeof(T2),cudaMemcpyHostToDevice);
        }
        cudaMemcpy(dPointerInDevice,dPointerInHost,3*sizeof(T2*),cudaMemcpyHostToDevice);

        dim3 blockSize(16,16,4); // thread array within a block

        uint gx = (imgOut->imgDims[0] + blockSize.x - 1) / blockSize.x;
        uint gy = (imgOut->imgDims[1] + blockSize.y - 1) / blockSize.y;
        uint gz = (imgOut->imgDims[2] + blockSize.z - 1) / blockSize.z;
        
        dim3 gridSize(gx,gy,gz); // block array within the grid

        gCUDA_imgResample<<<gridSize,blockSize>>>(img->gpuClass, dPointerInDevice, imgOut->gpuClass);
        cudaDeviceSynchronize();

        for (int i = 0; i < 3; i++)
            cudaFree(dPointerInHost[i]);

        free(dPointerInHost);
        cudaFree(dPointerInDevice);
        
    }
    
}
*/
 
#endif