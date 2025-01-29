#pragma once

#include "base/nibr.h"
#include "image/image.h"
#include "image/image_math.h"
#include "math/sphericalFunctions.h"
#include "math/sphericalHarmonics.h"
#include "math/reorient.h"
#include "math/conn3D.h"
#include <cstdint>
#include <tuple>

typedef unsigned int uint;

namespace NIBR
{

    void sf2sh(NIBR::Image<float>* out, NIBR::Image<float>* inp, std::vector<std::vector<float>>& coords, int shOrder);
    void sh2sf(NIBR::Image<float>* out, NIBR::Image<float>* inp, std::vector<std::vector<float>>& coords);

    void reorientSH(NIBR::Image<float>* img, OrderOfDirections ood);
    void rotateSH(NIBR::Image<float>* img, float R[][4]);

    DATATYPE getImageDataType(std::string imgFname);

    /*
    // List of defined functions in this file
    DATATYPE getImageDataType(std::string imgFname);
    bool imgCheckDimensionMatch(NIBR::Image<T1>& img1, NIBR::Image<T2>& img2);
    std::tuple<bool,T,T> isBinary(NIBR::Image<T>* img);
    std::vector<int> getIndices(NIBR::Image<T1>* img, T2 val);
    std::vector<int> getNonZeroIndices(NIBR::Image<T>* img);
    std::vector<int> getNonZeroIndices(NIBR::Image<T>* img, int volInd);
    std::vector<std::pair<T,int>> getNonZeroValuesAndIndices(NIBR::Image<T>* img);
    void imgResample(NIBR::Image<T_OUT>* imgOut,NIBR::Image<T1>* img, T2 M);
    void getImageSlice(NIBR::Image<T_OUT>* imgOut,NIBR::Image<T_INP>* img, int t);
    void padImg(NIBR::Image<T>* out, NIBR::Image<T>* inp, std::vector<uint> padDims, T padVal)
    */

    // -----COMPARE IMAGE DIMENSIONS---
    template<typename T1,typename T2>
    bool imgCheckDimensionMatch(NIBR::Image<T1>& img1, NIBR::Image<T2>& img2) {
        for (int i=0; i<7; i++) {
            if (img1.imgDims[i] != img2.imgDims[i])
                return false;
        }
        return true;
    }
    // -------------------------------
    
    // -----CHECK IF IMAGE IS BINARY---
    // Binary image is considered as any image that has up to two unique values
    // If the image turn out to be binary then the tuple returns true and the two values that exist in the image
    template<typename T>
    std::tuple<bool,T,T> isBinary(NIBR::Image<T>* img) {

        if (img->numel == 0)    return std::make_tuple(true,0,0);
        if (img->numel == 1)    return std::make_tuple(true,img->data[0],img->data[0]);

        T val0;
        T val1;
        val0 = img->data[0];
        val1 = val0;

        bool secondValFound = false;

        for (int n=1; n<img->numel; n++) {

            if (secondValFound) {
                if ((img->data[n]!=val0) && (img->data[n]!=val1))
                    return std::make_tuple(false,0,0);
            } else if (img->data[n]!=val0) {
                val1            = img->data[n];
                secondValFound  = true;
            }

        }

        if (val0>val1) std::swap(val0,val1);

        return std::make_tuple(true,val0,val1);

    }

    // -----GET INDICES OF A GIVEN VALUE---
    // Finds the indices of the given value
    template<typename T1,typename T2>
    std::vector<int> getIndices(NIBR::Image<T1>* img, T2 val) {

        std::vector<int> out;

        for (int n=0; n<img->numel; n++) {
            if (img->data[n] == val)
                out.push_back(n);
        }        

        return out;

    }
    // -------------------------------

    // -----GET INDICES OF NONZERO VALUES---
    template<typename T>
    std::vector<int> getNonZeroIndices(NIBR::Image<T>* img) {

        std::vector<int> out;

        for (int n=0; n<img->numel; n++) {
            if (img->data[n] != 0) {
                out.push_back(n);
            }
        }

        return out;

    }
    // -------------------------------
    

