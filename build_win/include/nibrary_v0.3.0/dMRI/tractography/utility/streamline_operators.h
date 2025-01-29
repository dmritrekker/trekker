#pragma once

#include "base/nibr.h"
#include "image/image.h"
#include "dMRI/tractography/io/tractogramReader.h"
#include <vector>

namespace NIBR 
{
    float getStreamlineLength(float** inp, int len);
    float getStreamlineLength(std::vector<std::vector<float>>& inp);
    float getStreamlineLength(std::vector<Point>& inp);

    float getStreamlineLength_hermite(float** inp, int len, int div);                   // Uses Hermite interpolation and does numerical integration with div number of divisions
    float getStreamlineLength_hermite(std::vector<std::vector<float>>& inp, int div);   // Uses Hermite interpolation and does numerical integration with div number of divisions
    std::vector<float> getStreamlineLength_hermiteWithSpeed(std::vector<std::vector<float>>& inp, int div);

    float getHausdorffDistance(std::vector<std::vector<float>>& trk1, std::vector<std::vector<float>>& trk2);
    float getMDFDistance(std::vector<std::vector<float>>& trk1, std::vector<std::vector<float>>& trk2);


    float** colorStreamline(std::vector<std::vector<float>>& inp);

    template<typename T>
    std::unordered_map<int64_t,float> traceStreamline(
        size_t streamlineId,
        int threadNo, 
        TractogramReader* tractogram,
        Image<T>& img,
        bool*** mask, 
        std::function<void(std::unordered_map<int64_t,float>*,Image<T>*,int*,NIBR::Segment&,void*)> f, void* fData)

    {

        // disp(MSG_DEBUG,"Tracing streamline: %d", streamlineId);

        std::unordered_map<int64_t,float> vIdx;

        int len = tractogram[threadNo].len[streamlineId];
        if (len < 1) return vIdx;
        
        float** streamline = tractogram[threadNo].readStreamline(streamlineId);
        
        double p0[3], p1[3], dir[3], length, lengthR, lengthScale;

        int32_t A[3], B[3];
        
        NIBR::Segment seg;
        seg.streamlineNo = streamlineId;
        
        // End of segment in real and its corner in image space
        img.to_ijk(streamline[0],p0);
        A[0]  = std::round(p0[0]);
        A[1]  = std::round(p0[1]);
        A[2]  = std::round(p0[2]);

        auto runFun = [&]() {
            if ( img.isInside(A) && ((mask==NULL) || mask[A[0]][A[1]][A[2]]) ) {
                f(&vIdx,&img,&A[0],seg,fData);
            }
        };
        
        // If streamline has many points and segments
        for (int i=0; i<len-1; i++) {

            // disp(MSG_DEBUG,"Segment: %d", i);

            // End of segment in real and its corner in image space
            img.to_ijk(streamline[i+1],p1);
            for (int m=0;m<3;m++) {
                seg.p[m]    = streamline[i][m];
                seg.dir[m]  = streamline[i+1][m] - streamline[i][m];
                B[m]        = std::round(p1[m]);
            }    
            
            // Find segment length and direction in real space
            lengthR = norm(seg.dir);
            for (int m=0;m<3;m++)
                seg.dir[m] /= lengthR;        
            
            // Segment does not leave the voxel, add this segment and continue with the next one
            if ( (A[0]==B[0]) && (A[1]==B[1]) && (A[2]==B[2]) ) {
                
                seg.length = lengthR;
                runFun();
                
                for (int m=0;m<3;m++)
                    p0[m] = p1[m];
                
                continue;
                
            }
            
            // Find length and direction of segment in image space
            for (int m=0;m<3;m++)
                dir[m] = p1[m] - p0[m];
            length = norm(dir);
            for (int m=0;m<3;m++)
                dir[m] /= length;

            // Grid lengthScale
            lengthScale = std::sqrt(dir[0]*img.pixDims[0]*dir[0]*img.pixDims[0]+dir[1]*img.pixDims[1]*dir[1]*img.pixDims[1]+dir[2]*img.pixDims[2]*dir[2]*img.pixDims[2]);
            
            // Keep the original segment beginning point length
            auto& sP = streamline[i];
            float sL = lengthR;

            auto pushSegment = [&](float pushAmount)->bool {

                // Update segment length and run the function
                seg.length = pushAmount*lengthScale;
                runFun();

                length    -= pushAmount;               // Update remaining length in image space
                lengthR   -= pushAmount*lengthScale;   // Update remaining length in real space
                
                for (int m=0;m<3;m++) {     
                    p0[m]   += pushAmount*dir[m];               // Move the initial point of the segment in image space
                    A[m]     = std::round(p0[m]);               // Update the current voxel
                    seg.p[m] = sP[m]+(sL-lengthR)*seg.dir[m];   // Move the initial point of the segment in real space // Is seg.p[m] even needed?
                }

                if ( (A[0]==B[0]) && (A[1]==B[1]) && (A[2]==B[2]) ) {

                    // Update segment length and run the function
                    seg.length = lengthR;
                    runFun();
                    
                    return true;
                }

                return false;

            };

            
            double t = 0.0;

            while (length > 0.0) {
            
                if (rayTraceVoxel(A,p0,dir,t)) {

                    // Cut t at length
                    t = std::min(t,length);

                    if(pushSegment(t))
                        break;

                }
                
                // Push the segment by EPS4 or by length. This does not introduce any errors. 
                // It is only manually moving the point instead of the ray-tracer since ray-tracing stops exactly at the voxel edge
                if (pushSegment(std::min(EPS4,length)))
                    break;
                            
            }

            for (int m=0;m<3;m++) {
                p0[m] = p1[m];
                A[m]  = B[m];
            }
            

        }

        for (int l=0; l<len; l++)
            delete[] streamline[l];
        delete[] streamline;
        
        // disp(MSG_DEBUG,"vIdx size: %d", vIdx.size());

        return vIdx;

        
    }

}