#pragma once

#include "base/nibr.h"
#include "image/image.h"
#include "image/image_math.h"
#include "image/image_morphological.h"
#include "surface/surface.h"
#include "surface/surface_operators.h"

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#endif

#include "marchingCubes/MarchingCubes.h"

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

namespace NIBR
{
    bool isosurface(Image<float>* img, float isoValue, Surface* surf);

    template<typename T1, typename T2, typename T3>
    Surface label2surface(Image<T1>& seg, T2 label, T3 meanFaceArea) {

        Image<float> inp;
        imgThresh(inp, seg, label, label);
        
        for (int n = 0; n < 2; n++) {
            imgDilate(inp,NIBR::CONN6);
            imgErode (inp,NIBR::CONN6);
        }

        Surface surf;
        if (!isosurface(&inp, 0.5, &surf)) {
            disp(MSG_ERROR, "Failed to generate surface");
            return Surface();
        }

        if (surf.nv > 0) surf = surfSmooth(surf,2);
        if (meanFaceArea != 0) {
            if (surf.nv > 0) surf.calcArea();
            if (surf.nv > 0) surf = surfRemesh(surf,surf.area/meanFaceArea*0.5f,1,0);
        }
        if (surf.nv > 0) surf = surfMakeItSingleClosed(surf);

        return surf;

    }
}