    // -----GET INDICES OF NONZERO VALUES---
    // This returns the ijk based index, not the ijkt-based
    template<typename T>
    std::vector<int> getNonZeroIndices(NIBR::Image<T>* img, int volInd) {

        std::vector<int> out;

        int64_t ind;

        for (int64_t i=0; i<img->imgDims[0]; i++) {
            for (int64_t j=0; j<img->imgDims[1]; j++) {
                for (int64_t k=0; k<img->imgDims[2]; k++) {
                    ind = img->sub2ind(i,j,k,volInd);
                    if (img->data[ind] != 0) {
                        out.push_back(int(img->sub2ind(i,j,k)));
                    }
                }
            }
        }   

        return out;

    }
    // -------------------------------


    // -----GET INDICES OF NONZERO VALUES---
    template<typename T>
    std::vector<std::pair<T,int>> getNonZeroValuesAndIndices(NIBR::Image<T>* img) {

        std::vector<std::pair<T, int>> out;

        T val;
        for (int n=0; n<img->numel; n++) {
            val = img->data[n];
            if (val != 0) {
                out.push_back(std::make_pair(val, n));
            }
        }

        return out;

    }
    // -------------------------------


    // -----GET 3D VOXEL SUBS WHEN THERE ARE NONZERO VALUES ALONG THE 4th DIM---
    template<typename T>
    std::vector<std::vector<int64_t>> getNonZero3DVoxelSubs(NIBR::Image<T>* img) {

        std::vector<std::vector<int64_t>>* nnzVoxelSubs;
        nnzVoxelSubs = new std::vector<std::vector<int64_t>>[MT::MAXNUMBEROFTHREADS()];

        auto data    = img->data;

        auto findVoxels = [&](NIBR::MT::TASK task)->void {
        
            int64_t i   = task.no;
            
            for (int64_t j=0; j<img->imgDims[1]; j++)
                for (int64_t k=0; k<img->imgDims[2]; k++)
                    for (int64_t t=0; t<img->imgDims[3]; t++) {

                        if (data[img->sub2ind(i,j,k,t)]!=0){
                            std::vector<int64_t> tmp{i,j,k};
                            nnzVoxelSubs[task.threadId].push_back(tmp);
                            break;
                        }
                        
                    }
        
        };
        NIBR::MT::MTRUN(img->imgDims[0], MT::MAXNUMBEROFTHREADS(), findVoxels);  

        std::vector<std::vector<int64_t>> subs;

        for (int i=0; i<MT::MAXNUMBEROFTHREADS(); i++) {
            subs.insert(subs.begin(),nnzVoxelSubs[i].begin(),nnzVoxelSubs[i].end());
        }
        delete[] nnzVoxelSubs;

        return subs;

    }
    // -------------------------------



    // -----RESAMPLE AN IMAGE---
    // This function is NOT tested. Use with care.
    template<typename T1,typename T2, typename T_OUT>
    void imgResample(NIBR::Image<T_OUT>* imgOut,NIBR::Image<T1>* img, T2 M) {
        
        auto f = [&](NIBR::MT::TASK task)->void {
            
            int64_t i,j,k;
            float     p[3];
            imgOut->ind2sub(task.no,i,j,k);
            imgOut->to_xyz(task.no,p);
            applyTransform(p,M);

            for (int64_t t=0; t<img->valCnt; t++) {
                imgOut->data[imgOut->sub2ind(i,j,k,t)] = (*img)(p,t);
            }

        };
        
        NIBR::MT::MTRUN(imgOut->voxCnt,f);
        
    }

    template<typename T_INP, typename T_OUT>
    void imgResample(NIBR::Image<T_OUT>* imgOut,NIBR::Image<T_INP>* img) {
        
        auto f = [&](NIBR::MT::TASK task)->void {
            
            int64_t i,j,k;
            float p[3];
            imgOut->ind2sub(task.no,i,j,k);
            imgOut->to_xyz(task.no,p);

            for (int64_t t=0; t<img->valCnt; t++) {
                imgOut->data[imgOut->sub2ind(i,j,k,t)] = (*img)(p,t);
            }

        };
        
        NIBR::MT::MTRUN(imgOut->voxCnt,f);
        
    }
    // -------------------------------

