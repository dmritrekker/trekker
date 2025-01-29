#pragma once

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstdint>

#include "base/nibr.h"
#include "surface/surface.h"
#include "surface/surface_operators.h"
#include "image/image_transform.h"
#include "math/core.h"
#include "math/sphere.h"

namespace NIBR 
{

    typedef enum {
        MASK,                           // INSIDE, OUTSIDE (voxels centers that are inside the mesh are 1)
        ONLY_BOUNDARY,                  // BOUNDARY only (voxels that intersect any triangle are 1)
        MASK_WITH_BOUNDARY,             // INSIDE, OUTSIDE, BOUNDARY
        PARTIAL_VOLUME,                 // Partial volumes
        FIELD_LABEL,                    // Face labels provided with a surface field input
        EDT,                            // Euclidean distance transform
        MAT,                            // Medial axis transform
        MIS                             // Maximal inscribed sphere (radius)
    } VOXELIZE_MODE;

    template<typename T>
    bool mapSurface2Image(NIBR::Surface* surf, NIBR::Image<T>* img, float voxDim, NIBR::SurfaceField* sf, std::vector<std::vector<std::vector<std::vector<int>>>>* faceGrid, VOXELIZE_MODE mode);

    void surfaceMask(NIBR::Image<bool>* img, NIBR::Surface* surf);
    void surfaceMaskWithBoundary(NIBR::Image<int8_t>* img, NIBR::Surface* surf);
    void surfacePVF(NIBR::Image<float>* img, NIBR::Surface* surf);
    void surfaceEDT(NIBR::Image<float>* img, NIBR::Surface* surf);
    void surfaceMAT(NIBR::Image<float>* img, NIBR::Surface* surf);
    void surfaceMIS(NIBR::Image<float>* img, NIBR::Surface* surf);
    

    template<typename T>
    bool*** indexSurfaceBoundary(NIBR::Surface* surf, NIBR::Image<T>* img, std::vector<std::vector<std::vector<std::vector<int>>>>* faceGrid, bool markFaceInd);

    // grid[i][j][k] has the faceIds that intersect with the voxel at [i][j][k] (in image space)
    template<typename T>
    void faceGridder(NIBR::Surface* surf, NIBR::Image<T>* img, std::vector<std::vector<std::vector<std::vector<int>>>>* grid, bool markFaceInd) {

        disp(MSG_DEBUG,"faceGridder()");

        float t[3][3];
        int   hullMin[3];
        int   hullMax[3];
        int   voxelInds[3];
        float min;
        float max;

        if (grid!=NULL) {
            grid->resize(img->imgDims[0]);
            for (int i = 0; i < img->imgDims[0]; i++) {
                grid->at(i).resize(img->imgDims[1]);
                for (int j = 0; j < img->imgDims[1]; j++) {
                    grid->at(i)[j].resize(img->imgDims[2]);
                }
            }
        }

        if (surf->centersOfFaces==NULL) surf->calcCentersOfFaces();
        if (surf->normalsOfFaces==NULL) surf->calcNormalsOfFaces();
        if (surf->triangleEdge1 ==NULL) surf->calcTriangleVectors();

        for (int n=0; n<surf->nf; n++) {
        
            img->to_ijk(surf->vertices[surf->faces[n][0]],t[0]);
            img->to_ijk(surf->vertices[surf->faces[n][1]],t[1]);
            img->to_ijk(surf->vertices[surf->faces[n][2]],t[2]);
            
            for (int k=0; k<3; k++) {

                min =  INFINITY;
                max = -INFINITY;

                for (int i=0; i<3; i++) {

                    if (t[i][k]<min) {
                        min         = t[i][k];
                        hullMin[k]  = std::round(min) - 1;
                    }

                    if (t[i][k]>max) {
                        max         = t[i][k];
                        hullMax[k]  = std::round(max) + 1;
                    }

                }

            }

            for (int i=hullMin[0]; i<=hullMax[0]; i++)
                for (int j=hullMin[1]; j<=hullMax[1]; j++)
                    for (int k=hullMin[2]; k<=hullMax[2]; k++) {
                        
                        voxelInds[0] = i;
                        voxelInds[1] = j;
                        voxelInds[2] = k;
                            
                        if ( img->isInside(voxelInds) && triangleLargerVoxelIntersection(voxelInds, t[0], t[1], t[2]) ) {

                            if (grid!=NULL) {
                                (*grid)[voxelInds[0]][voxelInds[1]][voxelInds[2]].push_back(n);
                            }
                            
                            int64_t ind = img->sub2ind(i,j,k);
                            if (markFaceInd) {
                                if (img->data[ind]<(n+1)) 
                                    img->data[ind] = n + 1; // So that the value is not mixed with OUTSIDE
                            } else {
                                img->data[ind] = 1;
                            }

                        }

                    }
        }

        disp(MSG_DEBUG,"Done faceGridder()");
        
    }

