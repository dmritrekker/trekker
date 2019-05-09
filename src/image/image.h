#ifndef SRC_IMAGE_H_
#define SRC_IMAGE_H_

#include <iostream>
#include <iomanip>
#include <string>

#include "../config/config_general.h"
#include "../image/voxel.h"
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
	bool 		  prepInterp(float *p, int *cor_ijk, float *volFrac);

	void 		  getVal(float *p, float* out);

	nifti_image  *nim;
	Voxel   	 *voxels;
	unsigned int  zp_sxy;
	unsigned int  zp_sxyz;

protected:
	void  		  readHeader_detail();

	std::string   filePath;
	float 	     *data;
	float         voxelVolume;
	float 	     *xyz2ijk;

	// The following are useful for trilinear interpolation
	bool  		  indexVoxels();
	unsigned int  sx;
	unsigned int  sxy;
	unsigned int  sxyz;
	unsigned int  zp_sx;

};

#endif
