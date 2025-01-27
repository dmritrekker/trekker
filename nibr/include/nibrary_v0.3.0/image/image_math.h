#pragma once

#include "base/nibr.h"
#include "image/image.h"
#include "math/core.h"
#include <cmath>
#include <tuple>

namespace NIBR
{

    /*
    // List of defined functions in this file
    void imgThresh(NIBR::Image<T>& img, float loVal, float hiVal);
    void imgThresh(NIBR::Image<T_OUT>& imgOut, NIBR::Image<T_INP>& img, float loVal, float hiVal=FLT_MAX);
    void imgAdd(NIBR::Image<T1>& img1,NIBR::Image<T2>& img2);
    void imgAdd(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,NIBR::Image<T2>& img2);
    void imgAdd(NIBR::Image<T1>& img1,T2 x);
    void imgAdd(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,T2 x);
    void imgSub(NIBR::Image<T1>& img1,NIBR::Image<T2>& img2);
    void imgSub(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,NIBR::Image<T2>& img2);
    void imgSub(NIBR::Image<T1>& img1,T2 x);
    void imgSub(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,T2 x);
    void imgMult(NIBR::Image<T1>& img1,NIBR::Image<T2>& img2);
    void imgMult(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,NIBR::Image<T2>& img2);
    void imgMult(NIBR::Image<T1>& img1,T2 x);
    void imgMult(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,T2 x);
    void imgDiv(NIBR::Image<T1>& img1,NIBR::Image<T2>& img2);
    void imgDiv(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,NIBR::Image<T2>& img2);
    void imgDiv(NIBR::Image<T1>& img1,T2 x);
    void imgDiv(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,T2 x);
    void imgAbs(NIBR::Image<T>& img);
    void imgAbs(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T>& img);
    void imgNot(NIBR::Image<T>& img);
    void imgNot(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T>& img);
    void imgSqrt(NIBR::Image<T>& img);
    void imgSqrt(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T>& img);
    void imgPow(NIBR::Image<T1>& img1,T2 x);
    void imgPow(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,T2 x);
    T imgMax(NIBR::Image<T>* img);
    T imgMin(NIBR::Image<T>* img);
    std::tuple<T,T> imgMinMax(NIBR::Image<T>* img);
    std::tuple<T,T> imgMinMax(NIBR::Image<T>* img, int volInd);
    void imgRescaleToRange(NIBR::Image<T>& img, double minVal, double maxVal, bool ignoreZeros);
    void imgRescaleToRange(NIBR::Image<T>& imgOut, NIBR::Image<T>& img, double minVal, double maxVal, bool ignoreZeros);
    void imgNorm(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T>& img);
    void imgNormalize(NIBR::Image<T>& imgOut,NIBR::Image<T>& img);
    */
    



    // FUNCTION DEFINITIONS

    // ------------THRESHOLDING-----------
    // Threshold and overwrite input image
    template<typename T>
    void imgThresh(NIBR::Image<T>& img, float loVal, float hiVal) {

        for (int n = 0; n < img.numel; n++) {
            img.data[n] = (img.data[n]>=loVal && img.data[n]<=hiVal) ? 1 : 0; 
        }
        
    }

    // Threshold and write output
    template<typename T_INP,typename T_OUT>
    void imgThresh(NIBR::Image<T_OUT>& imgOut, NIBR::Image<T_INP>& img, float loVal, float hiVal=FLT_MAX) {

        if (imgOut.data == NULL)
            imgOut.create(img.numberOfDimensions,img.imgDims,img.pixDims,img.ijk2xyz,true);

        for (int n = 0; n < img.numel; n++) {
            imgOut.data[n] = (img.data[n]>=loVal && img.data[n]<=hiVal) ? 1 : 0; 
        }
        
    }
    // -------------------------------

    // ------------CLAMPING-----------
    template<typename T>
    void imgClamp(NIBR::Image<T>& img, float loVal, float hiVal) {

        for (int n = 0; n < img.numel; n++) {
            if      (img.data[n] < loVal) img.data[n] = loVal;
            else if (img.data[n] > hiVal) img.data[n] = hiVal;
        }
        
    }

    template<typename T_INP,typename T_OUT>
    void imgClamp(NIBR::Image<T_OUT>& imgOut, NIBR::Image<T_INP>& img, float loVal, float hiVal=FLT_MAX) {

        if (imgOut.data == NULL)
            imgOut.create(img.numberOfDimensions,img.imgDims,img.pixDims,img.ijk2xyz,true);

        for (int n = 0; n < img.numel; n++) {
            if      (img.data[n] < loVal) imgOut.data[n] = loVal;
            else if (img.data[n] > hiVal) imgOut.data[n] = hiVal;
            else imgOut.data[n] = img.data[n];
        }
        
    }
    // -------------------------------