    // -----EXTRACTS A SLICE FROM A 4D IMAGE AND CREATES A 3D IMAGE---
    // This function is NOT tested. Use with care.
    template<typename T_OUT,typename T_INP>
    void getImageSlice(NIBR::Image<T_OUT>* imgOut,NIBR::Image<T_INP>* img, int t) {

        // Create image if needed
        if ( (imgOut == NULL) || (imgOut->data == NULL) ) {
            int64_t imgDims[3] = {img->imgDims[0], img->imgDims[1], img->imgDims[2]};
            imgOut->create(3, &imgDims[0], img->pixDims, img->ijk2xyz, true);
        }

        int64_t i,j,k;
        for (int64_t n=0; n<imgOut->voxCnt; n++) {
            imgOut->ind2sub(n,i,j,k);
            imgOut->data[n] = img->data[img->sub2ind(i,j,k,t)];
        }
        
    }
    // -------------------------------


    // -----PADS VALUES ON 3D or 4D IMAGES---
    // This function is NOT tested. Use with care.
    template<typename T>
    void imgPad(NIBR::Image<T>* out, NIBR::Image<T>* inp, std::vector<uint> padDims, T padVal)
    {

        if ((padDims.size()!=3) && (padDims.size()!=4) && (padDims.size()!=6) && (padDims.size()!=8)) {
            disp(MSG_ERROR,"Size of padDims should be either 3, 4, 6 or 8.");
            return;
        }

        if ( ((padDims.size()==4) || (padDims.size()==8)) && (inp->numberOfDimensions!=4) ) {
            disp(MSG_ERROR,"Size of padDims does not match image dimensions");
            return;
        }

        if (padDims.size()==3) {
            std::vector<uint> tmp = {padDims[0],padDims[0],padDims[1],padDims[1],padDims[2],padDims[2],0,0};
            padDims = tmp;
        }
        if (padDims.size()==4) {
            std::vector<uint> tmp = {padDims[0],padDims[0],padDims[1],padDims[1],padDims[2],padDims[2],padDims[3],padDims[3]};
            padDims = tmp;
        }
        if (padDims.size()==6) {
            std::vector<uint> tmp = {padDims[0],padDims[1],padDims[2],padDims[3],padDims[4],padDims[5],0,0};
            padDims = tmp;
        }

        int64_t imgDims[7];     memcpy(imgDims, inp->imgDims,  7*sizeof(int64_t));
        float   pixDims[7];     memcpy(pixDims, inp->pixDims,  7*sizeof(float));
        float   ijk2xyz[3][4];  memcpy(ijk2xyz, inp->ijk2xyz, 12*sizeof(float));

        for (int i=0; i<4; i++) {
            imgDims[i] += padDims[i*2] + padDims[i*2+1];
        }
        
        ijk2xyz[0][3] -= ijk2xyz[0][0]*padDims[0] + ijk2xyz[0][1]*padDims[2] + ijk2xyz[0][2]*padDims[4];
        ijk2xyz[1][3] -= ijk2xyz[1][0]*padDims[0] + ijk2xyz[1][1]*padDims[2] + ijk2xyz[1][2]*padDims[4];
        ijk2xyz[2][3] -= ijk2xyz[2][0]*padDims[0] + ijk2xyz[2][1]*padDims[2] + ijk2xyz[2][2]*padDims[4];

        out->create(inp->numberOfDimensions, imgDims, pixDims, ijk2xyz, true);
        
        memset(out->data, padVal, sizeof(T)*out->numel);
        
        if (out->numberOfDimensions==3) {
            for (int64_t i = 0; i < inp->imgDims[0]; i++)
                for (int64_t j = 0; j < inp->imgDims[1]; j++)
                    for (int64_t k = 0; k < inp->imgDims[2]; k++) {
                            out->data[out->sub2ind(i+padDims[0],j+padDims[2],k+padDims[4])] = inp->data[inp->sub2ind(i,j,k)];
                        }
        } else {
            for (int64_t i = 0; i < inp->imgDims[0]; i++)
                for (int64_t j = 0; j < inp->imgDims[1]; j++)
                    for (int64_t k = 0; k < inp->imgDims[2]; k++)
                        for (int64_t t = 0; t < inp->imgDims[3]; t++) {
                            out->data[out->sub2ind(i+padDims[0],j+padDims[2],k+padDims[4],t+padDims[6])] = inp->data[inp->sub2ind(i,j,k,t)];
                        }
        }

    }

