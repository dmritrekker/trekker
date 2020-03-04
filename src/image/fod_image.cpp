#include "../image/fod_image.h"
#include "../math/sphericalHarmonics.h"

FOD_Image::FOD_Image() {
	iseven              = true;
    
    discretizationFlag  = true;
    isspheresliced      = false;
    sphereFileName      = "";
    
    discVolSphInds      = NULL;
    
}

void FOD_Image::cleanFODImage() {
    if (discVolSphInds!=NULL) {
         delete[] discVolSphInds;
         discVolSphInds = NULL;
    }
}

float FOD_Image::getSmallestPixdim() {
	float smallestPixelDim = 1e10;
	for (int i=1; i<4; i++) {
		if (nim->pixdim[i]<smallestPixelDim)
			smallestPixelDim = nim->pixdim[i];
	}
	return smallestPixelDim;
}

int FOD_Image::getSHorder() {

    if (isspheresliced==false) {
        int order = sqrt(nim->nt);
        if ((order*order)!=nim->nt) {
            if (GENERAL::verboseLevel>QUITE) std::cout << "(symmetric FOD with order " << (sqrt(8*nim->nt+1)-3)/2 << ") " << std::flush;
            return (sqrt(8*nim->nt+1)-3)/2;
        } else {
            if (GENERAL::verboseLevel>QUITE) std::cout << "(asymmetric FOD with order " << sqrt(nim->nt)-1 << ") " << std::flush;
            iseven = false;
            return order-1;
        }
    } else {
        if (iseven) {
            int order = 16;
            if (GENERAL::verboseLevel>QUITE) std::cout << "(using symmetric FOD with order " << order << ") " << std::flush;
            return order;
        }
        else {
            int order = 13;
            if (GENERAL::verboseLevel>QUITE) std::cout << "(using asymmetric FOD with order " << order << ") " << std::flush;
            return order;
        }
            
    }

}

