#pragma once

#include "base/nibr.h"
#include "surface/surface.h"
#include "dMRI/tractography/io/tractogramReader.h"

namespace NIBR {

    // Find intersection of a line segment with a triangle using Möller–Trumbore algorithm
    // This functions returns the angle of intersection in degrees [0,90]
    // 0 means there is no intersection.

    // In its most general form. The following takes a surface and index of a face.
    // Given a point (p), direction (dir), length -> the function finds the intersection position on the triangle (pos) and its distance (dist) to p.
    // The function returns 0 if there is no intersection.
    double findSegmentTriangleIntersection(NIBR::Surface* inpSurf, int faceIndex, double* p, double* dir, double length, double* pos, double* dist);
    double findSegmentTriangleIntersection(NIBR::Surface* inpSurf, int faceIndex, float* beg, float* end, double* pos, double* dist);

}