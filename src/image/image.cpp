#include "../image/image.h"

NiftiDataAccessor::~NiftiDataAccessor() { }

Image::Image() {
	filePath 		= "";
	nim 			= NULL;
	data 			= NULL;
	xyz2ijk 		= NULL;
	voxelVolume 	= 0.0;

	sx 				= 0;
	sxy 			= 0;
	sxyz 			= 0;
	voxels 			= NULL;
	zp_sx			= 0;
	zp_sxy 			= 0;
	zp_sxyz			= 0;
    zero            = NULL;
}

Image::Image(const Image& obj) {
	filePath 		= obj.filePath;
	nim 			= obj.nim;
	data 			= obj.data;
	xyz2ijk 		= obj.xyz2ijk;
	voxelVolume 	= obj.voxelVolume;

	sx 				= obj.sx;
	sxy 			= obj.sxy;
	sxyz 			= obj.sxyz;
	voxels 			= obj.voxels;
	zp_sx 			= obj.zp_sx;
	zp_sxy 			= obj.zp_sxy;
	zp_sxyz 		= obj.zp_sxyz;
    zero            = obj.zero;
}

Image::~Image() {
    
    if (data!=NULL)             free(data);
    if (nim!=NULL)              nifti_image_free(nim);
	if (xyz2ijk!=NULL)			delete[] xyz2ijk;
	if (voxels!=NULL)			delete[] voxels;
    if (zero!=NULL)			    free(zero);
    
}

void Image::destroyCopy(){
	filePath.erase();
	data 			= NULL;
	nim 			= NULL;
	xyz2ijk 		= NULL;
	voxels 			= NULL;
    zero            = NULL;
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

	voxelVolume = nim->pixdim[1]*nim->pixdim[2]*nim->pixdim[3];

	// TODO: Enable option to choose between sform or qform
	if (nim->sform_code>0) {
		// Then xyz2ijk=sform
	}
	else {
		// Then xyz2ijk=qform
	}

	this->readHeader_detail();

	return true;

}

void Image::readHeader_detail() {

	// Used for sub2ind
	sx 			= nim->nx;
	sxy 		= nim->nx*nim->ny;
	sxyz 		= nim->nx*nim->ny*nim->nz;

	zp_sx 		= (nim->nx+2);
	zp_sxy 		= (nim->nx+2)*(nim->ny+2);
	zp_sxyz 	= (nim->nx+2)*(nim->ny+2)*(nim->nz+2);

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

	float i = nim->sto_ijk.m[0][0]*x + nim->sto_ijk.m[0][1]*y + nim->sto_ijk.m[0][2]*z + nim->sto_ijk.m[0][3];
	if ( (i<-0.5) || (i>nim->nx-0.5)) return 0;

	float j = nim->sto_ijk.m[1][0]*x + nim->sto_ijk.m[1][1]*y + nim->sto_ijk.m[1][2]*z + nim->sto_ijk.m[1][3];
	if ( (j<-0.5) || (j>nim->ny-0.5)) return 0;

	float k = nim->sto_ijk.m[2][0]*x + nim->sto_ijk.m[2][1]*y + nim->sto_ijk.m[2][2]*z + nim->sto_ijk.m[2][3];
	if ( (k<-0.5) || (k>nim->nz-0.5)) return 0;

	return 1;
}