void loadingTask(FOD_Image* FODImg, size_t begin_ind, size_t end_ind, NiftiDataAccessor *accessor) {
    
    float* FOD;
    if (FODImg->isspheresliced==false) {
        FOD = new float[FODImg->nim->nt];
    } else {
        FOD = new float[SH::numberOfSphericalHarmonicCoefficients];
    }
    
    for (size_t n=begin_ind; n<end_ind; n++) {
        
        size_t   ind = FODImg->nnzVoxelInds[n];
        size_t reInd = FODImg->nnzVoxelReInds[n];
        
        if (FODImg->isspheresliced==false) {
            for (int t=0; t<FODImg->nim->nt; t++)
                FOD[t] = accessor->get(FODImg->nim->data,ind+t*FODImg->sxyz);
        } else {
            for (int i=0; i<SH::numberOfSphericalHarmonicCoefficients; i++) {
                FOD[i] = 0;
                for (int t=0; t<FODImg->nim->nt; t++)
                    FOD[i] += SH::Ylm[t][i]*accessor->get(FODImg->nim->data,ind+t*FODImg->sxyz); // Ylm is already scaled by the surface area of a point
            }
        }
    
        if (FODImg->discretizationFlag==true) {
            for (size_t t=0; t<FODImg->discVolSphCoords.size(); t++) {
                float dir[3]            = {FODImg->discVolSphCoords[t].x,FODImg->discVolSphCoords[t].y,FODImg->discVolSphCoords[t].z};
                FODImg->data[reInd+t]   = SH::SH_amplitude(FOD,dir)/FODImg->voxelVolume;
            }
        } else {
            for (int t=0; t<SH::numberOfSphericalHarmonicCoefficients; t++) {
                FODImg->data[reInd+t]   = FOD[t]/FODImg->voxelVolume;
            }
        }
        
    }
    
    delete[] FOD;
    
    GENERAL::tracker_lock.lock();
    
    if ((GENERAL::verboseLevel!=QUITE) && (FODImg->discretizationFlag == true)) std::cout << "Discretizing FOD: " << (int)((end_ind/(float)(FODImg->nnzVoxelInds.size()-1))*100) << "%" << '\r' << std::flush;
    if ((GENERAL::verboseLevel!=QUITE) && (FODImg->discretizationFlag == false)) std::cout << "Loading FOD: " << (int)((end_ind/(float)(FODImg->nnzVoxelInds.size()-1))*100) << "%" << '\r' << std::flush;
    std::lock_guard<std::mutex> lk(GENERAL::exit_mx);
    GENERAL::exit_cv.notify_all();
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
	
	// Prep sphere parameters
	if (iseven)
        discVolSphDim       = 21; // Creates 1038 points on half-sphere (2076 points on full-sphere)
    else
        discVolSphDim       = 15; // Creates 1004 points on full-sphere, i.e. AFOD is less densely sampled
    
    discVolSphRadius    = (float(discVolSphDim)-1)/2.0 - 0.5;
    discVolSphShift     = discVolSphRadius + 0.5;
            
	
    // Load data
    std::unique_lock<std::mutex> lk(GENERAL::exit_mx);
    
    size_t nnt = SH::numberOfSphericalHarmonicCoefficients;
    if (discretizationFlag==true) {
        fillDiscVolSph();
        nnt = discVolSphCoords.size();
    }
    
    data = (float*) calloc(nim->nx * nim->ny * nim->nz * nnt, sizeof(float));
    
    if (data==NULL) {
        std::cout << "OUT OF MEMORY" << std::endl << std::flush;
        assert(0);
    }
    
    // Mark non-zero voxels
    size_t ind   = 0;
    for (int z=0; z<(nim->nz); z++) {
        for (int y=0; y<(nim->ny); y++) {
            for (int x=0; x<(nim->nx); x++) {
                
                ind = (x+y*sx+z*sxy);
                
                for (int t=0; t<(nim->nt); t++) {
                    
                    if (accessor->get(nim->data,ind+t*sxyz)!=0) {                            
                        nnzVoxelInds.push_back(ind);
                        nnzVoxelReInds.push_back(ind*nnt);
                        break;
                    }
                    
                }
                    
            }
            
        }
    }
    
    // Prepare data
    if ((GENERAL::verboseLevel!=QUITE) && (discretizationFlag == true) ) std::cout << "Discretizing FOD: 0%" << '\r' << std::flush;
    if ((GENERAL::verboseLevel!=QUITE) && (discretizationFlag == false)) std::cout << "Loading FOD: 0%" << '\r' << std::flush;
    
    size_t chunkSize = 128;
    size_t bind      = 0;
    size_t eind      = 0;
    size_t taskCount = 0;
    
    for (int i=0; i<GENERAL::numberOfThreads; i++) {
        if (eind < nnzVoxelInds.size()) {
            bind = eind;
            eind = bind + chunkSize;
            if (eind>nnzVoxelInds.size()) eind=nnzVoxelInds.size();
        } else {
            break;
        }
        std::thread task = std::thread(loadingTask, this, bind, eind, accessor);
        task.detach();
        taskCount++;
    }
    
    while(eind < nnzVoxelInds.size()) {
        
        GENERAL::exit_cv.wait(lk);
        
        bind = eind;
        eind = bind + chunkSize;
        if (eind>nnzVoxelInds.size()) eind=nnzVoxelInds.size();
        
        std::thread task = std::thread(loadingTask, this, bind, eind, accessor);
        task.detach();
        GENERAL::tracker_lock.unlock();
    }
    
    while (taskCount) {
        GENERAL::exit_cv.wait(lk);
        taskCount--;
        GENERAL::tracker_lock.unlock();
    }
    
    if ((GENERAL::verboseLevel!=QUITE) && (discretizationFlag == true) ) std::cout << "Discretizing FOD: 100%" << '\r' << std::flush;
    if ((GENERAL::verboseLevel!=QUITE) && (discretizationFlag == false)) std::cout << "Loading FOD: 100%" << '\r' << std::flush;
    if (GENERAL::verboseLevel!=QUITE) std::cout << std::endl;
    
    nim->nt     = nnt;
    nim->nvox   = nim->nx * nim->ny * nim->nz * nim->nt;
    
    nnzVoxelInds.clear();
    nnzVoxelReInds.clear();
    discVolSphCoords.clear();
    
    nifti_image_unload(nim);
    delete accessor;
    
	this->Image::indexVoxels();
    
	return true;
}


