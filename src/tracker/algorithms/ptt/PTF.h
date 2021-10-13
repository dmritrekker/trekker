#ifndef SRC_PTF_H_
#define SRC_PTF_H_

#include "../../../config/config_tracker.h"
#include "../../../math/math_aux.h"
#include "../../../math/doRandomThings.h"
#include "../../../math/sphericalHarmonics.h"

#define SMALL 0.0001
#define BIG   10000

class PTF {

public:

	PTF(RandomDoer *_rndmr);
	~PTF();
	void    init_Frame();
	void    swap(PTF *ptf);

	void    setPosition(Coordinate in) {p[0] = in.x;p[1] = in.y;p[2] = in.z;};    
    void    prepPropagator(float _step);
    void    walk();

	void    getARandomFrame();
	void    getARandomFrame(Coordinate _seed_init_direction);
	void    flip();
	void    print();
    
    float   getk1() {return k1_cand;};
    float   getk2() {return k2_cand;};
    float   getk()  {return std::sqrt(k1_cand*k1_cand+k2_cand*k2_cand);};

    float  *p;			// Position
    float **F;          // TN1N2 frame
    float  *PP;         // Propagator
    
	float 	likelihood;	// Likelihood
	float   lastVal;
    float   lastVal_cand;
    float   initVal;
    
    void    getInitCandidate();
    void    getCandidate();
    void    calcDataSupport();
    

private:
	
	float  k1;
    float  k2;    
    float  k1_cand;
    float  k2_cand;
    
	RandomDoer *rndmr;
    
    float      *FOD;
    FOD_Image  *ptf_FOD;
    float       getFODamp(float *pv, float *Tv);
    
    
    float _p[3];
    float _F[3][3];
    float _T[3];
    float _N1[3];
    float _N2[3];

};


inline float PTF::getFODamp(float *pv, float *Tv) {
    
    if (TRACKER::fodDiscretization==FODDISC_OFF) {
        ptf_FOD->getVal(pv,FOD);
        return SH::SH_amplitude(FOD,Tv);
    } else
        return ptf_FOD->getFODval(pv,Tv);
    
}

inline void PTF::prepPropagator(float _step) {
    
    float t = _step;
    
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


inline void PTF::walk() {
    
    prepPropagator(TRACKER::stepSize);

    k1 = k1_cand;
    k2 = k2_cand;
    
    float T[3];
    
	for (int i=0; i<3; i++) {
		p[i]   += PP[0]*F[0][i] + PP[1]*F[1][i] + PP[2]*F[2][i];
		T[i]    = PP[3]*F[0][i] + PP[4]*F[1][i] + PP[5]*F[2][i];
		F[2][i] = PP[6]*F[0][i] + PP[7]*F[1][i] + PP[8]*F[2][i];
	}

	normalize(T);
	cross(F[1],F[2],T);
	normalize(F[1]);
	cross(F[2],T,F[1]);
    
    F[0][0]     = T[0];
    F[0][1]     = T[1];
    F[0][2]     = T[2];
    
	likelihood 	= 0.0;
    
}

#endif
