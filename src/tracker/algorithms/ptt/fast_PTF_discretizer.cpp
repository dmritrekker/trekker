#include "fast_PTF_discretizer.h"

#include <string.h>
#include <cmath>
#include "../../../math/math_aux.h"

namespace PTF_CONSTS {

size_t  k_resolution;
float   k_step;

float** curve_consts;
float** probe_step_consts;

float   probe_cum_length;
float 	probeNormalizer;
float   angularSeparation;
size_t  numelk;
size_t  numelkq;

std::vector<size_t> valid_indices;
size_t  validIndexCount;

bool    isCleaned;
bool    isReady;

void precomputePTFCoefficients(size_t _k_resolution) {

	if (GENERAL::verboseLevel!=QUITE) std::cout << "Precomputing parallel transport propagator... " << std::flush;

	isCleaned 			= false;
	k_resolution        = _k_resolution;
    k_step              = 2*TRACKER::maxCurvature/float(_k_resolution-1);
    
	int   n             = _k_resolution/2;
	float probe_step    =   TRACKER::probeLength/(float)TRACKER::probeQuality;

	size_t dim          = k_resolution*k_resolution;

	curve_consts 		= new float*[dim];
	for(size_t i=0; i<dim; ++i)
		curve_consts[i] = new float[9];
    
    probe_step_consts 		= new float*[dim];
	for(size_t i=0; i<dim; ++i)
		probe_step_consts[i] = new float[9];

	probe_cum_length = 0;
	for (int i=0; i<TRACKER::probeQuality; i++)
		probe_cum_length += (float)(i+1);

	probeNormalizer = 1/float(TRACKER::probeQuality*TRACKER::probeCount);
    angularSeparation = M_2_PI/float(TRACKER::probeCount);
    
	numelk 	= k_resolution*k_resolution;
	numelkq = numelk*TRACKER::probeQuality;

	for (int i=-n; i<=n; i++) {
		for (int j=-n; j<=n; j++) {

			size_t ind   = (i+n)*k_resolution+(j+n);

			float _k1    = (float)i*k_step;
			float _k2    = (float)j*k_step;
			float _kk    = _k1*_k1 + _k2*_k2;

			if (_kk<=(TRACKER::maxCurvature*TRACKER::maxCurvature)) {
                
				float k         = sqrt(_kk);

				float sinsk   	= std::sin(TRACKER::stepSize*k);
				float cossk   	= std::cos(TRACKER::stepSize*k);
                
                float pssinsk   = std::sin(probe_step*k);
				float pscossk   = std::cos(probe_step*k);

				if (k<1e-4) {

					curve_consts[ind][0] = TRACKER::stepSize;
					curve_consts[ind][1] = 0;
					curve_consts[ind][2] = 0;
					curve_consts[ind][3] = 1;
					curve_consts[ind][4] = 0;
					curve_consts[ind][5] = 0;
					curve_consts[ind][6] = 0;
					curve_consts[ind][7] = 0;
					curve_consts[ind][8] = 1;
                    
                    probe_step_consts[ind][0] = probe_step;
					probe_step_consts[ind][1] = 0;
					probe_step_consts[ind][2] = 0;
					probe_step_consts[ind][3] = 1;
					probe_step_consts[ind][4] = 0;
					probe_step_consts[ind][5] = 0;
					probe_step_consts[ind][6] = 0;
					probe_step_consts[ind][7] = 0;
					probe_step_consts[ind][8] = 1;

				}
				else {

					curve_consts[ind][0] = sinsk/k;
					curve_consts[ind][1] = (1-cossk)/_kk*_k1;
					curve_consts[ind][2] = (1-cossk)/_kk*_k2;

					curve_consts[ind][3] = cossk;
					curve_consts[ind][4] = curve_consts[ind][0]*_k1;
					curve_consts[ind][5] = curve_consts[ind][0]*_k2;

					curve_consts[ind][6] = -curve_consts[ind][5];
					curve_consts[ind][7] = -_k2*curve_consts[ind][1];
					curve_consts[ind][8] = 1/_kk*(_k1*_k1+_k2*_k2*cossk);
                    
                    
                    
                    probe_step_consts[ind][0] = pssinsk/k;
					probe_step_consts[ind][1] = (1-pscossk)/_kk*_k1;
					probe_step_consts[ind][2] = (1-pscossk)/_kk*_k2;

					probe_step_consts[ind][3] = pscossk;
					probe_step_consts[ind][4] = probe_step_consts[ind][0]*_k1;
					probe_step_consts[ind][5] = probe_step_consts[ind][0]*_k2;

					probe_step_consts[ind][6] = -probe_step_consts[ind][5];
					probe_step_consts[ind][7] = -_k2*probe_step_consts[ind][1];
					probe_step_consts[ind][8] = 1/_kk*(_k1*_k1+_k2*_k2*pscossk);

				}
				
				valid_indices.push_back(ind);

			}

		}
	}

	validIndexCount = valid_indices.size();

	if (GENERAL::verboseLevel!=QUITE) std::cout << "Done" << std::endl;

}

void cleanPTFCoefficients() {

	if (isCleaned == false) {

		isCleaned = true;

		size_t dim = k_resolution*k_resolution;
		for(size_t i=0; i<dim; ++i)
			delete[] curve_consts[i];
		delete[] curve_consts;
        
        for(size_t i=0; i<dim; ++i)
			delete[] probe_step_consts[i];
		delete[] probe_step_consts;
        
		valid_indices.clear();
	}
	
}

}
