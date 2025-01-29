#pragma once

#include "base/nibr.h"
#include "math/core.h"
#include "math/sphericalHarmonics.h"
#include "image/image.h"
#include "vector"

namespace NIBR
{

    class FOD_Image : public NIBR::Image<float> {
        
    public:
        FOD_Image();
        FOD_Image(std::string _filePath);
        FOD_Image(const char* _filePath) {FOD_Image(std::string(_filePath));}

        FOD_Image(std::string _filePath,std::string _sphereFilePath);
        FOD_Image(const char* _filePath,const char* _sphereFilePath) {FOD_Image(std::string(_filePath), std::string(_sphereFilePath));}


        ~FOD_Image();
        
        bool    read();
        float   getFODamp(float *p, float* tan) { return fodAmp(this,p,tan);}
        int     getSHorder() {return shOrder;}
        
        void                            setOrderOfDirections(NIBR::OrderOfDirections ord) {orderOfDirections=ord;}
        NIBR::OrderOfDirections         getOrderOfDirections()                      {return orderOfDirections;}  
        
        void                            setSHprecomputationResolution(int num)  {SHprecomputationResolution=num;}
        int                             getSHprecomputationResolution()         {return SHprecomputationResolution;}  
        
        void                            setdiscretizationFlag(bool f) {discretizationFlag=f;}
        bool                            getdiscretizationFlag()       {return discretizationFlag;}

        bool                            getIsSphereSliced() {return isspheresliced;}        
        std::string                     getSphereFileName() {return sphereFileName;}

        void                            fodIsAsym()   {iseven = false;}
        void                            fodIsSym()    {iseven = false;}
        bool                            isFodAsym()   {return !iseven;}
        bool                            isFodsym()    {return  iseven;}

        std::vector<std::vector<float>> getCoordinates() { return isspheresliced ? sphereCoords : discVolSphCoords;}


    private:
        bool                                iseven;
        int                                 shOrder;
        int                                 SHprecomputationResolution;
        NIBR::OrderOfDirections             orderOfDirections;
        bool                                discretizationFlag;
        void                                setSHorder();
        
        int                                 discVolSphDim;
        int*                                discVolSphInds;
        std::vector<std::vector<float>>     discVolSphCoords;
        float                               discVolSphRadius;
        float                               discVolSphShift;
        
        void                                fillDiscVolSph();
        int64_t                             vertexCoord2volInd(float* vertexCoord);
        
        bool                                isspheresliced;
        std::string                         sphereFileName;
        std::vector<std::vector<float>>     sphereCoords;
        
        std::function<float(FOD_Image*, float *p, float* tan)> fodAmp;
        
        float ampWithDiscretizationON(float *p, float* tan);
        float ampWithDiscretizationOFF(float *p, float* tan);
        
    };

}
