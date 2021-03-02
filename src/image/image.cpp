#include "../image/image.h"

NiftiDataAccessor::~NiftiDataAccessor() { }

Image::Image() {
	filePath 		= "";
	nim 			= NULL;
 	data 			= NULL;
    dims            = NULL;
    pixDims         = NULL;
    normalizedPixDims = NULL;
	xyz2ijk 		= NULL;
    ijk2xyz         = NULL;
    zs2i            = NULL;

	sx 				= 0;
	sxy 			= 0;
	sxyz 			= 0;
	voxels 			= NULL;
    zero            = NULL;
}

Image::Image(const Image& obj) {
	filePath 		= obj.filePath;
	nim 			= obj.nim;
	data 			= obj.data;
    dims 			= obj.dims;
    pixDims 		= obj.pixDims;
    normalizedPixDims = obj.normalizedPixDims;
	xyz2ijk 		= obj.xyz2ijk;
    ijk2xyz 		= obj.ijk2xyz;

	sx 				= obj.sx;
	sxy 			= obj.sxy;
	sxyz 			= obj.sxyz;
	voxels 			= obj.voxels;
    zero            = obj.zero;
    zs2i            = obj.zs2i;
}

Image::~Image() {
    
    if (data!=NULL) {
        for (size_t i=0; i<sxyz; i++) 
            if (data[i]!=zero)
                delete[] data[i];
        delete[] data;
    }
    
    if (nim!=NULL)              nifti_image_free(nim);
	
    if (dims!=NULL)			    delete[] dims;
    if (pixDims!=NULL)			delete[] pixDims;
    if (normalizedPixDims!=NULL)			delete[] normalizedPixDims;
    if (zs2i!=NULL)			delete[] zs2i;
    
    if (xyz2ijk!=NULL) {
        delete[] xyz2ijk[0];
        delete[] xyz2ijk[1];
        delete[] xyz2ijk[2];
        delete[] xyz2ijk;
    }
    
    
    if (ijk2xyz!=NULL) {
        delete[] ijk2xyz[0];
        delete[] ijk2xyz[1];
        delete[] ijk2xyz[2];
        delete[] ijk2xyz;
    }
    
    if (voxels!=NULL) {
        for (size_t i=0; i<zs2i[2]; i++) delete[] voxels[i];
        delete[] voxels;
    }
    
    if (zero!=NULL)			    free(zero);
    
}

void Image::destroyCopy(){
	filePath.erase();
	data 			= NULL;
	nim 			= NULL;
    dims            = NULL;
    pixDims         = NULL;
	xyz2ijk 		= NULL;
    ijk2xyz         = NULL;
	voxels 			= NULL;
    zero            = NULL;
    normalizedPixDims = NULL;
    zs2i            = NULL;
}

