#ifndef SRC_IMAGE_FOD_IMAGE_H_
#define SRC_IMAGE_FOD_IMAGE_H_

#include "../config/config_general.h"
#include "../image/image.h"
#include "vector"

class FOD_Image : public Image {
public:
	FOD_Image();
    void         cleanFODImage();
	bool  		 readImage();
	int 		 getSHorder();
	float 		 getSmallestPixdim();

	bool 		 iseven; // True if it is even ordered
	
	bool         discretizationFlag;
	bool         isspheresliced;
    std::string  sphereFileName;
    
    int          vertexCoord2volInd(float* vertexCoord);
    float        getFODval(float *p, float* T);
    
    static void  orderDirections(float* unit_dir);
    
    std::vector<size_t>     nnzVoxelInds;
    std::vector<size_t>     nnzVoxelReInds;
    
    int                     discVolSphDim;
    int*                    discVolSphInds;
    std::vector<Coordinate> discVolSphCoords;
    float                   discVolSphRadius;
    float                   discVolSphShift;
    
    void                    fillDiscVolSph();
    
};

#endif