    // ------------ADDITION-----------
    // Add and overwrite first image
    template<typename T1,typename T2>
    void imgAdd(NIBR::Image<T1>& img1,NIBR::Image<T2>& img2) {

        for (int n = 0; n < img1.numel; n++) {
           img1.data[n] += img2.data[n]; 
        }
        
    }

    // Add and write output on new image
    template<typename T1,typename T2,typename T_OUT>
    void imgAdd(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,NIBR::Image<T2>& img2) {
        
        if (imgOut.data == NULL)
            imgOut.create(img1.numberOfDimensions,img1.imgDims,img1.pixDims,img1.ijk2xyz,true);

        for (int n = 0; n < img1.numel; n++) {
            imgOut.data[n] = img1.data[n] + img2.data[n]; 
        }
        
    }
    
    // Add a scalar value and overwrite first image
    template<typename T1,typename T2>
    void imgAdd(NIBR::Image<T1>& img1,T2 x) {
        
        for (int n = 0; n < img1.numel; n++) {
            img1.data[n] += x; 
        }
        
    }

    // Add and write output on new image
    template<typename T1,typename T2,typename T_OUT>
    void imgAdd(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,T2 x) {

        if (imgOut.data == NULL)
            imgOut.create(img1.numberOfDimensions,img1.imgDims,img1.pixDims,img1.ijk2xyz,true);
        
        for (int n = 0; n < img1.numel; n++) {
            imgOut.data[n] = img1.data[n] + x; 
        }
        
    }
    // -------------------------------


    // ------------SUBTRACT-----------
    // Subtract and overwrite first image
    template<typename T1,typename T2>
    void imgSub(NIBR::Image<T1>& img1,NIBR::Image<T2>& img2) {
        
        auto f = [&](NIBR::MT::TASK task)->void {
            img1.data[task.no] -= img2.data[task.no];
        };
        
        NIBR::MT::MTRUN(img1.numel,f);
        
    }

    // Subtract and write output on new image
    template<typename T1,typename T2,typename T_OUT>
    void imgSub(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,NIBR::Image<T2>& img2) {
        
        if (imgOut.data == NULL)
            imgOut.create(img1.numberOfDimensions,img1.imgDims,img1.pixDims,img1.ijk2xyz,true);

        auto f = [&](NIBR::MT::TASK task)->void {
            imgOut.data[task.no] = img1.data[task.no] - img2.data[task.no];
        };
        
        NIBR::MT::MTRUN(imgOut.numel,f);
        
    }
    
    // Subtract a scalar value and overwrite first image
    template<typename T1,typename T2>
    void imgSub(NIBR::Image<T1>& img1,T2 x) {
        
        auto f = [&](NIBR::MT::TASK task)->void {
            img1.data[task.no] -= x;
        };
        
        NIBR::MT::MTRUN(img1.numel,f);
        
    }

    // Subtract and write output on new image
    template<typename T1,typename T2,typename T_OUT>
    void imgSub(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,T2 x) {

        if (imgOut.data == NULL)
            imgOut.create(img1.numberOfDimensions,img1.imgDims,img1.pixDims,img1.ijk2xyz,true);
        
        auto f = [&](NIBR::MT::TASK task)->void {
            imgOut.data[task.no] = img1.data[task.no] - x;
        };
        
        NIBR::MT::MTRUN(img1.numel,f);
        
    }
    // -------------------------------




    // ------------MULTIPLY-----------
    // Subtract and overwrite first image
    template<typename T1,typename T2>
    void imgMult(NIBR::Image<T1>& img1,NIBR::Image<T2>& img2) {
        
        auto f = [&](NIBR::MT::TASK task)->void {
            img1.data[task.no] *= img2.data[task.no];
        };
        
        NIBR::MT::MTRUN(img1.numel,f);
        
    }

    // Subtract and write output on new image
    template<typename T1,typename T2,typename T_OUT>
    void imgMult(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,NIBR::Image<T2>& img2) {
        
        if (imgOut.data == NULL)
            imgOut.create(img1.numberOfDimensions,img1.imgDims,img1.pixDims,img1.ijk2xyz,true);

        auto f = [&](NIBR::MT::TASK task)->void {
            imgOut.data[task.no] = img1.data[task.no] * img2.data[task.no];
        };
        
        NIBR::MT::MTRUN(imgOut.numel,f);
        
    }
    
