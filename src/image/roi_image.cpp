#include "../image/roi_image.h"

ROI_Image::ROI_Image(const ROI_Image& obj) : Image(obj) {
	side 					= obj.side;
	status 					= obj.status;
	entry_status 			= obj.entry_status;
	exit_status  			= obj.exit_status;
	type 		 			= obj.type;
	self 					= obj.self;
	labelFlag 				= obj.labelFlag;
	label 					= obj.label;
}

Coordinate ROI_Image::ind2phy(size_t index) {

	int i,j,k;
	ind2sub(index,i,j,k);

	float x = nim->sto_xyz.m[0][0]*i + nim->sto_xyz.m[0][1]*j + nim->sto_xyz.m[0][2]*k + nim->sto_xyz.m[0][3];
	float y = nim->sto_xyz.m[1][0]*i + nim->sto_xyz.m[1][1]*j + nim->sto_xyz.m[1][2]*k + nim->sto_xyz.m[1][3];
	float z = nim->sto_xyz.m[2][0]*i + nim->sto_xyz.m[2][1]*j + nim->sto_xyz.m[2][2]*k + nim->sto_xyz.m[2][3];

	return Coordinate(x,y,z);
}

float ROI_Image::getVal(size_t index) {

	int i,j,k;
	ind2sub(index,i,j,k);
	size_t index_zp = (i+1) + zp_sx*(j+1)+zp_sxy*(k+1);
	return voxels[index_zp].val[0]*voxelVolume; // Indexing voxels for interpolation divided by voxelVolume, so multiple here
}

void ROI_Image::ind2sub(size_t index, int& i, int& j, int& k) {
	i = index % nim->nx;
	j = (index - i)/nim->nx % nim->ny;
	k = ((index - i)/nim->nx-j)/nim->ny;
}

size_t ROI_Image::sub2ind(int i, int j, int k) {
	return i + nim->nx*(j+nim->ny*k);
}

bool ROI_Image::readImage() {

	if (nifti_image_load(nim)==-1) {
		std::cout<<"Cannot read image data from : " << filePath << std::endl;
		return false;
	}

	NiftiDataAccessor  *accessor = NULL;

	switch (nim->datatype) {
	case 2:    accessor = new NiftiDataAccessor_ForType<unsigned char>; break;
	case 4:    accessor = new NiftiDataAccessor_ForType<short>; break;
	case 8:    accessor = new NiftiDataAccessor_ForType<int>; break;
	case 16:   accessor = new NiftiDataAccessor_ForType<float>; break;
	case 64:   accessor = new NiftiDataAccessor_ForType<double>; break;
	case 256:  accessor = new NiftiDataAccessor_ForType<signed char>; break;
	case 512:  accessor = new NiftiDataAccessor_ForType<unsigned short>; break;
	case 768:  accessor = new NiftiDataAccessor_ForType<unsigned int>; break;
	case 1024: accessor = new NiftiDataAccessor_ForType<long long int>; break;
	case 1280: accessor = new NiftiDataAccessor_ForType<unsigned long long int>; break;
	case 1536: accessor = new NiftiDataAccessor_ForType<long double>; break;
	}

	data = new float[nim->nvox];

	if (labelFlag) {
		for(size_t i=0; i<nim->nvox; i++) {
			if (accessor->get(nim->data,i)==label)
				data[i] = 1;
			else
				data[i] = 0;
		}
	} else {
		for(size_t i=0; i<nim->nvox; i++) {
			if (accessor->get(nim->data,i)>0)
				data[i] = 1;
			else
				data[i] = 0;
		}
	}

	nifti_image_unload(nim);
	delete accessor;

	status = notDone;

	nim->nt = 1; //Force the forth dimension to be 1 so indexing will not crash if an 3D image with nim->nt=0 comes

	this->Image::indexVoxels();

	delete[] data;
	data = NULL;

	return true;

}


float ROI_Image::getVal(Coordinate phy) {

	float out;
	float p[3];
	p[0] = phy.x;
	p[1] = phy.y;
	p[2] = phy.z;
	this->Image::getVal(p,&out);
	return out;
}

