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

	float x = ijk2xyz[0][0]*i + ijk2xyz[0][1]*j + ijk2xyz[0][2]*k + ijk2xyz[0][3];
	float y = ijk2xyz[1][0]*i + ijk2xyz[1][1]*j + ijk2xyz[1][2]*k + ijk2xyz[1][3];
	float z = ijk2xyz[2][0]*i + ijk2xyz[2][1]*j + ijk2xyz[2][2]*k + ijk2xyz[2][3];

	return Coordinate(x,y,z);
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
        
            if (labelFlag) {
                if (val==label) {
                    data[task.no]    = new float[1];
                    data[task.no][0] = 1;
                    MT::proc_mx.lock();
                    nnzVoxelInds.push_back(task.no);
                    MT::proc_mx.unlock();
                } else 
                    data[task.no] = zero;
            } 
            else if (val>0) {
                data[task.no]    = new float[1];
                data[task.no][0] = 1;
                MT::proc_mx.lock();
                nnzVoxelInds.push_back(task.no);
                MT::proc_mx.unlock();
            } else 
                data[task.no] = zero;
            
        }
        
    });

    
	nifti_image_unload(nim);
	delete accessor;

	status = notDone;

	nim->nt = 1; //Force the forth dimension to be 1 so indexing will not crash if an 3D image with nim->nt=0 comes

	this->Image::indexVoxels();

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

