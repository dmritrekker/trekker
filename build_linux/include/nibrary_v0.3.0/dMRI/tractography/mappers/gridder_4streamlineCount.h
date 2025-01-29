#pragma once

#include <cstdint>
#include <stdio.h>
#include "dMRI/tractography/mappers/tractogram2imageMapper.h"
#include "base/nibr.h"

namespace NIBR 
{
    template<typename T>
    inline void allocateGrid_4streamlineCount(Tractogram2ImageMapper<T>* tim) {
        tim->template allocateGrid<uint32_t>();
    }

    template<typename T>
    inline void deallocateGrid_4streamlineCount(Tractogram2ImageMapper<T>* tim) {
        tim->template deallocateGrid<uint32_t>();
    }

    template<typename T>
    inline void processor_4streamlineCount(Tractogram2ImageMapper<T>* tim, int* gridPos, NIBR::Segment&) {

        int64_t ind = tim->img->sub2ind(gridPos[0],gridPos[1],gridPos[2]);
        
        {
            std::lock_guard<std::mutex> lock(tim->gridMutex[ind]);
            
            if (tim->grid[ind]==NULL) {                    // Allocate memory here
                uint32_t* streamlineCount = new uint32_t;
                *streamlineCount          = 0;
                tim->grid[ind]            = ((void*)streamlineCount); // Each thread handles a single uint32_t for each voxel in the output
            }
            (*((uint32_t*)(tim->grid[ind])))++;            // Just increment the value of streamlineCount

        }

    }

    template<typename T>
    inline void outputCompiler_4streamlineCount(Tractogram2ImageMapper<T>* tim) {

        tim->img->allocData();

        auto genOut = [&](NIBR::MT::TASK task)->void{

            int64_t ind = task.no;

            if (tim->grid[ind]!=NULL) {
                tim->img->data[ind] += *((uint32_t*)(tim->grid[ind]));
                delete ((uint32_t*)(tim->grid[ind])); // Deallocate memory here
                tim->grid[ind] = NULL;
            }

        };
        NIBR::MT::MTRUN(tim->img->voxCnt, genOut);

    }

}
