#pragma once

#include "base/nibr.h"
#include "dMRI/tractography/io/tractogramReader.h"
#include "dMRI/tractography/pathway/pathway.h"
#include "dMRI/tractography/io/tractogramWriter.h"


namespace NIBR 
{
    
    // Compute bounding box of a tractogram
    std::vector<float> getTractogramBBox(NIBR::TractogramReader* tractogram);

    // tuple<diffStreamlineIdx,sameStreamlineIdx>
    std::tuple<std::vector<size_t>,std::vector<size_t>> tractogramDiff(NIBR::TractogramReader* inp_tractogram, NIBR::TractogramReader* ref_tractogram);

    // Extends inp1 by appending the streamlines in inp2, optionally checks and removes duplicates
    std::vector<std::vector<std::vector<float>>> tractogramMerge(NIBR::TractogramReader* inp1, NIBR::TractogramReader* inp2, bool checkDuplicates);

    std::vector<std::vector<std::vector<float>>> applyTransform(NIBR::TractogramReader* tractogram, float M[][4]);

    TractogramField colorTractogram(NIBR::TractogramReader* tractogram);

}