void FOD_Image::fillDiscVolSph() {

    float R = (float(discVolSphDim)-1)/2.0;
    float zs;
    

    if (iseven) {
        discVolSphInds      = new int[discVolSphDim*discVolSphDim*((discVolSphDim/2)+1)];
        zs                  = 0;
    } else {
        discVolSphInds      = new int[discVolSphDim*discVolSphDim*discVolSphDim];
        zs                  = -R;
    }
        
    int ind = 0;
    for (float x=-R; x<=R; x++)
        for (float y=-R; y<=R; y++)
            for (float z=zs; z<=R; z++) {
                float dist = std::sqrt(x*x+y*y+z*z);
                if (std::abs(dist-discVolSphRadius)<(std::sqrt(3)/2)) {
                    discVolSphInds[size_t((x+R)+((y+R)+(z-zs)*discVolSphDim)*discVolSphDim)] = ind++;                    
                    Coordinate vertex(x,y,z);
                    vertex.normalize();
                    discVolSphCoords.push_back(vertex);
                }
                else
                    discVolSphInds[size_t((x+R)+((y+R)+(z-zs)*discVolSphDim)*discVolSphDim)] = -1;
                
            }

}


void FOD_Image::orderDirections(float* unit_dir) {
    
    float X = unit_dir[0];
    float Y = unit_dir[1];
    float Z = unit_dir[2];
    
    switch (TRACKER::orderOfDirections)
    {  
        case XYZ : {unit_dir[0]= X;  unit_dir[1]= Y;  unit_dir[2]= Z; } break;
        case XYz : {unit_dir[0]= X;  unit_dir[1]= Y;  unit_dir[2]=-Z; } break;
        case XyZ : {unit_dir[0]= X;  unit_dir[1]=-Y;  unit_dir[2]= Z; } break;
        case Xyz : {unit_dir[0]= X;  unit_dir[1]=-Y;  unit_dir[2]=-Z; } break;
        case xYZ : {unit_dir[0]=-X;  unit_dir[1]= Y;  unit_dir[2]= Z; } break;
        case xYz : {unit_dir[0]=-X;  unit_dir[1]= Y;  unit_dir[2]=-Z; } break;
        case xyZ : {unit_dir[0]=-X;  unit_dir[1]=-Y;  unit_dir[2]= Z; } break;
        case xyz : {unit_dir[0]=-X;  unit_dir[1]=-Y;  unit_dir[2]=-Z; } break;
        
        case XZY : {unit_dir[0]= X;  unit_dir[1]= Z;  unit_dir[2]= Y; } break; 
        case XZy : {unit_dir[0]= X;  unit_dir[1]= Z;  unit_dir[2]=-Y; } break;
        case XzY : {unit_dir[0]= X;  unit_dir[1]=-Z;  unit_dir[2]= Y; } break; 
        case Xzy : {unit_dir[0]= X;  unit_dir[1]=-Z;  unit_dir[2]=-Y; } break;
        case xZY : {unit_dir[0]=-X;  unit_dir[1]= Z;  unit_dir[2]= Y; } break; 
        case xZy : {unit_dir[0]=-X;  unit_dir[1]= Z;  unit_dir[2]=-Y; } break;
        case xzY : {unit_dir[0]=-X;  unit_dir[1]=-Z;  unit_dir[2]= Y; } break; 
        case xzy : {unit_dir[0]=-X;  unit_dir[1]=-Z;  unit_dir[2]=-Y; } break;
        
        case YXZ : {unit_dir[0]= Y;  unit_dir[1]= X;  unit_dir[2]= Z; } break;
        case YXz : {unit_dir[0]= Y;  unit_dir[1]= X;  unit_dir[2]=-Z; } break;
        case YxZ : {unit_dir[0]= Y;  unit_dir[1]=-X;  unit_dir[2]= Z; } break;
        case Yxz : {unit_dir[0]= Y;  unit_dir[1]=-X;  unit_dir[2]=-Z; } break;
        case yXZ : {unit_dir[0]=-Y;  unit_dir[1]= X;  unit_dir[2]= Z; } break;
        case yXz : {unit_dir[0]=-Y;  unit_dir[1]= X;  unit_dir[2]=-Z; } break;
        case yxZ : {unit_dir[0]=-Y;  unit_dir[1]=-X;  unit_dir[2]= Z; } break;
        case yxz : {unit_dir[0]=-Y;  unit_dir[1]=-X;  unit_dir[2]=-Z; } break;

        case YZX : {unit_dir[0]= Y;  unit_dir[1]= Z;  unit_dir[2]= X; } break;
        case YZx : {unit_dir[0]= Y;  unit_dir[1]= Z;  unit_dir[2]=-X; } break;
        case YzX : {unit_dir[0]= Y;  unit_dir[1]=-Z;  unit_dir[2]= X; } break;
        case Yzx : {unit_dir[0]= Y;  unit_dir[1]=-Z;  unit_dir[2]=-X; } break;
        case yZX : {unit_dir[0]=-Y;  unit_dir[1]= Z;  unit_dir[2]= X; } break;
        case yZx : {unit_dir[0]=-Y;  unit_dir[1]= Z;  unit_dir[2]=-X; } break;
        case yzX : {unit_dir[0]=-Y;  unit_dir[1]=-Z;  unit_dir[2]= X; } break;
        case yzx : {unit_dir[0]=-Y;  unit_dir[1]=-Z;  unit_dir[2]=-X; } break;
        
        case ZYX : {unit_dir[0]= Z;  unit_dir[1]= Y;  unit_dir[2]= X; } break;
        case ZYx : {unit_dir[0]= Z;  unit_dir[1]= Y;  unit_dir[2]=-X; } break;
        case ZyX : {unit_dir[0]= Z;  unit_dir[1]=-Y;  unit_dir[2]= X; } break;
        case Zyx : {unit_dir[0]= Z;  unit_dir[1]=-Y;  unit_dir[2]=-X; } break;
        case zYX : {unit_dir[0]=-Z;  unit_dir[1]= Y;  unit_dir[2]= X; } break;
        case zYx : {unit_dir[0]=-Z;  unit_dir[1]= Y;  unit_dir[2]=-X; } break;
        case zyX : {unit_dir[0]=-Z;  unit_dir[1]=-Y;  unit_dir[2]= X; } break;
        case zyx : {unit_dir[0]=-Z;  unit_dir[1]=-Y;  unit_dir[2]=-X; } break;
        
        case ZXY : {unit_dir[0]= Z;  unit_dir[1]= X;  unit_dir[2]= Y; } break;
        case ZXy : {unit_dir[0]= Z;  unit_dir[1]= X;  unit_dir[2]=-Y; } break;
        case ZxY : {unit_dir[0]= Z;  unit_dir[1]=-X;  unit_dir[2]= Y; } break;
        case Zxy : {unit_dir[0]= Z;  unit_dir[1]=-X;  unit_dir[2]=-Y; } break;
        case zXY : {unit_dir[0]=-Z;  unit_dir[1]= X;  unit_dir[2]= Y; } break;
        case zXy : {unit_dir[0]=-Z;  unit_dir[1]= X;  unit_dir[2]=-Y; } break;
        case zxY : {unit_dir[0]=-Z;  unit_dir[1]=-X;  unit_dir[2]= Y; } break;
        case zxy : {unit_dir[0]=-Z;  unit_dir[1]=-X;  unit_dir[2]=-Y; } break;
        
        default: { } break;
    }
        
}