// Notice that the data has 1/voxelVolume fraction included to speed up interpolation
bool Image::indexVoxels() {

	// voxels are indexed on a zero padded image in order to avoid boundary checking
	voxels = new Voxel[zp_sxyz];
    zero   = (float*)calloc(nim->nt,sizeof(float));
    
	size_t index = 0;
	if (GENERAL::verboseLevel!=QUITE) std::cout << "Indexing voxels: 0%" << '\r' << std::flush;
    
	for (int z=0; z<(nim->nz+2); z++) {
		for (int y=0; y<(nim->ny+2); y++) {
			for (int x=0; x<(nim->nx+2); x++) {
                
                voxels[index].box[0]=((x<1)||(y<1)||(z<1)||(x>(nim->nx)  )||(y>(nim->ny)  )||(z>(nim->nz))  ) ? zero : data + ( size_t(x-1) + size_t(y-1)*sx + size_t(z-1)*sxy )*size_t(nim->nt);
                voxels[index].box[1]=((x<0)||(y<1)||(z<1)||(x>(nim->nx)-1)||(y>(nim->ny)  )||(z>(nim->nz))  ) ? zero : data + ( size_t(x)   + size_t(y-1)*sx + size_t(z-1)*sxy )*size_t(nim->nt);
                voxels[index].box[2]=((x<1)||(y<0)||(z<1)||(x>(nim->nx)  )||(y>(nim->ny)-1)||(z>(nim->nz))  ) ? zero : data + ( size_t(x-1) + size_t(y)  *sx + size_t(z-1)*sxy )*size_t(nim->nt);
                voxels[index].box[3]=((x<0)||(y<0)||(z<1)||(x>(nim->nx)-1)||(y>(nim->ny)-1)||(z>(nim->nz))  ) ? zero : data + ( size_t(x)   + size_t(y)  *sx + size_t(z-1)*sxy )*size_t(nim->nt);
                voxels[index].box[4]=((x<1)||(y<1)||(z<0)||(x>(nim->nx)  )||(y>(nim->ny)  )||(z>(nim->nz)-1)) ? zero : data + ( size_t(x-1) + size_t(y-1)*sx + size_t(z)  *sxy )*size_t(nim->nt);
                voxels[index].box[5]=((x<0)||(y<1)||(z<0)||(x>(nim->nx)-1)||(y>(nim->ny)  )||(z>(nim->nz)-1)) ? zero : data + ( size_t(x)   + size_t(y-1)*sx + size_t(z)  *sxy )*size_t(nim->nt);
                voxels[index].box[6]=((x<1)||(y<0)||(z<0)||(x>(nim->nx)  )||(y>(nim->ny)-1)||(z>(nim->nz)-1)) ? zero : data + ( size_t(x-1) + size_t(y)  *sx + size_t(z)  *sxy )*size_t(nim->nt);
                voxels[index].box[7]=((x<0)||(y<0)||(z<0)||(x>(nim->nx)-1)||(y>(nim->ny)-1)||(z>(nim->nz)-1)) ? zero : data + ( size_t(x)   + size_t(y)  *sx + size_t(z)  *sxy )*size_t(nim->nt);
                
				index++;
			}
		}
		if (GENERAL::verboseLevel!=QUITE) std::cout << "Indexing voxels: " << (size_t)((index/(float)(zp_sxyz-1))*100) << "%" << '\r' << std::flush;
	}

	if (GENERAL::verboseLevel!=QUITE) std::cout << "Indexing voxels: 100%" << '\r' << std::flush;
	if (GENERAL::verboseLevel!=QUITE) std::cout << std::endl;

	return true;
}


// Converts physical coordinates to index on the zero-padded image that is voxel indexed for interpolation
bool Image::prepInterp(float *p, int *cor_ijk, float *volFrac) {

	float ijk[3];
	float   a[3];
	float   b[3];

	for (int i=0; i<3; i++) {
		ijk[i] 		= nim->sto_ijk.m[i][0]*p[0] + nim->sto_ijk.m[i][1]*p[1] + nim->sto_ijk.m[i][2]*p[2] + nim->sto_ijk.m[i][3]+1; //+1 because the image is zero padded
		cor_ijk[i] 	= std::floor(ijk[i]);
		a[i] 		= (ijk[i]-cor_ijk[i])*nim->pixdim[i+1];
		b[i] 		= nim->pixdim[i+1]-a[i];
	}

	if ( (cor_ijk[0] < 0) || (cor_ijk[1] < 0) || (cor_ijk[2] < 0) || (cor_ijk[0] > nim->nx) || (cor_ijk[1] > nim->ny) || (cor_ijk[2] > nim->nz) )
		return false;

	// Volume fractions
	volFrac[7]  = a[0]*a[1]*a[2]; // front low left
	volFrac[6]  = b[0]*a[1]*a[2]; // front low right
	volFrac[5]  = a[0]*b[1]*a[2]; // front up  left
	volFrac[4]  = b[0]*b[1]*a[2]; // front up  right
	volFrac[3]  = a[0]*a[1]*b[2]; // back  low left
	volFrac[2]  = b[0]*a[1]*b[2]; // back  low right
	volFrac[1]  = a[0]*b[1]*b[2]; // back  up  left
	volFrac[0]  = b[0]*b[1]*b[2]; // back  up  right

	return true;

}

void Image::getVal(float *p, float* out) {

	int   cor_ijk[3];
	float volFrac[8];

	if ( prepInterp(p,cor_ijk,volFrac) == false ) {
		memset(out,0,nim->nt*sizeof(float));
		return;
	}
	
	float **vals = voxels[cor_ijk[0] + cor_ijk[1]*zp_sx + cor_ijk[2]*zp_sxy].box;

	for (int c=0; c<nim->nt; c++) {
		out[c] =volFrac[0]*vals[0][c] +
				volFrac[1]*vals[1][c] +
				volFrac[2]*vals[2][c] +
				volFrac[3]*vals[3][c] +
				volFrac[4]*vals[4][c] +
				volFrac[5]*vals[5][c] +
				volFrac[6]*vals[6][c] +
				volFrac[7]*vals[7][c];
	}
    
}


