#pragma once

#include "base/nibr.h"
#include "dMRI/tractography/io/tractogramReader.h"
#include "surface/surface.h"

namespace NIBR
{
    // Output size is of length streamlineIndices.size(). Each output element contains streamline ids, and their corresponding vertex_ids and streamline distances on the mesh
    std::vector<std::unordered_map<int,float>> index2surface(TractogramReader& tractogram, Surface& surf, float sigma);
    
    void index2surface(TractogramReader& tractogram, Surface& surf, float sigma, std::string indexFilePrefix);

    // Output size is of length streamlineIndices.size(). Each output element contains streamline ids, and their corresponding vertex_ids and streamline distances on the mesh
    std::unordered_map<int,std::unordered_map<int, float>> readSurfaceIndexing(const std::vector<int>& streamlineIndices, const std::string& indexFilePrefix, const std::vector<std::streampos>& positions);

    // Output size is of length numberOfStreamlines. Each output element contains corresponding vertex_ids and streamline distances on the mesh
    std::vector<std::unordered_map<int,float>> readSurfaceIndexing(TractogramReader& tractogram, std::string& indexFilePrefix);

    // Output has pairs of vertex_ids and cumulative streamline distance
    std::unordered_map<int,float> continuousSurfaceMap(const std::vector<int>& streamlineIndices, const std::string& indexFilePrefix);
    std::unordered_map<int,float> continuousSurfaceMap(const std::vector<int>& streamlineIndices, const std::string& indexFilePrefix, const std::vector<std::streampos>& positions);

}