bool Image::readHeader(char* _filePath) {

	filePath = std::string(_filePath);

	nim = nifti_image_read(filePath.c_str(),0);
	if (nim==NULL) exit(EXIT_FAILURE);

	switch (nim->datatype) {
	case 0:    std::cout<<"Nifti datatype: unknown "       	  << std::endl; std::cout << "is not an accepted datatype" << std::endl; return false; break;
	case 1:    std::cout<<"Nifti datatype: bool, binary "     << std::endl; std::cout << "is not an accepted datatype" << std::endl; return false; break;
	case 2:    break;
	case 4:    break;
	case 8:    break;
	case 16:   break;
	case 32:   std::cout<<"Nifti datatype: complex64 "        << std::endl; std::cout << "is not an accepted datatype" << std::endl; return false; break;
	case 64:   break;
	case 128:  std::cout<<"Nifti datatype: rgb24 "            << std::endl; std::cout << "is not an accepted datatype" << std::endl; return false; break;
	case 255:  std::cout<<"Nifti datatype: all? "             << std::endl; std::cout << "is not an accepted datatype" << std::endl; return false; break;
	case 256:  break;
	case 512:  break;
	case 768:  break;
	case 1024: break;
	case 1280: break;
	case 1536: break;
	case 1792: std::cout<<"Nifti datatype: complex128 "       << std::endl; std::cout << "is not an accepted datatype" << std::endl; return false; break;
	case 2048: std::cout<<"Nifti datatype: complex256 "       << std::endl; std::cout << "is not an accepted datatype" << std::endl; return false; break;
	case 2304: std::cout<<"Nifti datatype: rgba32 "           << std::endl; std::cout << "is not an accepted datatype" << std::endl; return false; break;
	default:   std::cout<<"Nifti datatype: Not applicable "   << std::endl; std::cout << "is not an accepted datatype" << std::endl; return false; break;
	}

    // Get dims and pixDims
    dims    = new float[3];
    dims[0] = nim->dim[1]; // nx
    dims[1] = nim->dim[2]; // ny
    dims[2] = nim->dim[3]; // nz
    
    pixDims    = new float[3];
    pixDims[0] = nim->pixdim[1]; // dx
    pixDims[1] = nim->pixdim[2]; // dy
    pixDims[2] = nim->pixdim[3]; // dz
    
    normalizedPixDims    = new float[3];
    float voxelVolumeNormalizer = 1.0/std::pow(pixDims[0]*pixDims[1]*pixDims[2],1.0f/3.0f);
    normalizedPixDims[0] = pixDims[0]*voxelVolumeNormalizer;
    normalizedPixDims[1] = pixDims[1]*voxelVolumeNormalizer;
    normalizedPixDims[2] = pixDims[2]*voxelVolumeNormalizer;
    
	// Choose between sform or qform
    xyz2ijk     = new float*[3];
    xyz2ijk[0]  = new float[4];
    xyz2ijk[1]  = new float[4];
    xyz2ijk[2]  = new float[4];
    
    ijk2xyz     = new float*[3];
    ijk2xyz[0]  = new float[4];
    ijk2xyz[1]  = new float[4];
    ijk2xyz[2]  = new float[4];
    
    
    // Use sform if possible otherwise use qform
    if (nim->sform_code>0) {        
        for (int i=0; i<3; i++)
            for (int j=0; j<4; j++) {
                xyz2ijk[i][j] = nim->sto_ijk.m[i][j];
                ijk2xyz[i][j] = nim->sto_xyz.m[i][j];
            }
	}
	else {
        for (int i=0; i<3; i++)
            for (int j=0; j<4; j++) {
                xyz2ijk[i][j] = nim->qto_ijk.m[i][j];
                ijk2xyz[i][j] = nim->qto_xyz.m[i][j];
            }
	}
    
	this->readHeader_detail();

	return true;

}

void Image::readHeader_detail() {

	// Used for sub2ind
	sx 			= nim->nx;
	sxy 		= nim->nx*nim->ny;
	sxyz 		= nim->nx*nim->ny*nim->nz;

    zs2i        = new size_t[3];
	zs2i[0] 	= (nim->nx+2);
	zs2i[1] 	= (nim->nx+2)*(nim->ny+2);
	zs2i[2] 	= (nim->nx+2)*(nim->ny+2)*(nim->nz+2);

}

std::string Image::getUnit() {
	std::string unit;
	switch (nim->xyz_units) {
	case 1:    unit = "meter";        break;
	case 2:    unit = "mm";           break;
	case 3:    unit = "micron";       break;
	default:   unit = "unknown unit"; break;
	}
	return unit;
}

