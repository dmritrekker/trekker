#pragma once

#include "base/nibr.h"
#include "dMRI/tractography/mappers/tractogram2imageMapper.h"

namespace NIBR 
{

    template<typename T>
    inline void allocateGrid_4mask(Tractogram2ImageMapper<T>* tim) {
        tim->template allocateGrid<bool>();
    }

    template<typename T>
    inline void deallocateGrid_4mask(Tractogram2ImageMapper<T>* tim) {
        tim->template deallocateGrid<bool>();
    }

    // Regular output
    template<typename T>
    inline void processor_4mask(Tractogram2ImageMapper<T>* tim, int* gridPos, NIBR::Segment&) {

        int64_t ind = tim->img->sub2ind(gridPos[0],gridPos[1],gridPos[2]);
        
        {
            std::lock_guard<std::mutex> lock(tim->gridMutex[ind]);
            
            if (tim->grid[ind]==NULL) {
                bool* val      = new bool;
                *val           = true;
                tim->grid[ind] = ((void*)val);
            }

        }

    }

    template<typename T>
    inline void outputCompiler_4mask(Tractogram2ImageMapper<T>* tim) {

        tim->img->allocData();

        auto genOut = [&](NIBR::MT::TASK task)->void{

            int64_t ind = task.no;

            if (tim->grid[ind]!=NULL) {
                tim->img->data[ind] = 1;
                delete ((bool*)(tim->grid[ind]));
                tim->grid[ind] = NULL;
            }

        };
        NIBR::MT::MTRUN(tim->img->voxCnt, genOut);

    }

    


}
