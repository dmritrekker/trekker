#ifndef SRC_IMAGE_VOXEL_H_
#define SRC_IMAGE_VOXEL_H_

#include <cstddef>
#include <string.h>

class Voxel  {

public:

	Voxel();
	~Voxel();

	void init(unsigned int dimensionOfData);

	float  		 *val;
	float 		**box;

};



#endif