    // Subtract a scalar value and overwrite first image
    template<typename T1,typename T2>
    void imgMult(NIBR::Image<T1>& img1,T2 x) {
        
        auto f = [&](NIBR::MT::TASK task)->void {
            img1.data[task.no] *= x;
        };
        
        NIBR::MT::MTRUN(img1.numel,f);
        
    }

    // Subtract and write output on new image
    template<typename T1,typename T2,typename T_OUT>
    void imgMult(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,T2 x) {

        if (imgOut.data == NULL)
            imgOut.create(img1.numberOfDimensions,img1.imgDims,img1.pixDims,img1.ijk2xyz,true);
        
        auto f = [&](NIBR::MT::TASK task)->void {
            imgOut.data[task.no] = img1.data[task.no] * x;
        };
        
        NIBR::MT::MTRUN(img1.numel,f);
        
    }
    // -------------------------------





    // ------------DIVIDE-----------
    // Subtract and overwrite first image
    template<typename T1,typename T2>
    void imgDiv(NIBR::Image<T1>& img1,NIBR::Image<T2>& img2) {
        
        auto f = [&](NIBR::MT::TASK task)->void {
            img1.data[task.no] = (img1.data[task.no]==0) ? 0 : img1.data[task.no]/img2.data[task.no];                
        };
        
        NIBR::MT::MTRUN(img1.numel,f);
        
    }

    // Subtract and write output on new image
    template<typename T1,typename T2,typename T_OUT>
    void imgDiv(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,NIBR::Image<T2>& img2) {
        
        if (imgOut.data == NULL)
            imgOut.create(img1.numberOfDimensions,img1.imgDims,img1.pixDims,img1.ijk2xyz,true);

        auto f = [&](NIBR::MT::TASK task)->void {
            imgOut.data[task.no] = img1.data[task.no] / img2.data[task.no];
        };
        
        NIBR::MT::MTRUN(imgOut.numel,f);
        
    }
    
    // Subtract a scalar value and overwrite first image
    template<typename T1,typename T2>
    void imgDiv(NIBR::Image<T1>& img1,T2 x) {
        
        auto f = [&](NIBR::MT::TASK task)->void {
            img1.data[task.no] /= x;
        };
        
        NIBR::MT::MTRUN(img1.numel,f);
        
    }

    // Subtract and write output on new image
    template<typename T1,typename T2,typename T_OUT>
    void imgDiv(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,T2 x) {

        if (imgOut.data == NULL)
            imgOut.create(img1.numberOfDimensions,img1.imgDims,img1.pixDims,img1.ijk2xyz,true);
        
        auto f = [&](NIBR::MT::TASK task)->void {
            imgOut.data[task.no] = img1.data[task.no] / x;
        };
        
        NIBR::MT::MTRUN(img1.numel,f);
        
    }
    // -------------------------------


    // ------------ABSOLUTE VALUE-----------
    // Abs and overwrite input image
    template<typename T>
    void imgAbs(NIBR::Image<T>& img) {
        
        auto f = [&](NIBR::MT::TASK task)->void {
            if (getSign(img.data[task.no])==-1)
                img.data[task.no] = -img.data[task.no];
        };
        
        NIBR::MT::MTRUN(img.numel,f);
        
    }

    // Abs and write output on new image
    template<typename T,typename T_OUT>
    void imgAbs(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T>& img) {

        if (imgOut.data == NULL)
            imgOut.createFromTemplate(img,true);
        
        auto f = [&](NIBR::MT::TASK task)->void {
            imgOut.data[task.no] = (getSign(img.data[task.no])==-1) ? -img.data[task.no] : img.data[task.no];
        };
        
        NIBR::MT::MTRUN(img.numel,f);
        
    }
    // -------------------------------

    // ------------NOT VALUE-----------
    // Not and overwrite input image
    template<typename T>
    void imgNot(NIBR::Image<T>& img) {
        
        auto f = [&](NIBR::MT::TASK task)->void {
            img.data[task.no] = !img.data[task.no];
        };
        
        NIBR::MT::MTRUN(img.numel,f);
        
    }

    // Not and write output on new image
    template<typename T,typename T_OUT>
    void imgNot(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T>& img) {

        if (imgOut.data == NULL)
            imgOut.createFromTemplate(img,true);
        
        auto f = [&](NIBR::MT::TASK task)->void {
            imgOut.data[task.no] = !(img.data[task.no]>0);
        };
        
        NIBR::MT::MTRUN(img.numel,f);
        
    }
    // -------------------------------

