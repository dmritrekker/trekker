#ifndef SRC_PTF_WITH_PARAMETER_PRIORS_H_
#define SRC_PTF_WITH_PARAMETER_PRIORS_H_

#include "../../../config/config_tracker.h"
#include "../../../math/math_aux.h"
#include "../../../math/doRandomThings.h"
#include "../../../math/sphericalHarmonics.h"

#define SMALL 0.0001
#define BIG   10000

class PTF_with_parameter_priors {

public:

	PTF_with_parameter_priors(RandomDoer *_rndmr);
	~PTF_with_parameter_priors();
	void  init_Frame();
	void  swap(PTF_with_parameter_priors *ptf);

	void  setPosition(Coordinate in) {p[0] = in.x;p[1] = in.y;p[2] = in.z;};
    void  initkT(PTF_with_parameter_priors *ptf);    
    
    void  prepInitProbePropagator();
    void  prepProbePropagator();
    
    void  prepCandStepPropagator();
    
    void  walk();

    void  getCurrentCurve(float* inp, float **inF);
	void  getARandomFrame();
	void  getARandomFrame(Coordinate _seed_init_direction);
	void  flip();
	void  print();
    
    float getk1() {return k1_cand;};
    float getk2() {return k2_cand;};
    float getk()  {return std::sqrt(k1_cand*k1_cand+k2_cand*k2_cand);};

    float  *p;			// Position
    float **F;          // TN1N2 frame
    
    float  *PP;         // Propagator for probe
    
	float 	likelihood;	// Likelihood
	float   lastVal;
    float   lastVal_cand;
    
    float   initFirstVal;
    float   initFirstVal_cand;
	
	bool    initialized;
    
    int     probeStepCount;
    
    void    getInitCandidate();
    void    getCandidate();
    void    calcDataSupport();
    
    // Parameters
    void    updateTrackingParameters();
    
    float   param_maxCurvature;
    float   param_dataSupportExponent;
    

private:
	
	float  k1;
    float  k2;
    float  kT1;
    float  kT2;
    
    float  k1_cand;
    float  k2_cand;
    float  kT1_cand;
    float  kT2_cand;
    
    float  probe_k1;
    float  probe_k2;
    
    float  *sPP;        // Propagator for step
    
    
	RandomDoer *rndmr;
    
    
    FOD_Image       *ptf_FOD;
    SCALAR_Image    *ptf_dispersion;
    float  getFODamp(float *pv, float *Tv);
    float  *FOD;
    
    
    float _p[3];
    float _F[3][3];
    float _T[3];
    float _N1[3];
    float _N2[3];

};


inline float PTF_with_parameter_priors::getFODamp(float *pv, float *Tv) {
    
    if (TRACKER::fodDiscretization==FODDISC_OFF) {
        ptf_FOD->getVal(pv,FOD);
        return SH::SH_amplitude(FOD,Tv);
    } else
        return ptf_FOD->getFODval(pv,Tv);
    
}


inline void PTF_with_parameter_priors::getCurrentCurve(float* inp, float **inF) {
    
    for (int i=0; i<3; i++) {
        inp[i] 	= p[i];
        for (int j=0; j<3; j++) {
            inF[i][j] = F[i][j];
        }
        
	}
    
}


