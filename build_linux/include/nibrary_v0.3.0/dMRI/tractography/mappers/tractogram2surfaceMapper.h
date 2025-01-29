#pragma once

#include "base/nibr.h"
#include "dMRI/tractography/io/tractogramReader.h"
#include "surface/surface2imageMapper.h"
#include "surface/findSegmentTriangleIntersection.h"
#include "dMRI/tractography/mappers/tractogram2imageMapper.h"

namespace NIBR
{

    struct streamline2faceMap {
        int   index;
        float p[3];
        float dir[3];
        float angle;
    };

    void tractogram2surfaceMapper(NIBR::TractogramReader* _tractogram, NIBR::Surface* surf, std::vector<std::vector<streamline2faceMap>>& mapping, bool mapOnce);

}

