#pragma once

#include "base/nibr.h"
#include "base/stringOperations.h"
#include <fstream>

namespace NIBR
{

    // Reads bvals and bvecs from text files. Also checks whether number of elements match. Returns true if everything is OK.
    std::tuple<bool,std::vector<std::vector<float>> > readGradTable(std::string bvals_fname, std::string bvecs_fname);

    // Rotates gradTable
    void rotateGradTable(std::vector<std::vector<float>>* gradTable, float R[][4]);

    // Reads bvalues
    std::vector<float> readBvalues(std::string bvals_fname, bool* success);

    // Reads bvectors
    std::vector<std::vector<float>> readBvectors(std::string bvals_fname, bool* success);

}
