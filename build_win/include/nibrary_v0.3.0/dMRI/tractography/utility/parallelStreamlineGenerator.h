#pragma once

#include "base/nibr.h"
#include "dMRI/tractography/io/tractogramReader.h"

namespace NIBR 
{

    // TODO: This functions saves the parallel streamlines in the memory right now. This should be improved depending on the application.
    //
    // Compute parallel streamlines around a streamline
    // radius:    the outermost radius where streamlines will be generated
    // ringCount: inner ring count
    // pointCountPerRing: number of streamlines to be generated per ring
    // Explanation:
    //     if radius=1, ringCount=2, pointCountPerRing=5, then:
    //     at the center will be the input streamline.
    //     5 streamlines will be located on a ring at radius=0.5 
    //     5 more streamlines will be located on a ring at radius=1
    //     there will be a total of 11 streamlines at the output 
    void getParallelStreamlines(std::vector<std::vector<std::vector<float>>>& out, NIBR::TractogramReader* _tractogram, float radius, int ringCount, int pointCountPerRing);

    void getParallelStreamlines(std::vector<std::vector<std::vector<float>>>& out, NIBR::TractogramReader* _tractogram, float sigma, int N);

}
