#pragma once

#include "base/nibr.h"
#include "math/core.h"
#include "dMRI/tractography/io/tractogramReader.h"
#include "dMRI/tractography/mappers/tractogram2surfaceMapper.h"
#include "image/image.h"
#include "surface/surface.h"
#include <set>

namespace NIBR 
{

    // Structural connectivity surface indexer
    class SCsurfaceIndexer {

    public:    
        SCsurfaceIndexer(NIBR::TractogramReader* _tractogram, NIBR::Surface* _surf, NIBR::SurfaceField *_surfLabels);
        ~SCsurfaceIndexer();
        
        void run();
        
        void setEndLength(float el)     { endLengthThresh = el; }
        void addBackgroundLabel(int bg) { bgLabels.insert(bg);  }
        
        bool writeConn(std::string outFname);    

    private:

        bool processStreamline(int _streamlineId, uint16_t _threadNo);
        bool isBg(int val);

        NIBR::TractogramReader*   tractogram;
        NIBR::Surface*            surf;
        NIBR::SurfaceField*       surfLabels;
        std::vector<int>    faceLabels;                 // This is used to check the label. If input label is VERTEX then we create labels on FACEs.

        std::vector<std::vector<NIBR::streamline2faceMap>> tract2surfMap;

        float               endLengthThresh;

        std::set<int>       bgLabels;                    // labels to ignore
        int                 bgVal;

        std::vector<int> original_labels;                // labels fetched from the image
        std::vector<int> labels;                         // modified labels for faster vector access
        std::vector<std::vector<std::set<size_t>>> conn; // indices of streamlines
        size_t labelCnt;

    };

}
