#include "../image/fod_image.h"

FOD_Image::FOD_Image() {
	iseven = true;
}

FOD_Image::~FOD_Image() { }

float FOD_Image::getSmallestPixdim() {
	float smallestPixelDim = 1e10;
	for (int i=1; i<4; i++) {
		if (nim->pixdim[i]<smallestPixelDim)
			smallestPixelDim = nim->pixdim[i];
	}
	return smallestPixelDim;
}

int FOD_Image::getSHorder() {

	int order = sqrt(nim->nt);
	if ((order*order)!=nim->nt) {
		if (GENERAL::verboseLevel>ON) std::cout << "Symmetric FOD with order " << (sqrt(8*nim->nt+1)-3)/2 << std::endl << std::flush;
		return (sqrt(8*nim->nt+1)-3)/2;
	}
	else {
		if (GENERAL::verboseLevel>ON) std::cout << "Asymmetric FOD with order " << sqrt(nim->nt)-1 << std::endl << std::flush;
		iseven = false;
		return order-1;
	}

}

bool FOD_Image::readImage() {

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
	float progress = 0;
	if (GENERAL::verboseLevel!=QUITE) std::cout << "Loading FOD image: 0%" << '\r' << std::flush;
	for(unsigned int i=0; i<nim->nvox; i++) {
		*(data+i) = accessor->get(nim->data,i);
		if ((i/(float)(nim->nvox-1))*100 > progress) {
			if (GENERAL::verboseLevel!=QUITE) std::cout << "Loading FOD image: " << progress << "%" << '\r' << std::flush;
			progress++;
		}
	}
	if (GENERAL::verboseLevel!=QUITE) std::cout << "Loading FOD image: 100%" << '\r' << std::flush;
	if (GENERAL::verboseLevel!=QUITE) std::cout << std::endl;

	nifti_image_unload(nim);
	delete accessor;

	this->Image::indexVoxels();

	delete[] data;
	data = NULL;

	return true;
}