    // out[i][j][k] has the faceIds that intersect with the voxel at [i][j][k] (in image space)
    template <typename T>
    bool*** indexSurfaceBoundary(NIBR::Surface* surf, NIBR::Image<T>* img, std::vector<std::vector<std::vector<std::vector<int>>>>* faceGrid, bool markFaceInd) 
    {

        if (img->numberOfDimensions==0)
            surfBbox2Img(*img,*surf,1);
        
        if (img->data == NULL) img->allocData();

        // Prepare surfaceGrid and processing mask
        std::vector<std::vector<std::vector<std::vector<int>>>> grid;
        faceGridder(surf, img, &grid, markFaceInd);
        disp(MSG_DETAIL,"Face grid computed.");

        // Create mask
        bool*** mask = new bool**[img->imgDims[0]];
        for (int i = 0; i < img->imgDims[0]; i++) {
            mask[i] = new bool*[img->imgDims[1]];
            for (int j = 0; j < img->imgDims[1]; j++) {
                mask[i][j] = new bool[img->imgDims[2]];
            }
        }

        auto createMask = [&](NIBR::MT::TASK task)->void {
            int64_t i,j,k;
            img->ind2sub(int64_t(task.no),i,j,k);
            mask[i][j][k] = (!grid[i][j][k].empty()) ? true : false;
        };
        NIBR::MT::MTRUN(img->voxCnt, NIBR::MT::MAXNUMBEROFTHREADS(), createMask);

        std::swap(*faceGrid,grid);

        return mask;

    }

    template<typename T>
    bool prepVoxelization(
        NIBR::Surface* surf, 
        NIBR::Image<T>* img, 
        float voxDim,
        NIBR::SurfaceField* sf,
        VOXELIZE_MODE mode) 
    {

        disp(MSG_DEBUG,"prepVoxelization()");

        // Do checks
        if ((mode==FIELD_LABEL) && (sf==NULL)) {
            disp(MSG_ERROR,"Voxelization requires surface field to map face labels.");
            return false;
        }

        // Read mesh and prepare output image
        if (surf->vertices==NULL)  surf->readMesh();
        if (surf->nv == 0)         {disp(MSG_DEBUG,"Done prepVoxelization()"); return true;}

        if ((mode==PARTIAL_VOLUME) && (surf->openOrClosed == OPEN )) {
            disp(MSG_WARN,"Partial volumes for open surfaces are always 0.");
        }

        if (img->numberOfDimensions==0) {
            if (voxDim==0)
                voxDim = 1;
            surfBbox2Img(*img,*surf,voxDim);
        }

        if ( (std::fabs(img->pixDims[0]-img->pixDims[1])>EPS6) || (std::fabs(img->pixDims[0]-img->pixDims[2])>EPS6) || (std::fabs(img->pixDims[1]-img->pixDims[2])>EPS6) ) {
            disp(MSG_ERROR,"Voxelization requires template image to have isotropic voxel dimensions, differences [%.8f, %.8f, %.8f].", std::fabs(img->pixDims[0]-img->pixDims[1]), std::fabs(img->pixDims[0]-img->pixDims[2]), std::fabs(img->pixDims[1]-img->pixDims[2]));
            return false;
        }

        if (img->data == NULL) img->allocData();

        disp(MSG_DEBUG,"Done prepVoxelization()");
        return true;
    }

