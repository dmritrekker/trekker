#include "../image/voxel.h"

Voxel::Voxel() {
	val  = NULL;
	box  = new float*[8];
}

Voxel::~Voxel() {
	delete[] box;
}
