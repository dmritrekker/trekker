#pragma once

#include "base/nibr.h"
#include <stdio.h>
#include "dMRI/tractography/mappers/tractogram2imageMapper.h"

namespace NIBR 
{

    struct decLength {
        float x{0};
        float y{0};
        float z{0};
    };

    template<typename T>
    inline void allocateGrid_4DECsegmentLength(Tractogram2ImageMapper<T>* tim) {
        tim->template allocateGrid<decLength>();
    }

    template<typename T>
    inline void deallocateGrid_4DECsegmentLength(Tractogram2ImageMapper<T>* tim) {
        tim->template deallocateGrid<decLength>();
    }

    template<typename T>
    inline void processor_4DECsegmentLength(Tractogram2ImageMapper<T>* tim, int* gridPos, NIBR::Segment& seg) {

        int64_t ind = tim->img->sub2ind(gridPos[0],gridPos[1],gridPos[2]);

        {
            std::lock_guard<std::mutex> lock(tim->gridMutex[ind]);

            if (tim->grid[ind]==NULL) {
                decLength* dec  = new decLength;
                tim->grid[ind]  = ((void*)dec);                     // Allocate memory here
            }

            ((decLength*)(tim->grid[ind]))->x   += std::fabs(seg.dir[0])*seg.length;
            ((decLength*)(tim->grid[ind]))->y   += std::fabs(seg.dir[1])*seg.length;
            ((decLength*)(tim->grid[ind]))->z   += std::fabs(seg.dir[2])*seg.length;

        }
    }

    template<typename T>
    inline void outputCompiler_4DECsegmentLength(Tractogram2ImageMapper<T>* tim) {

        // Set index order for image such that the 4th, i.e. the DEC dimension, runs first
        tim->img->indexOrder[0] = 3;
        tim->img->indexOrder[1] = 0;
        tim->img->indexOrder[2] = 1;
        tim->img->indexOrder[3] = 2;
        tim->img->indexOrder[4] = 4;
        tim->img->indexOrder[5] = 5;
        tim->img->indexOrder[6] = 6;
        tim->img->allocData();


        auto genOut = [&](NIBR::MT::TASK task)->void{

            int64_t ind    = task.no;
            int64_t sub[7] = {0,0,0,0,0,0,0};

            int64_t i,j,k,sind;

            tim->img->ind2sub(ind,i,j,k);

            if (tim->grid[ind]!=NULL) {
                
                sub[0] = i;
                sub[1] = j;
                sub[2] = k;
                sind   = tim->img->sub2ind(sub);

                tim->img->data[sind]   += ((decLength*)(tim->grid[ind]))->x;
                tim->img->data[sind+1] += ((decLength*)(tim->grid[ind]))->y;
                tim->img->data[sind+2] += ((decLength*)(tim->grid[ind]))->z;

                delete ((decLength*)(tim->grid[ind])); // Deallocate memory here
                tim->grid[ind] = NULL;
            }

        };
        NIBR::MT::MTRUN(tim->img->voxCnt, genOut);

    }

}
