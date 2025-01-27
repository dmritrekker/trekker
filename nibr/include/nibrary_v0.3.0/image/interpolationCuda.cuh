#pragma once

#ifdef HAVE_CUDA

#include "base/nibr.h"
#include "base/cudaHandler.cuh"

typedef enum {
    CUDA_INTERP_INSIDE,
    CUDA_INTERP_BOUNDARY,
    CUDA_INTERP_OUTSIDE
} CUDA_INTERPAT;

// ==== Initialization macro ==== //
#define init_cuda_interpAt(typeVal, p, xyz2ijk, imgDims, ijk, cor_ijk, cfs) \
    ijk[0] = xyz2ijk[0][0] * p[0] + xyz2ijk[0][1] * p[1] + xyz2ijk[0][2] * p[2] + xyz2ijk[0][3]; \
    ijk[1] = xyz2ijk[1][0] * p[0] + xyz2ijk[1][1] * p[1] + xyz2ijk[1][2] * p[2] + xyz2ijk[1][3]; \
    ijk[2] = xyz2ijk[2][0] * p[0] + xyz2ijk[2][1] * p[1] + xyz2ijk[2][2] * p[2] + xyz2ijk[2][3]; \
    cor_ijk[0]  = int64_t(ijk[0] + 1.0f); \
    cor_ijk[1]  = int64_t(ijk[1] + 1.0f); \
    cor_ijk[2]  = int64_t(ijk[2] + 1.0f); \
    cfs[0]      = cor_ijk[0] - ijk[0]; \
    cfs[1]      = cor_ijk[1] - ijk[1]; \
    cfs[2]      = cor_ijk[2] - ijk[2]; \
    if ( cor_ijk[0]>0 && cor_ijk[1]>0 && cor_ijk[2]>0 && cor_ijk[0]<imgDims[0] && cor_ijk[1]<imgDims[1] && cor_ijk[2]<imgDims[2] ) \
        typeVal = CUDA_INTERP_INSIDE; \
    else if ( cor_ijk[0]<0 || cor_ijk[1]<0 || cor_ijk[2]<0 || cor_ijk[0]>imgDims[0] || cor_ijk[1]>imgDims[1] || cor_ijk[2]>imgDims[2] ) \
        typeVal = CUDA_INTERP_OUTSIDE; \
    else { \
        if (cfs[0]>1) cfs[0] -= 1.0f; \
        if (cfs[1]>1) cfs[1] -= 1.0f; \
        if (cfs[2]>1) cfs[2] -= 1.0f; \
        if (cfs[0]<0) cfs[0] += 1.0f; \
        if (cfs[1]<0) cfs[1] += 1.0f; \
        if (cfs[2]<0) cfs[2] += 1.0f; \
        typeVal = CUDA_INTERP_BOUNDARY; \
    }
// ============================== //

namespace NIBR
{

    template <typename T>
    class CUDAImage;

    template <typename T>
    class InterpolatorCUDA
    {

    public:
        InterpolatorCUDA()  {};
        ~InterpolatorCUDA() {};

        static __device__ T cuda_interp_linear_3D(CUDAImage<T>* img, float* p);
    };


