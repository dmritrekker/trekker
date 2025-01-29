#pragma once

#include "base/nibr.h"
#include "dMRI/tractography/io/tractogramReader.h"
#include "streamline_operators.h"

namespace NIBR 
{

    std::vector<std::vector<float>> resampleStreamline_withStepSize (std::vector<std::vector<float>>& streamline, float step);
    std::vector<std::vector<float>> resampleStreamline_withStepCount(std::vector<std::vector<float>>& streamline, int N);

    std::vector<std::vector<float>> resampleStreamline_withStepSize (NIBR::TractogramReader* tractogram, int index, float step);
    std::vector<std::vector<float>> resampleStreamline_withStepCount(NIBR::TractogramReader* tractogram, int index, int N);

    std::vector<std::vector<std::vector<float>>> resampleTractogram_withStepSize(NIBR::TractogramReader* tractogram, float step);
    std::vector<std::vector<std::vector<float>>> resampleTractogram_withStepCount(NIBR::TractogramReader* tractogram, int N);

}