#pragma once

#include "base/nibr.h"
#include "pathway.h"
#include "pathwayRule.h"


// Handles following cases, otherwise returns false.

// <rule> 1.2,2.4,33,2,4                    -> sphere

// <rule> img.nii.gz                        -> if image is integer  type, then it is considered as mask that is created by thresholding values above zero. Nearest neighbor interpolation is used.
// <rule> img.nii.gz                        -> if image is floating type, then it is considered as partial volume fraction. A value above zero is considered inside. Linear interpolation is used.
// <rule> img.nii.gz label                  -> a mask is created by thresholding values above zero. Nearest neighbor interpolation is used.
// <rule> img.nii.gz pvf                    -> image is considered as partial volume fraction. A value above zero is considered inside. Linear interpolation is used.
// <rule> img.nii.gz label 4                -> a mask is created using the provided label. Nearest neighbor interpolation is used.
// <rule> img.nii.gz pvf 4                  -> image is considered as partial volume fraction, and 4 is considered as volume index. A value above zero is considered inside. Linear interpolation is used.

// <rule> surf.vtk                          -> if surface is closed then the rule includes the interior of the surface, otherwise only the surface is used
// <rule> surf.vtk 1.2,2.4,33,2,4           -> a disk is extract and used as an open surface
// <rule> surf.vtk maskField 3              -> A mask is created using label 3 of SurfaceField maskField
// <rule> surf.vtk maskFile VERT int 3      -> A mask is created using label 3 from a maskFile for VERTices that uses int datatype

// <rule> surf.vtk 0.5                      -> if surface is closed then the rule includes the interior of the surface, otherwise only the surface is used
// <rule> surf.vtk 0.5 1.2,2.4,33,2,4       -> a disk is extract and used as an open surface
// <rule> surf.vtk 0.5 maskField 3          -> A mask is created using label 3 of SurfaceField maskField
// <rule> surf.vtk 0.5 maskFile VERT int 3  -> A mask is created using label 3 from a maskFile for VERTices that uses int datatype

namespace NIBR {

std::vector<PathwayRule> parsePathwayInput(std::vector<std::string> inp);
PathwayRule              parseSeedInput   (std::vector<std::string> inp);

}