void Image::printInfo() {

	std::cout << std::endl;
	std::cout << "Data description:       " << nim->descrip << std::endl;
	std::cout << "Nifti type:             ";
	switch (nim->nifti_type) {
	case 0:    std::cout<<"Analyze"                          << std::endl; break;
	case 1:    std::cout<<"NIFTI-1 (1 file)"                 << std::endl; break;
	case 2:    std::cout<<"NIFTI-2 (2 file)"                 << std::endl; break;
	case 3:    std::cout<<"NIFTI-ASCII (1 file)"             << std::endl; break;
	default:   std::cout<<"Not applicable"                   << std::endl; break;
	}

	std::cout << "Number of dimensions:   "   << nim->dim[0] << std::endl;
	std::cout << "Dimensions:             [";
	for (int i=1; i<(nim->dim[0]+1); i++) {
		std::cout << nim->dim[i];
		if (i!=nim->dim[0])
			std::cout << " x ";
	}
	std::cout << "]" << std::endl;

	std::cout << "Number of bytes/voxel:  "   << nim->nbyper << std::endl;

	std::cout << "Pixdim:                 [" << nim->pixdim[1] << " x " << nim->pixdim[2] << " x " << nim->pixdim[3] << "]" << std::endl;

	std::cout << "Unit of pixdim:         ";
	switch (nim->xyz_units) {
	case 0:    std::cout<<"unknown"                          << std::endl; break;
	case 1:    std::cout<<"meter"                            << std::endl; break;
	case 2:    std::cout<<"mm"                               << std::endl; break;
	case 3:    std::cout<<"micron"                           << std::endl; break;
	default:   std::cout<<"Not applicable"                   << std::endl; break;
	}

	std::cout << "Datatype:               " ;
	switch (nim->datatype) {
	case 2:    std::cout<<"uint8, unsigned char"             << std::endl; break;
	case 4:    std::cout<<"int16, signed short"              << std::endl; break;
	case 8:    std::cout<<"int32, signed int"                << std::endl; break;
	case 16:   std::cout<<"float32"                          << std::endl; break;
	case 64:   std::cout<<"float64, double"                  << std::endl; break;
	case 256:  std::cout<<"int8, signed char"                << std::endl; break;
	case 512:  std::cout<<"uint16, unsigned short"           << std::endl; break;
	case 768:  std::cout<<"uint32, unsigned int"             << std::endl; break;
	case 1024: std::cout<<"int64, long long int"             << std::endl; break;
	case 1280: std::cout<<"uint64, unsigned long long int"   << std::endl; break;
	case 1536: std::cout<<"float128, long double"            << std::endl; break;
	}

}

bool Image::indexVoxels() {

	// voxels are indexed on a zero padded image in order to avoid boundary checking
	voxels = new float**[zs2i[2]];
    
	size_t index = 0;
	if (GENERAL::verboseLevel!=QUITE) std::cout << "Indexing voxels: 0%" << '\r' << std::flush;
    
	for (int z=0; z<(nim->nz+2); z++) {
		for (int y=0; y<(nim->ny+2); y++) {
			for (int x=0; x<(nim->nx+2); x++) {
                
                voxels[index] = new float*[8];
                voxels[index][0]=((x<1)||(y<1)||(z<1)||(x>(nim->nx)  )||(y>(nim->ny)  )||(z>(nim->nz))  ) ? zero : data[(x-1)+(y-1)*sx+(z-1)*sxy];
                voxels[index][1]=((x<0)||(y<1)||(z<1)||(x>(nim->nx)-1)||(y>(nim->ny)  )||(z>(nim->nz))  ) ? zero : data[(x)  +(y-1)*sx+(z-1)*sxy];
                voxels[index][2]=((x<1)||(y<0)||(z<1)||(x>(nim->nx)  )||(y>(nim->ny)-1)||(z>(nim->nz))  ) ? zero : data[(x-1)+(y)*sx  +(z-1)*sxy];
                voxels[index][3]=((x<0)||(y<0)||(z<1)||(x>(nim->nx)-1)||(y>(nim->ny)-1)||(z>(nim->nz))  ) ? zero : data[(x)  +(y)*sx  +(z-1)*sxy];
                voxels[index][4]=((x<1)||(y<1)||(z<0)||(x>(nim->nx)  )||(y>(nim->ny)  )||(z>(nim->nz)-1)) ? zero : data[(x-1)+(y-1)*sx+(z)*sxy  ];
                voxels[index][5]=((x<0)||(y<1)||(z<0)||(x>(nim->nx)-1)||(y>(nim->ny)  )||(z>(nim->nz)-1)) ? zero : data[(x)  +(y-1)*sx+(z)*sxy  ];
                voxels[index][6]=((x<1)||(y<0)||(z<0)||(x>(nim->nx)  )||(y>(nim->ny)-1)||(z>(nim->nz)-1)) ? zero : data[(x-1)+(y)*sx  +(z)*sxy  ];
                voxels[index][7]=((x<0)||(y<0)||(z<0)||(x>(nim->nx)-1)||(y>(nim->ny)-1)||(z>(nim->nz)-1)) ? zero : data[(x)  +(y)*sx  +(z)*sxy  ];
                
				index++;
			}
		}
		if (GENERAL::verboseLevel!=QUITE) std::cout << "Indexing voxels: " << (size_t)((index/(float)(zs2i[2]-1))*100) << "%" << '\r' << std::flush;
	}

	if (GENERAL::verboseLevel!=QUITE) std::cout << "Indexing voxels: 100%" << '\r' << std::flush;
	if (GENERAL::verboseLevel!=QUITE) std::cout << std::endl;

	return true;
}

