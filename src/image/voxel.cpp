#include "../image/voxel.h"

Voxel::Voxel() {
	box  = new float*[8];
}

Voxel::~Voxel() {
	delete[] box;
}
