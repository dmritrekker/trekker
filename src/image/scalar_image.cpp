#include "scalar_image.h"

bool SCALAR_Image::readImage() {

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

    // all zero valued voxels are pointed to here
    zero     = new float[1];
    zero[0]  = 0;
    
    // Copy everything in a float array with dimension 1
    data    = new float*[sxyz];
    MT::MTRUN(sxyz, sxyz/16, MT::maxNumberOfThreads,[&](MTTASK task)->void {
        float val = accessor->get(nim->data,task.no);
        if (val==0) {
            data[task.no] = zero;
        }
        else {
            data[task.no]    = new float[1];
            data[task.no][0] = val;
        }
    });

    
	nifti_image_unload(nim);
	delete accessor;

	nim->nt = 1; //Force the forth dimension to be 1 so indexing will not crash if an 3D image with nim->nt=0 comes

	this->Image::indexVoxels();

	return true;

}
