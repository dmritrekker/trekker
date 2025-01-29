#pragma once

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <float.h>
#include "base/nibr.h"
#include "math/core.h"
#include "image/image.h"


namespace NIBR
{

    struct trkFileStruct {

        char            id_string[6];
        short           dim[3];
        float           voxel_size[3];
        float           origin[3];
        short           n_scalars;
        char            scalar_name[10][20];
        short           n_properties;
        char            property_name[10][20];
        float           vox_to_ras[4][4];
        char            reserved[444];
        char            voxel_order[4];
        char            pad2[4];
        float           image_orientation_patient[6];
        char            pad1[2];
        unsigned char   invert_x;
        unsigned char   invert_y;
        unsigned char   invert_z;
        unsigned char   swap_xy;
        unsigned char   swap_yz;
        unsigned char   swap_zx;
        int             n_count;
        int             version;
        int             hdr_size;

    };

    struct Segment {
        int    streamlineNo;
        float  p[3];
        float  dir[3];
        float  length;
        void*  data;
    };

    typedef enum {
        VTK_ASCII,
        VTK_BINARY,
        TCK,
        TRK
    } TRACTOGRAMFILEFORMAT;

    class TractogramReader {
        
    public:
        
        TractogramReader();
        TractogramReader(std::string _fileName);
        ~TractogramReader();
        TractogramReader(const TractogramReader& obj);
        void copyFrom(const TractogramReader& obj);
        void destroyCopy();
        void printInfo();
        
        bool     initReader(std::string _fileName);
        void     setThreadId (uint32_t _threadId) {threadId=_threadId;}
        uint32_t getThreadId () {return threadId;}

        template<typename T>
        void     setReferenceImage(Image<T>* ref) 
        {
            for(int i=0;i<3;++i) {
                for(int j=0;j<4;++j) {
                    ijk2xyz[i][j] = ref->ijk2xyz[i][j];
                    xyz2ijk[i][j] = ref->xyz2ijk[i][j];
                }
            }

            for(int j=0;j<3;++j) {
                ijk2xyz[3][j] = 0;
                xyz2ijk[3][j] = 0;
                imgDims[j]    = ref->imgDims[j];
                pixDims[j]    = ref->pixDims[j];
            }

            ijk2xyz[3][3] = 1;
            xyz2ijk[3][3] = 1;

            std::strcpy(voxOrdr,"LAS");             // TODO: Compute this properly and not assume LAS.

        }
        
        std::vector<std::vector<std::vector<float>>>    read(); // Reads all the streamlines
        void                                            readPoint(size_t n, uint32_t l, float* p);
        float**                                         readStreamline(size_t n);
        std::vector<Point>                              readStreamlinePoints(size_t n);
        std::vector<std::vector<float>>                 readStreamlineVector(size_t n);

        FILE                   *file;
        std::string             fileName;
        std::string             fileDescription;
        TRACTOGRAMFILEFORMAT    fileFormat;
        
        size_t                  numberOfPoints;
        size_t                  numberOfStreamlines;
        uint32_t*               len;

        // Nifti assumes voxel center to be 0,0,0.
        // TRK assumes 0,0,0 to be one of the corners. But which corner? 
        // That depends on the image orientation, which makes it difficult to compute.
        // The ijk2xyz and xyz2ijk belong to the reference image.
        // TODO: For now, we will assume that the reference image was in LAS orientation. This needs to be properly computed in the future.
        // For example using the approach in Nipy: https://github.com/nipy/nibabel/blob/d23a8336582d07d683d3af73cf097c3be2de9af8/nibabel/orientations.py#L356
        //
        // With LAS assumption: 
        // - when reading a trk file, we will subtract 0.5 from all values
        // - when writing a trk file, we will add 0.5 to all values

        short                   imgDims[3];
        float                   pixDims[3];
        char                    voxOrdr[4];         // We assume this to be LAS for now
        float                   ijk2xyz[4][4];
        float                   xyz2ijk[4][4];
        
        long*                   streamlinePos;      // file positions for first points of streamlines

    private:
        uint32_t                threadId;

        // TRK specific
        short                   n_scalars_trk;      // TRK file format extension
        short                   n_properties_trk;   // TRK file format extension

    };

}