int FOD_Image::vertexCoord2volInd(float* vertexCoord) {
    
    int x,y,z;
    
    if (iseven) { 
        if (vertexCoord[2]<0) {
            x = std::round(-vertexCoord[0]*discVolSphRadius) + discVolSphShift;
            y = std::round(-vertexCoord[1]*discVolSphRadius) + discVolSphShift;
            z = std::round(-vertexCoord[2]*discVolSphRadius);
        } else {
            x = std::round( vertexCoord[0]*discVolSphRadius) + discVolSphShift;
            y = std::round( vertexCoord[1]*discVolSphRadius) + discVolSphShift;
            z = std::round( vertexCoord[2]*discVolSphRadius);
        }
    } else {
        x = std::round(vertexCoord[0]*discVolSphRadius) + discVolSphShift;
        y = std::round(vertexCoord[1]*discVolSphRadius) + discVolSphShift;
        z = std::round(vertexCoord[2]*discVolSphRadius) + discVolSphShift;
    }
    
    int volInd = discVolSphInds[x+(y+z*discVolSphDim)*discVolSphDim];
    
    return volInd;
}

float FOD_Image::getFODval(float *p, float* vertexCoord) {
    
    int   cor_ijk[3];
	float volFrac[8];
    
	if ( prepInterp(p,cor_ijk,volFrac) == false )
		return 0;

    int volInd   = vertexCoord2volInd(vertexCoord);
 
	float **vals = voxels[cor_ijk[0] + cor_ijk[1]*zp_sx + cor_ijk[2]*zp_sxy].box;    
    
    return  volFrac[0]*vals[0][volInd] +
			volFrac[1]*vals[1][volInd] +
			volFrac[2]*vals[2][volInd] +
			volFrac[3]*vals[3][volInd] +
			volFrac[4]*vals[4][volInd] +
			volFrac[5]*vals[5][volInd] +
			volFrac[6]*vals[6][volInd] +
			volFrac[7]*vals[7][volInd];
    
}
