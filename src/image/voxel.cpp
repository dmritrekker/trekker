#include "../image/voxel.h"

Voxel::Voxel() {
	val  = NULL;
	box  = NULL;
}

Voxel::~Voxel() {
	delete[] val;
	delete[] box;
}

void Voxel::init(unsigned int dimensionOfData) {
	val 	= new float[dimensionOfData];
	memset(val,0.0,dimensionOfData*sizeof(float));

	box  	= new float*[8];
	memset(box,0,8*sizeof(float*));
}