    // ONLY_BOUNDARY        - Image<bool>
    // MASK                 - Image<bool>
    // MASK_WITH_BOUNDARY   - Image<int8_t>
    // FIELD_LABEL          - Image<int>
    // PARTIAL_VOLUME       - Image<float>
    // EDT                  - Image<float>
    // MAT                  - Image<float>
    // MIS                  - Image<float>
    template<typename T>
    bool mapSurface2Image(
        NIBR::Surface* surf, 
        NIBR::Image<T>* img, 
        float voxDim,
        NIBR::SurfaceField* sf, 
        std::vector<std::vector<std::vector<std::vector<int>>>>* faceGrid, 
        VOXELIZE_MODE mode) 
    {
        if (!prepVoxelization(surf,img,voxDim,sf,mode)) 
            return false;

        if (surf->nv==0) {
            if (faceGrid!=NULL) std::vector<std::vector<std::vector<std::vector<int>>>>().swap(*faceGrid);
            if (img->numberOfDimensions!=0) {img->deallocData();img->allocData();}
            return true;
        }

        // if ((mode!=MASK) || (faceGrid!=NULL)) {
        if ((mode==ONLY_BOUNDARY) || (mode==MASK_WITH_BOUNDARY) || (mode==PARTIAL_VOLUME) || (mode==FIELD_LABEL) || (faceGrid!=NULL)) {
            faceGridder(surf, img, faceGrid, (mode==FIELD_LABEL));
            disp(MSG_DETAIL,"Face grid is computed.");
        }

        switch (mode) {

            case ONLY_BOUNDARY: {break;} // There is nothing to do

            case MASK_WITH_BOUNDARY: {
                if (img->dataType != INT8_DT) {disp(MSG_ERROR,"Expected int8_t image type."); return false;}
                disp(MSG_DEBUG,"mapSurface2Image()->surfaceMaskWithBoundary()");
                surfaceMaskWithBoundary(reinterpret_cast<NIBR::Image<int8_t>*>(img), surf);
                break;
            }

            case MASK: {
                if (img->dataType != BOOL_DT) {disp(MSG_ERROR,"Expected bool image type."); return false;}
                disp(MSG_DEBUG,"mapSurface2Image()->surfaceMask()");
                surfaceMask(reinterpret_cast<NIBR::Image<bool>*>(img), surf);
                break;
            }

            case EDT: {
                if (img->dataType != FLOAT32_DT) {disp(MSG_ERROR,"Expected floating image type."); return false;}
                disp(MSG_DEBUG,"mapSurface2Image()->surfaceEDT()");
                surfaceEDT(reinterpret_cast<NIBR::Image<float>*>(img), surf);
                break;
            }

            case MAT: {
                if (img->dataType != FLOAT32_DT) {disp(MSG_ERROR,"Expected floating image type."); return false;}
                disp(MSG_DEBUG,"mapSurface2Image()->surfaceMAT()");
                surfaceMAT(reinterpret_cast<NIBR::Image<float>*>(img), surf);
                break;
            }

            case MIS: {
                if (img->dataType != FLOAT32_DT) {disp(MSG_ERROR,"Expected floating image type."); return false;}
                disp(MSG_DEBUG,"mapSurface2Image()->surfaceMIS()");
                surfaceMIS(reinterpret_cast<NIBR::Image<float>*>(img), surf);
                break;
            }

            case PARTIAL_VOLUME: {
                if (img->dataType != FLOAT32_DT) {disp(MSG_ERROR,"Expected floating image type."); return false;}
                disp(MSG_DEBUG,"mapSurface2Image()->surfacePVF()");
                surfacePVF(reinterpret_cast<NIBR::Image<float>*>(img), surf);
                break;
            }

            case FIELD_LABEL: {
                disp(MSG_DEBUG,"mapSurface2Image()->FIELD_LABEL");
                std::vector<std::vector<int>> flabel;
                if (sf->owner==NIBR::VERTEX) {
                    flabel.resize(surf->nf);
                    for (int i=0; i<surf->nf; i++) {

                        int l1 = sf->idata[surf->faces[i][0]][0];
                        int l2 = sf->idata[surf->faces[i][1]][0];
                        int l3 = sf->idata[surf->faces[i][2]][0];
                        int l  = (l2==l3) ? l2 : l1; // assign the majority label

                        flabel[i].push_back(l);
                    }
                }
                
                auto write_field_label = [&](NIBR::MT::TASK task) {
                    if (img->data[task.no] > OUTSIDE)
                        img->data[task.no] = (sf->owner==NIBR::FACE) ? sf->idata[int(img->data[task.no]-1)][0] : flabel[int(img->data[task.no]-1)][0];
                    else
                        img->data[task.no] = 0;
                    return;
                };

                NIBR::MT::MTRUN(img->numel, NIBR::MT::MAXNUMBEROFTHREADS(), write_field_label);

                break;
            }

            default: {
                disp(MSG_ERROR, "Unknown surface2imageMapper mode");
                return false;
            }
        
        }

        disp(MSG_DETAIL,"Mesh discretization finished.");
        return true;

    }

}