inline void PTF_with_parameter_priors::prepProbePropagator() {
    
    float t     = TRACKER::probeStepSize;
    
    if ( (std::fabs(k1_cand)<SMALL) && (std::fabs(k2_cand)<SMALL) ) {
        
        PP[0] = t;
        PP[1] = 0;
        PP[2] = 0;
        PP[3] = 1;
        PP[4] = 0;
        PP[5] = 0;
        PP[6] = 0;
        PP[7] = 0;
        PP[8] = 1;
        
    } else {
        
        if (std::fabs(k1_cand)<SMALL) k1_cand = SMALL;
        if (std::fabs(k2_cand)<SMALL) k2_cand = SMALL;
        
        float tto2  = t*t/float(2);
        
        PP[0] = t;
        PP[1] = k1_cand*tto2;
        PP[2] = k2_cand*tto2;
        PP[3] = 1-k2_cand*k2_cand*tto2-k1_cand*k1_cand*tto2;
        PP[4] = k1_cand*t;
        PP[5] = k2_cand*t;
        PP[6] = -k2_cand*t;
        PP[7] = -k1_cand*k2_cand*tto2;
        PP[8] = 1-k2_cand*k2_cand*tto2;
        
        /*
        float k     = std::sqrt(k1_cand*k1_cand+k2_cand*k2_cand);
        float kk    = k*k;
        float sinkt = std::sin(k*t);
        float coskt = std::cos(k*t);
        
        PP[0] = sinkt/k;
        PP[1] = k1_cand*(1-coskt)/kk;
        PP[2] = k2_cand*(1-coskt)/kk;
        PP[3] = coskt;
        PP[4] = k1_cand*sinkt/k;
        PP[5] = k2_cand*sinkt/k;
        PP[6] = -PP[5];
        PP[7] = k1_cand*k2_cand*(coskt-1)/kk;
        PP[8] = k1_cand*k1_cand+k2_cand*k2_cand*coskt/kk;
        */
    }
    
}


inline void PTF_with_parameter_priors::prepCandStepPropagator() {
    
    float t     = TRACKER::stepSize;
    
    if ( (std::fabs(k1_cand)<SMALL) && (std::fabs(k2_cand)<SMALL) ) {
        
        sPP[0] = t;
        sPP[1] = 0;
        sPP[2] = 0;
        sPP[3] = 1;
        sPP[4] = 0;
        sPP[5] = 0;
        sPP[6] = 0;
        sPP[7] = 0;
        sPP[8] = 1;
        
    } else {
        
        if (std::fabs(k1_cand)<SMALL) k1_cand = SMALL;
        if (std::fabs(k2_cand)<SMALL) k2_cand = SMALL;
        
        
        float tto2  = t*t/float(2);
        
        sPP[0] = t;
        sPP[1] = k1_cand*tto2;
        sPP[2] = k2_cand*tto2;
        sPP[3] = 1-k2_cand*k2_cand*tto2-k1_cand*k1_cand*tto2;
        sPP[4] = k1_cand*t;
        sPP[5] = k2_cand*t;
        sPP[6] = -k2_cand*t;
        sPP[7] = -k1_cand*k2_cand*tto2;
        sPP[8] = 1-k2_cand*k2_cand*tto2;
        
        /*
        float k     = std::sqrt(k1_cand*k1_cand+k2_cand*k2_cand);
        float kk    = k*k;
        float sinkt = std::sin(k*t);
        float coskt = std::cos(k*t);
        
        sPP[0] = sinkt/k;
        sPP[1] = k1_cand*(1-coskt)/kk;
        sPP[2] = k2_cand*(1-coskt)/kk;
        sPP[3] = coskt;
        sPP[4] = k1_cand*sinkt/k;
        sPP[5] = k2_cand*sinkt/k;
        sPP[6] = -sPP[5];
        sPP[7] = k1_cand*k2_cand*(coskt-1)/kk;
        sPP[8] = k1_cand*k1_cand+k2_cand*k2_cand*coskt/kk;
        */
        
    
    }
    
    k1 = k1_cand;
    k2 = k2_cand;
    
}

inline void PTF_with_parameter_priors::prepInitProbePropagator() {
    
    probeStepCount = 0;
    prepProbePropagator();
    probeStepCount = 1;
    return;
    
}

inline void PTF_with_parameter_priors::walk() {
    
    prepCandStepPropagator();
    
    float T[3];
    
	for (int i=0; i<3; i++) {
		p[i]   += sPP[0]*F[0][i] +  sPP[1]*F[1][i]  +  sPP[2]*F[2][i];
		T[i]    = sPP[3]*F[0][i] +  sPP[4]*F[1][i]  +  sPP[5]*F[2][i];
		F[2][i] = sPP[6]*F[0][i] +  sPP[7]*F[1][i]  +  sPP[8]*F[2][i];
	}

	normalize(T);
	cross(F[1],F[2],T);
	normalize(F[1]);
	cross(F[2],T,F[1]);
    
    F[0][0]=T[0];
    F[0][1]=T[1];
    F[0][2]=T[2];
    
	likelihood 	= 0.0;
    
    updateTrackingParameters();
    
}

#endif