    // --------------SQRT-------------
    // Sqrt and overwrite input image
    template<typename T>
    void imgSqrt(NIBR::Image<T>& img) {
        
        auto f = [&](NIBR::MT::TASK task)->void {
            img.data[task.no] = std::sqrt(img.data[task.no]);
        };
        
        NIBR::MT::MTRUN(img.numel,f);
        
    }

    // Sqrt and write output on new image
    template<typename T_OUT,typename T>
    void imgSqrt(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T>& img) {

        if (imgOut.data == NULL)
            imgOut.createFromTemplate(img,true);
        
        auto f = [&](NIBR::MT::TASK task)->void {
            imgOut.data[task.no] = std::sqrt(img.data[task.no]);
        };
        
        NIBR::MT::MTRUN(img.numel,f);
        
    }
    // -------------------------------


    // --------------POW--------------
    // Power of an image
    template<typename T1,typename T2>
    void imgPow(NIBR::Image<T1>& img1,T2 x) {
        
        auto f = [&](NIBR::MT::TASK task)->void {
            img1.data[task.no] = std::pow(img1.data[task.no],x);
        };
        
        NIBR::MT::MTRUN(img1.numel,f);
        
    }

    // Power and write output on new image
    template<typename T1,typename T2,typename T_OUT>
    void imgPow(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T1>& img1,T2 x) {

        if (imgOut.data == NULL)
            imgOut.create(img1.numberOfDimensions,img1.imgDims,img1.pixDims,img1.ijk2xyz,true);
        
        auto f = [&](NIBR::MT::TASK task)->void {
            imgOut.data[task.no] = std::pow(img1.data[task.no],x);
        };
        
        NIBR::MT::MTRUN(img1.numel,f);
        
    }
    // -------------------------------


    // ------------FINDS MAX VALUE----
    template<typename T>
    T imgMax(NIBR::Image<T>* img) {

        if (img->numel == 0) return 0;

        T maxVal = img->data[0];

        for (int n=0; n<img->numel; n++) {
            if (img->data[n] > maxVal)
                maxVal = img->data[n];
        }        

        return maxVal;

    }
    // -------------------------------

    // ------------FINDS MIN VALUE----
    template<typename T>
    T imgMin(NIBR::Image<T>* img) {

        if (img->numel == 0) return 0;
        
        T minVal = img->data[0];

        for (int n=0; n<img->numel; n++) {
            if (img->data[n] < minVal)
                minVal = img->data[n];
        }        

        return minVal;

    }
    // -------------------------------
    // ----FINDS MIN AND MAX VALUE----
    template<typename T>
    std::tuple<T,T> imgMinMax(NIBR::Image<T>* img, bool ignoreZeros, bool ignoreNANs) {

        if (img->numel == 0) {
            disp(MSG_WARN,"Empty image, returning 0 as min and max values.");
            return std::make_tuple(static_cast<T>(0), static_cast<T>(0));
        }

        auto isValid = [&](T val) {
            if (ignoreZeros && val == static_cast<T>(0))             return false;
            if (ignoreNANs  && std::isnan(static_cast<double>(val))) return false;
            return true;
        };

        // Find the first valid value
        int firstValidIndex = -1;
        for (int n = 0; n < img->numel; ++n) {
            if (isValid(img->data[n])) {
                firstValidIndex = n;
                break;
            }
        }

        // If no valid values found, return (0,0)
        if (firstValidIndex == -1) {
            disp(MSG_WARN,"Image only has zeros or nans, returning 0 as min and max values.");
            return std::make_tuple(static_cast<T>(0), static_cast<T>(0));
        }

        T minVal = img->data[firstValidIndex];
        T maxVal = img->data[firstValidIndex];

        // Loop through the remaining elements
        for (int n = firstValidIndex + 1; n < img->numel; ++n) {
            T val = img->data[n];
            if (!isValid(val)) continue;
            if (val < minVal)  minVal = val;
            if (val > maxVal)  maxVal = val;
        }

        return std::make_tuple(minVal, maxVal);
    }

    // ----FINDS MIN AND MAX VALUE----
    template<typename T>
    std::tuple<T,T> imgMinMax(NIBR::Image<T>* img) {

        if (img->numel == 0) return std::make_tuple(0,0);
        
        T minVal = img->data[0];
        T maxVal = img->data[0];

        for (int n=0; n<img->numel; n++) {
            if (img->data[n] < minVal)  minVal = img->data[n];
            if (img->data[n] > maxVal)  maxVal = img->data[n];
        }        

        return std::make_tuple(minVal,maxVal);

    }

