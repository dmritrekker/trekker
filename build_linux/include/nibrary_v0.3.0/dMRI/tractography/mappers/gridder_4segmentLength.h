#pragma once

#include <stdio.h>
#include <map>
#include "base/nibr.h"
#include "math/sphericalFunctions.h"
#include "dMRI/tractography/mappers/tractogram2imageMapper.h"

using namespace NIBR::SF;

namespace NIBR 
{

    template<typename T>
    inline void allocateGrid_4segmentLength(Tractogram2ImageMapper<T>* tim) {
        tim->template allocateGrid<float>();
    }

    template<typename T>
    inline void deallocateGrid_4segmentLength(Tractogram2ImageMapper<T>* tim) {
        tim->template deallocateGrid<float>();
    }

    // Regular output
    template<typename T>
    inline void processor_4segmentLength(Tractogram2ImageMapper<T>* tim, int* gridPos, NIBR::Segment& seg) {

        int64_t ind = tim->img->sub2ind(gridPos[0],gridPos[1],gridPos[2]);
        
        {
            std::lock_guard<std::mutex> lock(tim->gridMutex[ind]);
            
            if (tim->grid[ind]==NULL) {                    // Allocate memory here
                float* segmentLength    = new float;
                *segmentLength          = 0;
                tim->grid[ind]          = ((void*)segmentLength);   // Each thread handles a single float for each voxel in the output
            }
            *((float*)(tim->grid[ind])) += seg.length;     // Add to the segment length

        }

    }

    // Weighted output
    template<typename T>
    inline void processor_4segmentLength_weighted(Tractogram2ImageMapper<T>* tim, int* gridPos, NIBR::Segment& seg) {

        int64_t ind = tim->img->sub2ind(gridPos[0],gridPos[1],gridPos[2]);

        {
            std::lock_guard<std::mutex> lock(tim->gridMutex[ind]);

            if (tim->grid[ind]==NULL) {                    // Allocate memory here
                float* segmentLength    = new float;
                *segmentLength          = 0;
                tim->grid[ind]          = ((void*)segmentLength);   // Each thread handles a single float for each voxel in the output
            }

            *((float*)(tim->grid[ind])) += seg.length*(*(float*)(seg.data));     // Add to weighted segment length

        }
    }

    template<typename T>
    inline void outputCompiler_4segmentLength(Tractogram2ImageMapper<T>* tim) {

        tim->img->allocData();

        auto genOut = [&](NIBR::MT::TASK task)->void{

            int64_t ind = task.no;

            if (tim->grid[ind]!=NULL) {
                tim->img->data[ind] += *((float*)(tim->grid[ind]));
                delete ((float*)(tim->grid[ind])); // Deallocate memory here
                tim->grid[ind] = NULL;
            }

        };
        NIBR::MT::MTRUN(tim->img->voxCnt, genOut);

    }

    // Spherical output

    template<typename T>
    inline void allocateGrid_4segmentLength_sf(Tractogram2ImageMapper<T>* tim) {
        tim->template allocateGrid<std::map<int64_t,float>>();
    }

    template<typename T>
    inline void deallocateGrid_4segmentLength_sf(Tractogram2ImageMapper<T>* tim) {
        tim->template deallocateGrid<std::map<int64_t,float>>();
    }

    template<typename T>
    inline void processor_4segmentLength_sf(Tractogram2ImageMapper<T>* tim, int* gridPos, NIBR::Segment& seg) {

        if (seg.length <= 0) return;

        int64_t ind = tim->img->sub2ind(gridPos[0],gridPos[1],gridPos[2]);

        {
            std::lock_guard<std::mutex> lock(tim->gridMutex[ind]);

            if (tim->grid[ind]==NULL) {
                std::map<int64_t,float>* dirLength = new std::map<int64_t,float>();
                tim->grid[ind] = ((void*)dirLength);   // Allocate memory here
            }            
            (*((std::map<int64_t,float>*)(tim->grid[ind])))[SF::coordinate2index(seg.dir)] += seg.length;

        }

    }

    template<typename T>
    inline void outputCompiler_4segmentLength_sf(Tractogram2ImageMapper<T>* tim) {

        tim->img->allocData();

        auto genOut = [&](NIBR::MT::TASK task)->void{

            int64_t ind = task.no;

            int64_t i,j,k;

            tim->img->ind2sub(ind,i,j,k);

            if (tim->grid[ind]!=NULL) {
                
                for (auto lenSf : (*((std::map<int64_t,float>*)(tim->grid[ind])))) {
                    auto index = tim->img->sub2ind(i,j,k,lenSf.first);
                    tim->img->data[index] += lenSf.second;
                }

                delete ((std::map<int64_t,float>*)(tim->grid[ind])); // Deallocate memory here
                tim->grid[ind] = NULL;
            }

        };
        NIBR::MT::MTRUN(tim->img->voxCnt, "Writing img", genOut);

    }


}
