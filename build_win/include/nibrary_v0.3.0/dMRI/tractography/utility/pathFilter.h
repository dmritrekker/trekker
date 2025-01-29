#pragma once

#include "base/nibr.h"
#include "dMRI/tractography/io/tractogramReader.h"
#include "dMRI/tractography/pathway/pathway.h"
#include "dMRI/tractography/io/tractogramWriter.h"

namespace NIBR 
{
    // Returns streamline indx, streamline beg, streamline end
    std::tuple<std::vector<size_t>,std::vector<float>,std::vector<float>> pathFilter(NIBR::TractogramReader* tractogram, Pathway* pw, int numberOfThreads, int stopLim);
}