    template<typename T>
    void imgPad(NIBR::Image<T>& inp, int padDim) 
    {
        Image<T> out;
        T padVal = 0;

        std::vector<uint> padDims;

        padDims.push_back(padDim);
        padDims.push_back(padDim);
        padDims.push_back(padDim);

        if (inp.numberOfDimensions == 4) {
            padDims.push_back(padDim);
        }

        imgPad(&out, &inp, padDims, padVal);

        inp = out;
    }


    // -----Crops 3D or 4D IMAGES---
    // This function is NOT tested. Use with care.
    template<typename T>
    void imgCrop(NIBR::Image<T>* out, NIBR::Image<T>* inp, std::vector<uint> slicesInds)
    {

        if ((slicesInds.size()!=6) && (slicesInds.size()!=8)) {
            disp(MSG_ERROR," Size of slice indices for cropping should be 6 or 8.");
            return;
        }

        if ((slicesInds.size()==8) && (inp->numberOfDimensions!=4) ) {
            disp(MSG_ERROR,"Size of slices does not match image dimensions");
            return;
        }

        if ((slicesInds.size()==6) && (inp->numberOfDimensions==3)) {
            std::vector<uint> tmp = {slicesInds[0]-1,slicesInds[1],slicesInds[2]-1,slicesInds[3],slicesInds[4]-1,slicesInds[5]};
            slicesInds = tmp;

            if ( (slicesInds[1]>inp->imgDims[0]) || (slicesInds[0]>=slicesInds[1]) ||
                 (slicesInds[3]>inp->imgDims[1]) || (slicesInds[2]>=slicesInds[3]) ||
                 (slicesInds[5]>inp->imgDims[2]) || (slicesInds[4]>=slicesInds[5])) {
                disp(MSG_ERROR,"Slice indices from cropping are not acceptable.");
                return;
            }

        } else if ((slicesInds.size()==6) && (inp->numberOfDimensions==4)) {
            std::vector<uint> tmp = {slicesInds[0]-1,slicesInds[1],slicesInds[2]-1,slicesInds[3],slicesInds[4]-1,slicesInds[5],0,uint(inp->valCnt)};
            slicesInds = tmp;
        } else if (slicesInds.size()==8) {
            std::vector<uint> tmp = {slicesInds[0]-1,slicesInds[1],slicesInds[2]-1,slicesInds[3],slicesInds[4]-1,slicesInds[5],slicesInds[6]-1,slicesInds[7]};
            slicesInds = tmp;
        }

        int64_t imgDims[7];     memcpy(imgDims, inp->imgDims,  7*sizeof(int64_t));
        float   pixDims[7];     memcpy(pixDims, inp->pixDims,  7*sizeof(float));
        float   ijk2xyz[3][4];  memcpy(ijk2xyz, inp->ijk2xyz, 12*sizeof(float));


        imgDims[0] = slicesInds[1] - slicesInds[0];
        imgDims[1] = slicesInds[3] - slicesInds[2];
        imgDims[2] = slicesInds[5] - slicesInds[4];

        if (slicesInds.size()==8) {
            if ( (slicesInds[1]>inp->imgDims[0]) || (slicesInds[0]>=slicesInds[1]) ||
                 (slicesInds[3]>inp->imgDims[1]) || (slicesInds[2]>=slicesInds[3]) ||
                 (slicesInds[5]>inp->imgDims[2]) || (slicesInds[4]>=slicesInds[5]) ||
                 (slicesInds[7]>inp->imgDims[3]) || (slicesInds[6]>=slicesInds[7]) ) {
                    disp(MSG_ERROR,"Slice indices from cropping are not acceptable.");
                    return;
                }
            imgDims[3] = slicesInds[7] - slicesInds[6];
        }
        
        ijk2xyz[0][3] += ijk2xyz[0][0]*slicesInds[0] + ijk2xyz[0][1]*slicesInds[2] + ijk2xyz[0][2]*slicesInds[4];
        ijk2xyz[1][3] += ijk2xyz[1][0]*slicesInds[0] + ijk2xyz[1][1]*slicesInds[2] + ijk2xyz[1][2]*slicesInds[4];
        ijk2xyz[2][3] += ijk2xyz[2][0]*slicesInds[0] + ijk2xyz[2][1]*slicesInds[2] + ijk2xyz[2][2]*slicesInds[4];

        out->create(inp->numberOfDimensions, imgDims, pixDims, ijk2xyz, true);
        
        if (out->numberOfDimensions==3) {
            for (int64_t i = 0; i < imgDims[0]; i++)
                for (int64_t j = 0; j < imgDims[1]; j++)
                    for (int64_t k = 0; k < imgDims[2]; k++) {
                            out->data[out->sub2ind(i,j,k)] = inp->data[inp->sub2ind(i+slicesInds[0],j+slicesInds[2],k+slicesInds[4])];
                        }
        } else {
            for (int64_t i = 0; i < imgDims[0]; i++)
                for (int64_t j = 0; j < imgDims[1]; j++)
                    for (int64_t k = 0; k < imgDims[2]; k++)
                        for (int64_t t = 0; t < imgDims[3]; t++) {
                            out->data[out->sub2ind(i,j,k,t)] = inp->data[inp->sub2ind(i+slicesInds[0],j+slicesInds[2],k+slicesInds[4],t+slicesInds[6])];
                        }
        }

    }