// Checked on the non-zero padded image
unsigned char Image::checkImageBounds(float i, float j, float k) {

	// Interior of the image is between [-0.5,nim->nx-0.5] since voxel indices show centers

	// inside the image
	if ( (i >= 0) && (j >= 0) && (k >= 0) ) {
		// Case 1: inside the image [0,nim->nx-1) , i.e.: (nim->nx-2)+1 is a valid index
		if ( (i < (nim->nx-1)) && (j < (nim->ny-1)) && (k < (nim->nz-1)) )
			return 1;
		// Case 2: inside the image [0,nim->nx-1] , i.e.: (nim->nx-1)+1 is NOT a valid index
		else if ( (i <= (nim->nx-1)) && (j <= (nim->ny-1)) && (k <= (nim->nz-1)) )
			return 2;
	}

	// outside the image [-0.5,nim->nx-0.5]
	if ( (i < -0.5) || (j < -0.5) || (k < -0.5) || (i > (nim->nx-0.5)) || (j > (nim->ny-0.5)) || (k > (nim->nz-0.5)) )
		return 0;

	// on the boundary
	return 3;

}

// Returns 0 if outside image
unsigned char Image::checkWorldBounds(float x, float y, float z) {

	float i = xyz2ijk[0][0]*x + xyz2ijk[0][1]*y + xyz2ijk[0][2]*z + xyz2ijk[0][3];
	if ( (i<-0.5) || (i>nim->nx-0.5)) return 0;

	float j = xyz2ijk[1][0]*x + xyz2ijk[1][1]*y + xyz2ijk[1][2]*z + xyz2ijk[1][3];
	if ( (j<-0.5) || (j>nim->ny-0.5)) return 0;

	float k = xyz2ijk[2][0]*x + xyz2ijk[2][1]*y + xyz2ijk[2][2]*z + xyz2ijk[2][3];
	if ( (k<-0.5) || (k>nim->nz-0.5)) return 0;

	return 1;
}

void Image::getVal(float *p, float* out) {
    
    float ijk;
    int cor_ijk[3];
    float iwa[3], iwb[3];
    
    for (int i=0; i<3; i++) {
        
		ijk 	    = xyz2ijk[i][0]*p[0] + xyz2ijk[i][1]*p[1] + xyz2ijk[i][2]*p[2] + xyz2ijk[i][3] + 1;
		cor_ijk[i] 	= int(ijk);
        
        if ( (cor_ijk[i] < 0) || (cor_ijk[i] > dims[i]) ) {
            memset(out,0,(nim->nt)*sizeof(float));
            return;
        }
		
        iwa[i] 		= (ijk-cor_ijk[i])*normalizedPixDims[i];
		iwb[i] 		= normalizedPixDims[i]-iwa[i];
        
	}
    
	float **vals = voxels[cor_ijk[0] + cor_ijk[1]*zs2i[0] + cor_ijk[2]*zs2i[1]];
    
	for (int c=0; c<(nim->nt); c++) {
		out[c] =iwb[0]*iwb[1]*iwb[2]*vals[0][c] +
				iwa[0]*iwb[1]*iwb[2]*vals[1][c] +
				iwb[0]*iwa[1]*iwb[2]*vals[2][c] +
				iwa[0]*iwa[1]*iwb[2]*vals[3][c] +
				iwb[0]*iwb[1]*iwa[2]*vals[4][c] +
				iwa[0]*iwb[1]*iwa[2]*vals[5][c] +
				iwb[0]*iwa[1]*iwa[2]*vals[6][c] +
				iwa[0]*iwa[1]*iwa[2]*vals[7][c];
	}
    
}