    template <typename OUT_T>
    __device__ OUT_T InterpolatorCUDA<OUT_T>::cuda_interp_linear_3D(CUDAImage<OUT_T>* img, float* p)
    {

        OUT_T    out        = 0;
        OUT_T*   data       = img->cudaData;
        int64_t* s2i        = img->s2i;
        int64_t* imgDims    = img->imgDims;

        
        float           ijk[3];
        float           cfs[3];
        int64_t         cor_ijk[3];
        CUDA_INTERPAT   type;
        init_cuda_interpAt(type, p, img->xyz2ijk, imgDims, ijk, cor_ijk, cfs);


        switch (type)
        {

        case CUDA_INTERP_INSIDE:
            out = cfs[0] * cfs[1] * cfs[2] * OUT_T(data[(cor_ijk[0] - 1) * s2i[0] + (cor_ijk[1] - 1) * s2i[1] + (cor_ijk[2] - 1) * s2i[2]]) +
                        (1 - cfs[0]) * cfs[1] * cfs[2] * OUT_T(data[(cor_ijk[0]) * s2i[0] + (cor_ijk[1] - 1) * s2i[1] + (cor_ijk[2] - 1) * s2i[2]]) +
                        cfs[0] * (1 - cfs[1]) * cfs[2] * OUT_T(data[(cor_ijk[0] - 1) * s2i[0] + (cor_ijk[1]) * s2i[1] + (cor_ijk[2] - 1) * s2i[2]]) +
                        (1 - cfs[0]) * (1 - cfs[1]) * cfs[2] * OUT_T(data[(cor_ijk[0]) * s2i[0] + (cor_ijk[1]) * s2i[1] + (cor_ijk[2] - 1) * s2i[2]]) +
                        cfs[0] * cfs[1] * (1 - cfs[2]) * OUT_T(data[(cor_ijk[0] - 1) * s2i[0] + (cor_ijk[1] - 1) * s2i[1] + (cor_ijk[2]) * s2i[2]]) +
                        (1 - cfs[0]) * cfs[1] * (1 - cfs[2]) * OUT_T(data[(cor_ijk[0]) * s2i[0] + (cor_ijk[1] - 1) * s2i[1] + (cor_ijk[2]) * s2i[2]]) +
                        cfs[0] * (1 - cfs[1]) * (1 - cfs[2]) * OUT_T(data[(cor_ijk[0] - 1) * s2i[0] + (cor_ijk[1]) * s2i[1] + (cor_ijk[2]) * s2i[2]]) +
                        (1 - cfs[0]) * (1 - cfs[1]) * (1 - cfs[2]) * OUT_T(data[(cor_ijk[0]) * s2i[0] + (cor_ijk[1]) * s2i[1] + (cor_ijk[2]) * s2i[2]]);

            break;
        case CUDA_INTERP_BOUNDARY:
        {

            if (cor_ijk[0] > 0 && cor_ijk[1] > 0 && cor_ijk[2] > 0)
                out += cfs[0] * cfs[1] * cfs[2] * OUT_T(data[(cor_ijk[0] - 1) * s2i[0] + (cor_ijk[1] - 1) * s2i[1] + (cor_ijk[2] - 1) * s2i[2]]);

            if (cor_ijk[0] < imgDims[0] && cor_ijk[1] > 0 && cor_ijk[2] > 0)
                out += (1 - cfs[0]) * cfs[1] * cfs[2] * OUT_T(data[(cor_ijk[0]) * s2i[0] + (cor_ijk[1] - 1) * s2i[1] + (cor_ijk[2] - 1) * s2i[2]]);

            if (cor_ijk[0] > 0 && cor_ijk[1] < imgDims[1] && cor_ijk[2] > 0)
                out += cfs[0] * (1 - cfs[1]) * cfs[2] * OUT_T(data[(cor_ijk[0] - 1) * s2i[0] + (cor_ijk[1]) * s2i[1] + (cor_ijk[2] - 1) * s2i[2]]);

            if (cor_ijk[0] < imgDims[0] && cor_ijk[1] < imgDims[1] && cor_ijk[2] > 0)
                out += (1 - cfs[0]) * (1 - cfs[1]) * cfs[2] * OUT_T(data[(cor_ijk[0]) * s2i[0] + (cor_ijk[1]) * s2i[1] + (cor_ijk[2] - 1) * s2i[2]]);

            if (cor_ijk[0] > 0 && cor_ijk[1] > 0 && cor_ijk[2] < imgDims[2])
                out += cfs[0] * cfs[1] * (1 - cfs[2]) * OUT_T(data[(cor_ijk[0] - 1) * s2i[0] + (cor_ijk[1] - 1) * s2i[1] + (cor_ijk[2]) * s2i[2]]);

            if (cor_ijk[0] < imgDims[0] && cor_ijk[1] > 0 && cor_ijk[2] < imgDims[2])
                out += (1 - cfs[0]) * cfs[1] * (1 - cfs[2]) * OUT_T(data[(cor_ijk[0]) * s2i[0] + (cor_ijk[1] - 1) * s2i[1] + (cor_ijk[2]) * s2i[2]]);

            if (cor_ijk[0] > 0 && cor_ijk[1] < imgDims[1] && cor_ijk[2] < imgDims[2])
                out += cfs[0] * (1 - cfs[1]) * (1 - cfs[2]) * OUT_T(data[(cor_ijk[0] - 1) * s2i[0] + (cor_ijk[1]) * s2i[1] + (cor_ijk[2]) * s2i[2]]);

            if (cor_ijk[0] < imgDims[0] && cor_ijk[1] < imgDims[1] && cor_ijk[2] < imgDims[2])
                out += (1 - cfs[0]) * (1 - cfs[1]) * (1 - cfs[2]) * OUT_T(data[(cor_ijk[0]) * s2i[0] + (cor_ijk[1]) * s2i[1] + (cor_ijk[2]) * s2i[2]]);

            break;
        }

        default: // OUTSIDE
            out = img->outsideVal;
            break;
        }

        return out;

    }

}

#endif
