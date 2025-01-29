#pragma once

#include "base/nibr.h"
#include "math/core.h"
#include "math/sphericalHarmonics.h"
#include "math/sphericalFunctions.h"
#include "image.h"
#include "vector"

namespace NIBR {

    class SF_Image : public NIBR::Image<float> {
        
    public:

        SF_Image();
        SF_Image(std::string _filePath, bool _isEven);
        SF_Image(const char* _filePath, bool _isEven) {SF_Image(std::string(_filePath),_isEven);}

        SF_Image(std::string _filePath, std::string _dirPath, bool _isEven);
        SF_Image(const char* _filePath, std::string _dirPath, bool _isEven) {SF_Image(std::string(_filePath),std::string(_dirPath),_isEven);}
        SF_Image(std::string _filePath, const char* _dirPath, bool _isEven) {SF_Image(std::string(_filePath),std::string(_dirPath),_isEven);}
        SF_Image(const char* _filePath, const char* _dirPath, bool _isEven) {SF_Image(std::string(_filePath),std::string(_dirPath),_isEven);}

        ~SF_Image();

        bool    read();
        float   getSFval(float *p, float* tan) {return (*this)(p,SF::coordinate2index(tan));} // Returns the value of the nearest neighbor without any interpolation
        void    smooth(float angle);

    private:

        bool                            isEven;
        bool                            hasDirections;
        int                             sphericalDomainResolution;
        std::vector<std::vector<float>> dirs;

    };

}

