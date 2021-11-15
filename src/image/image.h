#ifndef SRC_IMAGE_H_
#define SRC_IMAGE_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <assert.h>
#include "vector"

#include "../config/config_general.h"
#include "../nifticlib-2.0.0/niftilib/nifti1_io.h"
#include "../math/coordinate.h"

class NiftiDataAccessor{
public:
	virtual float get(void *_data, unsigned int index) = 0;
	virtual void  set(void *_data, unsigned int index, float value) = 0;
	virtual ~NiftiDataAccessor() = 0;
};

template<class T> class NiftiDataAccessor_ForType : public NiftiDataAccessor {
public:
	virtual float get(void *_data, unsigned int index) {
		return ((T*) _data)[index];
	}
	virtual void  set(void *_data, unsigned int index, float value) {
		((T*) _data)[index] = (T)value;
	}

};


class Image {

public:
	Image();
	Image(const Image& obj);
	~Image();

	bool readHeader(char* _filePath);
	void printInfo();

	std::string   getFilePath() {return filePath;};
	nifti_image*  getNim() 		{return nim;};
	std::string   getUnit();

	void          destroyCopy();
	unsigned char checkImageBounds(float i, float j, float k);
	unsigned char checkWorldBounds(float x, float y, float z);

	void 		  getVal(float *p, float* out);

	nifti_image  *nim;
    size_t       *zs2i;
    float        *zero;

	void  		  readHeader_detail();

    float**       data;
    float***      voxels;
    
    std::vector<size_t> nnzVoxelInds;
    
	std::string   filePath;
	float**       xyz2ijk;
    float**       ijk2xyz;

	// The following are useful for trilinear interpolation
	bool  		  indexVoxels();
	size_t        sx;
	size_t        sxy;
	size_t        sxyz;
    
    float*        dims;
    float*        pixDims;
    
    // Interpolation related variables
    float*        normalizedPixDims;
};

#endif
