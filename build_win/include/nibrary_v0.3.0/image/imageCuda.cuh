#pragma once

#ifdef HAVE_CUDA

#include <cstddef>
#include <stdio.h>

#include "base/nibr.h"
#include "image/image.h"
#include "base/cudaHandler.cuh"
#include "interpolationCuda.cuh"

namespace NIBR
{

    template <typename T>
    class CUDAImage : public NIBR::Image<T>
    {
    public:

        CUDAImage<T>*   gpuClass;
        T*              cudaData;

        CUDAImage() : NIBR::Image<T>()                                                                {}
        CUDAImage(std::string _filePath) : NIBR::Image<T>(_filePath)                                  { init(); }
        CUDAImage(const char *_filePath) : NIBR::Image<T>(_filePath)                                  { init(); }
        CUDAImage(std::string _filePath, int *_indexOrder) : NIBR::Image<T>(_filePath, _indexOrder)   { init(); }
        CUDAImage(const char *_filePath, int *_indexOrder) : NIBR::Image<T>(_filePath, _indexOrder)   { init(); }
        
        ~CUDAImage() 
        {
            cudaFree(cudaData);
            cudaFree(gpuClass);
        }

        void init()
        {

            this->read();

            NIBR::CUDAHANDLER::CUDA_PRINT_ERROR(cudaMalloc((void **)&cudaData, sizeof(T)*this->numel));
            NIBR::CUDAHANDLER::CUDA_PRINT_ERROR(cudaMemcpy(cudaData, this->data, sizeof(T)*this->numel,cudaMemcpyHostToDevice));
            delete[] this->data;
            this->data = NULL;

            NIBR::CUDAHANDLER::CUDA_PRINT_ERROR(cudaMalloc((void **)&gpuClass,    sizeof(CUDAImage<T>)));
            NIBR::CUDAHANDLER::CUDA_PRINT_ERROR(cudaMemcpy(gpuClass, this, sizeof(CUDAImage<T>), cudaMemcpyHostToDevice));

        }

        void moveDataToHost()
        {
            this->data = new T[this->numel]();
            NIBR::CUDAHANDLER::CUDA_PRINT_ERROR(cudaMemcpy(this->data, cudaData, sizeof(T)*this->numel,cudaMemcpyDeviceToHost));
            cudaFree(cudaData);
        }

        __device__ T operator()(float* p) {return InterpolatorCUDA<T>::cuda_interp_linear_3D(this,p); };

    };


    template class CUDAImage<bool>;
    template class CUDAImage<uint8_t>;
    template class CUDAImage<int8_t>;
    template class CUDAImage<uint16_t>;
    template class CUDAImage<int16_t>;
    template class CUDAImage<uint32_t>;
    template class CUDAImage<int32_t>;
    template class CUDAImage<uint64_t>;
    template class CUDAImage<int64_t>;
    template class CUDAImage<float>;
    template class CUDAImage<double>;
    
}

#endif