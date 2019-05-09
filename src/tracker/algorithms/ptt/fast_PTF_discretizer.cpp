#include "fast_PTF_discretizer.h"

#include <string.h>
#include <cmath>
#include "../../../math/math_aux.h"

namespace PTF_CONSTS {

size_t  k_resolution;

float*  k1;
float*  k2;
float*  k;
bool*   isvalid;

float** curve_consts;
float** probe_consts;

float   probe_cum_length;
float 	probeNormalizer;
size_t  numelk;
size_t  numelkq;

std::vector<size_t> valid_indices;
size_t  validIndexCount;

bool    isCleaned;

void precomputePTFCoefficients(size_t _k_resolution) {

	if (GENERAL::verboseLevel!=QUITE) std::cout << "Precomputing parallel transport propagator... " << std::flush;

	isCleaned 			= false;
	k_resolution        = _k_resolution;
	int   n             = _k_resolution/2;
	float k_step        = 2*TRACKER::maxCurvature/float(_k_resolution-1);
	float probe_step    =   TRACKER::probeLength/(float)TRACKER::probeQuality;

	size_t dim          = k_resolution*k_resolution;
	k1                  = new float[dim]; memset(k1,0,dim*sizeof(float));
	k2                  = new float[dim]; memset(k2,0,dim*sizeof(float));
	k                   = new float[dim]; memset(k ,0,dim*sizeof(float));
	isvalid             = new  bool[dim]; memset(isvalid,false,dim*sizeof(bool));

	curve_consts 		= new float*[dim];
	for(size_t i=0; i<dim; ++i)
		curve_consts[i] = new float[9];

	probe_consts 		= new float*[dim*TRACKER::probeQuality*TRACKER::probeCount];
	for(size_t i=0; i<dim*TRACKER::probeQuality*TRACKER::probeCount; ++i)
		probe_consts[i] = new float[6];

	float* probe_N1_sft  	= new float[TRACKER::probeCount];
	float* probe_N2_sft  	= new float[TRACKER::probeCount];
	float* probe_k1     	= new float[TRACKER::probeCount];
	float* probe_k2     	= new float[TRACKER::probeCount];
	float* probe_k      	= new float[TRACKER::probeCount];
	float* probe_kk     	= new float[TRACKER::probeCount];

	if (TRACKER::probeCount>1) {
		float angularSeparation	= 2*PI/TRACKER::probeCount;
		for (int i=0; i<TRACKER::probeCount; i++) {
			probe_N1_sft[i] = TRACKER::probeRadius*std::cos(float(i)*angularSeparation);
			probe_N2_sft[i] = TRACKER::probeRadius*std::sin(float(i)*angularSeparation);
		}
	} else {
		probe_N1_sft[0] = 0;
		probe_N2_sft[0] = 0;
	}

	probe_cum_length = 0;
	for (int i=0; i<TRACKER::probeQuality; i++)
		probe_cum_length += (float)(i+1);

	probeNormalizer = 1/float(TRACKER::probeQuality*TRACKER::probeCount);

	numelk 	= k_resolution*k_resolution;
	numelkq = numelk*TRACKER::probeQuality;

	for (int i=-n; i<=n; i++) {
		for (int j=-n; j<=n; j++) {

			size_t ind   = (i+n)*k_resolution+(j+n);

			float _k1    = (float)i*k_step;
			float _k2    = (float)j*k_step;
			float _kk    = _k1*_k1 + _k2*_k2;

			if (_kk<=(TRACKER::maxCurvature*TRACKER::maxCurvature)) {
				k1[ind]         = _k1;
				k2[ind]         = _k2;
				k[ind]          = sqrt(_kk);

				float sinsk   	= std::sin(TRACKER::stepSize*k[ind]);
				float cossk   	= std::cos(TRACKER::stepSize*k[ind]);

				if (k[ind]<1e-4) {

					curve_consts[ind][0] = TRACKER::stepSize;
					curve_consts[ind][1] = 0;
					curve_consts[ind][2] = 0;
					curve_consts[ind][3] = 1;
					curve_consts[ind][4] = 0;
					curve_consts[ind][5] = 0;
					curve_consts[ind][6] = 0;
					curve_consts[ind][7] = 0;
					curve_consts[ind][8] = 1;

					for (int q=0; q<TRACKER::probeQuality; q++) {
						for (int c=0; c<TRACKER::probeCount; c++) {
							size_t ind_pa          		= c*k_resolution*k_resolution*TRACKER::probeQuality+q*k_resolution*k_resolution+ind;

							probe_consts[ind_pa][0] 	= 1;
							probe_consts[ind_pa][1] 	= 0;
							probe_consts[ind_pa][2] 	= 0;
							probe_consts[ind_pa][3] 	= (float)(q+1)*probe_step;
							probe_consts[ind_pa][4] 	= probe_N1_sft[c];
							probe_consts[ind_pa][5] 	= probe_N2_sft[c];
						}
					}

				}
				else {

					curve_consts[ind][0] = sinsk/k[ind];
					curve_consts[ind][1] = (1-cossk)/_kk*_k1;
					curve_consts[ind][2] = (1-cossk)/_kk*_k2;

					curve_consts[ind][3] = cossk;
					curve_consts[ind][4] = curve_consts[ind][0]*_k1;
					curve_consts[ind][5] = curve_consts[ind][0]*_k2;

					curve_consts[ind][6] = -curve_consts[ind][5];
					curve_consts[ind][7] = -_k2*curve_consts[ind][1];
					curve_consts[ind][8] = 1/_kk*(_k1*_k1+_k2*_k2*cossk);

					for (int c=0; c<TRACKER::probeCount; c++) {
						float tmp    = 1-probe_N1_sft[c]*_k1-probe_N2_sft[c]*_k2;
						if (std::fabs(tmp) < 1e-4 ) {
							if (tmp < 0) tmp = -1e-4;
							else		 tmp =  1e-4;
						}
						probe_k1[c]  = _k1/tmp;
						probe_k2[c]  = _k2/tmp;
						probe_kk[c]  = probe_k1[c]*probe_k1[c]+probe_k2[c]*probe_k2[c];
						probe_k[c]   = sqrt(probe_kk[c]);
					}

					for (int q=0; q<TRACKER::probeQuality; q++) {
						for (int c=0; c<TRACKER::probeCount; c++) {
							size_t ind_pa           	= c*k_resolution*k_resolution*TRACKER::probeQuality+q*k_resolution*k_resolution+ind;

							float skp               	= (float)(q+1)*probe_step*k[ind];
							float sinskp            	= std::sin(skp);
							float cosskp            	= std::cos(skp);

							probe_consts[ind_pa][0] 	= std::cos((float)(q+1)*probe_step*k[ind]);
							probe_consts[ind_pa][1] 	= std::sin((float)(q+1)*probe_step*k[ind])/k[ind]*_k1;
							probe_consts[ind_pa][2] 	= std::sin((float)(q+1)*probe_step*k[ind])/k[ind]*_k2;
							probe_consts[ind_pa][3] 	= sinskp/probe_k[c];
							probe_consts[ind_pa][4] 	= (1-cosskp)/probe_kk[c]*probe_k1[c] + probe_N1_sft[c];
							probe_consts[ind_pa][5] 	= (1-cosskp)/probe_kk[c]*probe_k2[c] + probe_N2_sft[c];


						}
					}

				}

				isvalid[ind] = true;
				valid_indices.push_back(ind);

			}

		}
	}

	validIndexCount = valid_indices.size();

	delete[] probe_N1_sft;
	delete[] probe_N2_sft;
	delete[] probe_k1;
	delete[] probe_k2;
	delete[] probe_k;
	delete[] probe_kk;

	if (GENERAL::verboseLevel!=QUITE) std::cout << "Done" << std::endl;

}

void cleanPTFCoefficients() {

	if (isCleaned == false) {

		isCleaned = true;

		delete[] k1;
		delete[] k2;
		delete[] k;
		delete[] isvalid;


		size_t dim = k_resolution*k_resolution;
		for(size_t i=0; i<dim; ++i)
			delete[] curve_consts[i];
		delete[] curve_consts;

		for(size_t i=0; i<dim*TRACKER::probeQuality*TRACKER::probeCount; ++i)
			delete[] probe_consts[i];
		delete[] probe_consts;

		valid_indices.clear();
	}
}

}