    template<typename T_OUT, typename T>
    void imgMedFilt(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T>& img, CONN3D conn) {

        if (imgOut.data == NULL) {
            imgOut.createFromTemplate(img, true);
        }

        auto N = get3DNeighbors(conn);
        std::vector<int> inds = getNonZeroIndices(&img);

        auto f = [&](NIBR::MT::TASK task)->void {
            int64_t i, j, k;
            img.ind2sub(inds[task.no], i, j, k);

            std::vector<T> neighbors;
            neighbors.push_back(img(i, j, k)); // Include the pixel itself

            for (auto n : N) {
                int64_t ii = i + n[0];
                int64_t jj = j + n[1];
                int64_t kk = k + n[2];

                if (img.isInside(ii, jj, kk)) {
                    neighbors.push_back(img(ii, jj, kk));
                }
            }

            std::sort(neighbors.begin(), neighbors.end());
            imgOut.data[inds[task.no]] = neighbors[neighbors.size() / 2];
        };

        NIBR::MT::MTRUN(inds.size(), f);

    }


    template<typename T_OUT, typename T>
    void imgAveFilt(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T>& img, CONN3D conn) {

        if (imgOut.data == NULL) {
            imgOut.createFromTemplate(img, true);
        }

        auto N = get3DNeighbors(conn);
        std::vector<int> inds = getNonZeroIndices(&img);

        auto f = [&](NIBR::MT::TASK task)->void {
            int64_t i, j, k;
            img.ind2sub(inds[task.no], i, j, k);

            double totSum = img.data[inds[task.no]];
            double totEl  = 1;

            for (auto n : N) {
                int64_t ii = i + n[0];
                int64_t jj = j + n[1];
                int64_t kk = k + n[2];

                if (img.isInside(ii, jj, kk)) {
                   totSum += img(ii, jj, kk);
                   totEl++;
                }
            }

            imgOut.data[inds[task.no]] = totSum / totEl;
        };

        NIBR::MT::MTRUN(inds.size(), f);

    }


}