    // ----FINDS MIN AND MAX VALUE----
    template<typename T>
    std::tuple<T,T> imgMinMax(NIBR::Image<T>* img, int volInd) {

        if (img->numel == 0) {
            disp(MSG_WARN,"Image is empty. Setting min and max to 0.");
            return std::make_tuple(0,0);
        }

        int64_t ind = img->sub2ind(0,0,0,volInd);
        
        T minVal    = img->data[ind];
        T maxVal    = img->data[ind];

        for (int64_t i=0; i<img->imgDims[0]; i++) {
            for (int64_t j=0; j<img->imgDims[1]; j++) {
                for (int64_t k=0; k<img->imgDims[2]; k++) {
                    ind = img->sub2ind(i,j,k,volInd);
                    if (img->data[ind] < minVal)  minVal = img->data[ind];
                    if (img->data[ind] > maxVal)  maxVal = img->data[ind];
                }
            }
        }

        disp(MSG_DETAIL,"Image [min,max] is [%.2f,%.2f].",minVal,maxVal);
        return std::make_tuple(minVal,maxVal);

    }

    // --------------RESCALETORANGE-------------
    template<typename T>
    void imgRescaleToRange(NIBR::Image<T>& img, double minVal, double maxVal, bool ignoreZeros) {

        if (minVal > maxVal) {
            disp(MSG_ERROR,"minVal > maxVal");
            return;
        }

        auto [oldMin, oldMax] = imgMinMax(&img, ignoreZeros, true);

        if (oldMax == oldMin) {
            for (int i = 0; i < img.numel; ++i) {
                T& val = img.data[i];
                if (ignoreZeros && val == static_cast<T>(0)) continue;
                val = minVal;
            }
            return;
        }

        T oldRange = oldMax - oldMin;
        T newRange = maxVal - minVal;

        for (int i = 0; i < img.numel; ++i) {
            T& val = img.data[i];

            if (ignoreZeros && val == static_cast<T>(0)) continue;

            T norm = (val - oldMin) / oldRange;
            val = norm * newRange + minVal;
        }
    }

    template<typename T_OUT,typename T>
    void imgRescaleToRange(NIBR::Image<T>& imgOut, NIBR::Image<T>& img, double minVal, double maxVal, bool ignoreZeros) {
        imgOut = img;
        return imgRescaleToRange(imgOut,minVal,maxVal,ignoreZeros);
    }
    // -------------------------------

    template<typename T_OUT,typename T>
    void imgNorm(NIBR::Image<T_OUT>& imgOut,NIBR::Image<T>& img) {

        if (imgOut.data == NULL) {
            int64_t imgDims[7] = {img.imgDims[0],img.imgDims[1],img.imgDims[2],1,1,1,1};
            float   pixDims[7] = {img.pixDims[0],img.pixDims[1],img.pixDims[2],0,0,0,0};
            imgOut.create(3,imgDims,pixDims,img.ijk2xyz,true);
        }

        if (img.numberOfDimensions == 3) {
            imgOut = img;
            return;
        }
        
        auto f = [&](NIBR::MT::TASK task)->void {

            double val = 0.0;
            for (int t = 0; t < img.valCnt; t++) {
                val += (*img.at(task.no,t))*(*img.at(task.no,t));
            }

            imgOut.data[task.no] = std::sqrt(val);
        };
        
        NIBR::MT::MTRUN(img.voxCnt,f);

    }

    template<typename T>
    void imgNormalize(NIBR::Image<T>& imgOut,NIBR::Image<T>& img) {

        if (imgOut.data == NULL) {
            imgOut.create(img.numberOfDimensions,img.imgDims,img.pixDims,img.ijk2xyz,true);
            // int order[7] = {3,0,1,2,4,5,6};
            // imgOut.indexData(order);
        }

        if (img.numberOfDimensions == 3) {
            for (int n = 0; n < imgOut.numel; n++) {
                imgOut.data[n] = 1.0;
            }
            return;
        }
        
        auto f = [&](NIBR::MT::TASK task)->void {
            double val = 0.0;
            for (int t = 0; t < img.valCnt; t++) {
                val += (*img.at(task.no,t))*(*img.at(task.no,t));
            }

            val = (std::fabs(val) > EPS12) ? (1.0 / std::sqrt(val)) : 0.0;

            for (int t = 0; t < img.valCnt; t++) {
                (*imgOut.at(task.no,t)) = (*img.at(task.no,t)) * val;
            }
        };
        
        NIBR::MT::MTRUN(img.voxCnt,f);

    }



}
