#pragma once

#include "base/nibr.h"
#include "math/core.h"
#include "dMRI/tractography/io/tractogramReader.h"
#include "image/image.h"
#include <set>

namespace NIBR 
{

    typedef enum {
        END_POINT_LABEL,
        LONGEST_END_LABEL,
        FIRST_NONE_BG_LABEL,
        LAST_NONE_BG_LABEL
    } SCEndType;

    // Structural connectivity indexer
    class SCimageIndexer {

    public:    
        SCimageIndexer(NIBR::TractogramReader* _tractogram, NIBR::Image<int>* _img);
        ~SCimageIndexer();
        

        void run();
        
        void setEndLength(float el)     { endLengthThresh = el; }
        void setEndType(SCEndType et)   { endType = et;         }
        void addBackgroundLabel(int bg) { bgLabels.insert(bg);  }

        bool writeConn(std::string outFname);    

    private:

        bool processStreamline(int _streamlineId, uint16_t _threadNo);
        bool isBg(int val);

        NIBR::TractogramReader* tractogram;
        NIBR::Image<int>* img;

        float       endLengthThresh;
        SCEndType   endType;

        std::set<int>    bgLabels;                       // labels to ignore
        int              bgVal;

        std::vector<int> original_labels;                // labels fetched from the image
        std::vector<int> labels;                         // modified labels for faster vector access
        std::vector<std::vector<std::set<size_t>>> conn; // indices of streamlines
        size_t labelCnt;

    };

}
