#pragma once

#include "base/nibr.h"
#include "math/core.h"
#include "image/image.h"
#include "image/image_morphological.h"

#define FDM_EXTERIOR  std::numeric_limits<double>::max()
#define FDM_INTERIOR  std::numeric_limits<double>::min()

namespace NIBR 
{

// Solves the Laplacian equation with Dirichlet boundary conditions (BC) using finite difference method (FDM).
// The input problem is defined as a 3D rectilinear image made of voxels with isotropic dimensions.
// The FDM_EXTERIOR values in the problem mark the region where the problem is not going to be solved, and the output solution value will be NAN.
// The FDM_INTERIOR values in the problem mark the region where the problem will be solved, and the output will be proved in the solution.
// The values in the problem, that are neither FDM_EXTERIOR nor FDM_INTERIOR, are the Dirichlet BC, and the successful solution will return the same values in the output.
bool FDMsolveLaplacianWithDirichletBC(Image<double>& problem, Image<double>& solution);

}
