#ifndef SRC_PTF_H_
#define SRC_PTF_H_

#include "../../../config/config_tracker.h"
#include "../../../math/math_aux.h"
#include "../../../math/doRandomThings.h"

class PTF {

public:

	PTF(RandomDoer *_rndmr);
	~PTF();
	void  init_Frame();
    void  updateF();
	void  swap(PTF *ptf);

	void  setPosition(Coordinate _p) {p[0] = _p.x;p[1] = _p.y;p[2] = _p.z;};
    void  getACandidatek1k2();
	void  setToCandidate(){k1=k1_cand; k2=k2_cand;};
    void  prepProbePropagator();
    void  prepStepPropagator();

	void  getARandomFrame();
	void  getARandomFrame(Coordinate _seed_init_direction);
	void  flip();
	void  walk();
	void  print();
    
    float getk1() {return k1;};
    float getk2() {return k2;};
    float getk()  {return std::sqrt(k1*k1+k2*k2);};

	float  *p;			// Position

	float  k1;
    float  k2;
    float  kT1;
    float  kT2;
    
    float  k1_cand;
    float  k2_cand;
	
	float  *T; 			// Tangent
	float  *N1;
	float  *N2;
    float **F;          // TN1N2 frame
    float  *PP;         // Probe propagator

	float 	likelihood;	// Likelihood
	float 	prior;		// Prior probability
	float 	posterior;	// Posterior probability

private:
    
	RandomDoer *rndmr;

};

inline void PTF::getACandidatek1k2() {
    rndmr->getARandomPointWithDisk(&k1_cand, &k2_cand, TRACKER::maxCurvature);
}

inline void PTF::prepProbePropagator() {
    PP[0] = TRACKER::probeStepSize;
    PP[4] = k1_cand*TRACKER::probeStepSize;
    PP[5] = k2_cand*TRACKER::probeStepSize;
    PP[1] = PP[4]*TRACKER::probeStepSize/2;
    PP[2] = PP[5]*TRACKER::probeStepSize/2;
    PP[8] = 1-k2_cand*PP[2];
    PP[3] = PP[8]-k1_cand*PP[1];
    PP[6] = -PP[5];
    PP[7] = -PP[4]*PP[5]/2;
}

inline void PTF::prepStepPropagator() {
    PP[0] = TRACKER::stepSize;
    PP[4] = k1*TRACKER::stepSize;
    PP[5] = k2*TRACKER::stepSize;
    PP[1] = PP[4]*TRACKER::stepSize/2;
    PP[2] = PP[5]*TRACKER::stepSize/2;
    PP[8] = 1-k2*PP[2];
    PP[3] = PP[8]-k1*PP[1];
    PP[6] = -PP[5];
    PP[7] = -PP[4]*PP[5]/2;
}

#endif
