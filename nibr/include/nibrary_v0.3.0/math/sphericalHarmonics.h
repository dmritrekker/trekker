#pragma once

#include "base/nibr.h"
#include "math/core.h"
#include "math/reorient.h"

namespace NIBR 
{

    int getNumberOfSHCoeffs(int order);
    int getSHOrderFromNumberOfCoeffs(int numberOfCoefficients);

    // Input:
    //  inpCoords: Nx3 list of coordinates on a unit sphere.
    //  order: spherical harmonics order
    // Output:
    //  B: NxM basis coefficients
    void  SH_basis(std::vector<std::vector<float>>& B, std::vector<std::vector<float>>& inpCoords, int order);

    // TODO: Below should be a separate class.
    namespace SH 
    {
        int   getNumberOfSHCoeffs();
        void  precompute(int _sphericalHarmonicOrder, OrderOfDirections _orderOfDirs, size_t _num);
        void  clean();
        float toSF(float *sh, float *dir);        
    }

}
