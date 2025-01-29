#pragma once

#include "base/nibr.h"
#include "surface/surface.h"

using namespace NIBR;

namespace NIBR {    

    // Functions using geogram
    Surface surfRepair(const Surface& surf);
    Surface surfRemoveSmallFaces(const Surface& surf,double minArea);
    Surface surfRemoveSmallConnectedComponents(const Surface& surf,double minArea);
    Surface surfFillHoles(const Surface& surf,double maxArea);
    Surface surfSmooth(const Surface& surf,int smoothing_iterations = 1);
    Surface surfRemesh(const Surface& surf,int newVertexCount, int smoothing_iterations = 1, float anisotropy = 0);
    std::vector<double> surfCalcAreasOfHoles(const Surface& surf);

    // Other functions
    Surface surfRemoveVerticesWithNAN(Surface& surf);
    Surface surfRemoveSingularVertices(Surface& surf);
    Surface surfRemoveOverconnectedVertices(Surface& surf);
    Surface surfRemoveBadBoundaryVertices(Surface& surf);
    Surface surfRemoveAllBoundaryVertices(Surface& surf);

    Surface surfMerge(const Surface& s1, const Surface& s2);
    Surface surfDiff(const Surface& s1, const Surface& s2);
    Surface surfGlueBoundaries(const Surface& s1, const Surface& s2); // Glues boundaries of two open surfaces so they become a closed watertight surface

    Surface surfRemoveAllButLargestComponent(const Surface& surf);
    Surface surfFillAllButLargestHole(const Surface& surf);

    Surface surfMakeItSingleOpen(const Surface& surf);
    Surface surfMakeItWatertight(Surface& surf);
    Surface surfMakeItSingleClosed(const Surface& surf);

    Surface surfMoveVerticesAlongNormal(const Surface& surf, float shift);
    Surface meanCurvatureFlow(const Surface& surf, float dt, int iterationCount);

    std::vector<float> surfBbox(const Surface& surf);
    
    Surface surfGrow(const Surface& surf, float discretizationResolution, float shift);


    // closed and open parts of a surface
    std::tuple<Surface,Surface> splitClosedAndOpenParts(Surface& surf);

    // Front of plane, behind the plane surface
    std::tuple<Surface,Surface> splitWithPlane(const Surface& surf, const float planePoint[3], const float planeNormal[3]);

    void surfaceFieldThreshold(Surface *surf, SurfaceField* field, int   loVal, int   hiVal);
    void surfaceFieldThreshold(Surface *surf, SurfaceField* field, float loVal, float hiVal);

    SurfaceField makeDiscMask(Surface* surf, float x, float y, float z, float r);
    
    void extractDisc(Surface* outSurf, Surface* surf, float x, float y, float z, float r);

    void removeVertices(Surface* outSurf, Surface* surf, SurfaceField* mask);
    void removeVertices(Surface* outSurf, Surface* surf, SurfaceField* mask, int label);

    void selectVertices(Surface* outSurf, Surface* surf, SurfaceField* mask);
    void selectVertices(Surface* outSurf, Surface* surf, SurfaceField* mask, int label);

    bool shiftVerticesBetweenSurfaces(Surface* out, Surface* s1, Surface* s2, float shift);
    
    SurfaceField convert2FaceField(Surface* surf, SurfaceField* field);
    SurfaceField convert2VertField(Surface* surf, SurfaceField* field);

    Surface applyMask(const Surface& surf, std::vector<bool>& mask);

    // Returns the index of neighboring vertices and the distance between the vertex and the point (which lays inside face with faceNo)
    std::unordered_map<int,float> getVertexNeigborhoodOfAPoint(Surface* surf, float* p, int faceNo, float radius);




    template <typename T>
    void surfBbox2Img(Image<T>& img,const Surface& surf,float voxDim)
    {

        Image<T> tmpImg;

        auto bbox = NIBR::surfBbox(surf);

        if (bbox.empty()) {
            disp(MSG_DETAIL,"Empty bounding box.");
        }

        if (voxDim <= 0)
            voxDim = std::pow((bbox[1] - bbox[0]) * (bbox[3] - bbox[2]) * (bbox[5] - bbox[4]) / 1000000.0f, 0.333333);

        bbox[0] -= 1.51 * voxDim;
        bbox[1] += 1.51 * voxDim;
        bbox[2] -= 1.51 * voxDim;
        bbox[3] += 1.51 * voxDim;
        bbox[4] -= 1.51 * voxDim;
        bbox[5] += 1.51 * voxDim;

        tmpImg.createFromBoundingBox(3, bbox, voxDim, false);

        img.deallocData();
        img.createFromTemplate(tmpImg,true);

    }

}
