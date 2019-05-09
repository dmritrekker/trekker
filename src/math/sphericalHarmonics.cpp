#include "sphericalHarmonics.h"

namespace SH {

int 	sphericalHarmonicOrder             		= 0;
int 	numberOfSphericalHarmonicCoefficients  	= 0;

size_t  numberOfSamples_phi 					= 0;
size_t  numberOfSamples_theta 					= 0;

float   scalingFactor_phi 						= 0;
float   scalingFactor_theta 					= 0;

float  *precomputedPhiComponent 				= NULL;
float  *precomputedThetaComponent            	= NULL;

int sphPlmInd(int l,int m) {
	return (l*(l+1))/2+m;
}

void computeLegendrePolynomials(double *plm, double x, int order) {

	plm[0] = 1.0/std::sqrt(4.0*PI);

	for(double m = 1; m <= order; m++) {
		plm[sphPlmInd(m,m)] = -std::sqrt((2*m+1)*(1-x*x)/(2*m))*plm[sphPlmInd(m-1,m-1)];
	}

	for(double m=0; m < order; m++) {
		plm[sphPlmInd(m+1,m)] = std::sqrt(2*m+3)*x*plm[sphPlmInd(m,m)];
	}

	for(double m =  0; m <= order; m++)
		for(double l = m+2; l <= order; l++) {
			plm[sphPlmInd(l,m)] = std::sqrt(((2.0*l+1)*(2.0*l-1)) / ((l+m)*(l-m)))*x*plm[sphPlmInd(l-1,m)]-std::sqrt( (2.0*l+1)*(l-m-1.0)*(l+m-1.0) / ((2.0*l-3)*(l-m)*(l+m)))*plm[sphPlmInd(l-2,m)];
		}

}

void clean() {
	delete[] precomputedPhiComponent;
	delete[] precomputedThetaComponent;
}



void precompute(size_t num) {

	if (GENERAL::verboseLevel!=QUITE) std::cout << "Precomputing spherical harmonic multipliers... " << std::flush;

	numberOfSamples_phi 					=   num;
	numberOfSamples_theta 					= 8*num;

	sphericalHarmonicOrder   				= TRACKER::img_FOD->getSHorder();
	numberOfSphericalHarmonicCoefficients 	= TRACKER::img_FOD->getNim()->nt;

	double delta_phi 	 					= 2/(double)(numberOfSamples_phi-1);
	double delta_theta 						= 2/(double)(numberOfSamples_theta -1);

	scalingFactor_phi 						= 1/delta_phi;
	scalingFactor_theta 					= 1/delta_theta;

	precomputedPhiComponent 				= new float[numberOfSphericalHarmonicCoefficients*numberOfSamples_phi*numberOfSamples_phi];
	precomputedThetaComponent         		= new float[numberOfSphericalHarmonicCoefficients*numberOfSamples_theta];

	size_t c = 0;
	for (size_t i=0; i<numberOfSamples_phi; i++) {

		double x 		= (double)i*delta_phi-1;

		for (size_t j=0; j<numberOfSamples_phi; j++) {

			double y 		= (double)j*delta_phi-1;
			double phi 		= std::atan2(y,x);

			precomputedPhiComponent[c++] = 1;
			for(double l = 2; l <= sphericalHarmonicOrder; l+=2) {
				for(double m = -l; m <= l; m++) {
					double ang = (fabs((double)m))*phi;
					if (m<0)  		precomputedPhiComponent[c++] = std::sin(ang);
					else if (m==0)  precomputedPhiComponent[c++] = 1;
					else 			precomputedPhiComponent[c++] = std::cos(ang);
				}
			}

		}
	}

	double *plm  = new double[(sphericalHarmonicOrder*(sphericalHarmonicOrder+1))/2+sphericalHarmonicOrder+1];

	c = 0;
	for (size_t k=0; k<numberOfSamples_theta; k++) {

		double theta = (double)k*delta_theta-1;
		computeLegendrePolynomials(plm, theta, sphericalHarmonicOrder);

		precomputedThetaComponent[c++] = plm[sphPlmInd(0,0)];
		for(float l = 2; l <= sphericalHarmonicOrder; l+=2) {
			for(float m = -l; m <= l; m++) {
				if (m<0)  		precomputedThetaComponent[c++] = SQRT2*plm[sphPlmInd(l,-m)];
				else if (m==0) 	precomputedThetaComponent[c++] =       plm[sphPlmInd(l, 0)];
				else  			precomputedThetaComponent[c++] = SQRT2*plm[sphPlmInd(l, m)];
			}
		}


	}

	delete[] plm;

	if (GENERAL::verboseLevel!=QUITE) std::cout << "Done" << std::endl;

}


size_t getPhiIndex(float* unit_dir) {
	return  numberOfSphericalHarmonicCoefficients*((size_t)((unit_dir[0]+1)*scalingFactor_phi)*numberOfSamples_phi + (size_t)((unit_dir[1]+1)*scalingFactor_phi));
}


size_t getThetaIndex(float* unit_dir) {
	return  (int)((unit_dir[2]+1)*scalingFactor_theta)*numberOfSphericalHarmonicCoefficients;
}


float SH_amplitude(float *values, float *unit_dir) {

	verifyUnitRange(unit_dir);
	float *phiComp 		= precomputedPhiComponent   +   getPhiIndex(unit_dir);
	float *thetaComp  	= precomputedThetaComponent + getThetaIndex(unit_dir);

	float amp = 0;
	for (int i=0; i<numberOfSphericalHarmonicCoefficients; i++)
		amp += values[i]*phiComp[i]*thetaComp[i];

	if (amp>0) 	return amp;
	else 		return 0;

}

}
