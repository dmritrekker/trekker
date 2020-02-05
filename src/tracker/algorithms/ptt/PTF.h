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
	void  swap(PTF *ptf);

	void  setPosition(Coordinate _p) {p[0] = _p.x;p[1] = _p.y;p[2] = _p.z;};
    void  getACandidatek1k2();
	// void  setToCandidate(){k1=k1_cand; k2=k2_cand; kT1=kT1_cand; kT2=kT2_cand;};
    void  initkT(PTF *ptf);    
    
    void  prepProbePropagator();
    void  prepProbePropagator_C1();
    void  prepProbePropagator_C2();
    void  prepProbePropagator_C3(float r,float xc,float yc,float alpha,float theta);
    
    void  prepCandStepPropagator();
    void  prepCandStepPropagator_C1();
    void  prepCandStepPropagator_C2();
    void  prepCandStepPropagator_C3(float r,float xc,float yc,float alpha,float theta);
    
    void  walk();
    void  walkCandStep(float *init_p, float **init_F);

	void  getARandomFrame();
	void  getARandomFrame(Coordinate _seed_init_direction);
	void  flip();
	void  print();
    
    float getk1() {return k1;};
    float getk2() {return k2;};
    float getk()  {return std::sqrt(k1*k1+k2*k2);};

	float  *p;			// Position
    float **F;          // TN1N2 frame
	
	float  k1;
    float  k2;
    float  kT1;
    float  kT2;
    
    float  k1_cand;
    float  k2_cand;
    float  kT1_cand;
    float  kT2_cand;
    
    float  *PP;         // Propagator for probe
    float  *sPP;        // Propagator for step

	float 	likelihood;	// Likelihood
	float 	prior;		// Prior probability
	float 	posterior;	// Posterior probability
	
	bool    initialized;
    
    int     probeStepCount;
    float   probe_k1;
    float   probe_k2;

private:
    
	RandomDoer *rndmr;

};

inline void PTF::getACandidatek1k2() {
    rndmr->getARandomPointWithDisk(&k1_cand, &k2_cand, TRACKER::maxCurvature);
}

inline void PTF::prepProbePropagator_C1() {
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

inline void PTF::prepProbePropagator_C2() {
    
    float t    = TRACKER::probeStepSize;
    float tto2 = t*t/2;
    float twos = 2*TRACKER::probeStepSize;
    
    PP[0] = t;
    PP[1] = probe_k1*tto2;
    PP[2] = probe_k2*tto2;
    PP[3] = 1 + (-probe_k1*probe_k1 - probe_k2*probe_k2)*tto2;
    PP[4] = -t*((probe_k1-k1_cand)*t/twos - probe_k1);
    PP[5] = -t*((probe_k2-k2_cand)*t/twos - probe_k2);
    PP[6] = -PP[5];
    PP[7] = -probe_k1*probe_k2*tto2;
    PP[8] = 1-probe_k2*probe_k2*tto2;
    
    probe_k1 = (1 - probeStepCount*t/TRACKER::probeLength)*probe_k1 + probeStepCount*t/TRACKER::probeLength*k1_cand;
    probe_k2 = (1 - probeStepCount*t/TRACKER::probeLength)*probe_k2 + probeStepCount*t/TRACKER::probeLength*k2_cand;
    
}


inline void PTF::prepCandStepPropagator_C1() {
    sPP[0] = TRACKER::stepSize;
    sPP[4] = k1_cand*TRACKER::stepSize;
    sPP[5] = k2_cand*TRACKER::stepSize;
    sPP[1] = sPP[4]*TRACKER::stepSize/2;
    sPP[2] = sPP[5]*TRACKER::stepSize/2;
    sPP[8] = 1-k2_cand*sPP[2];
    sPP[3] = sPP[8]-k1_cand*sPP[1];
    sPP[6] = -sPP[5];
    sPP[7] = -sPP[4]*sPP[5]/2;
    
    k1 = k1_cand;
    k2 = k2_cand;
}

inline void PTF::prepCandStepPropagator_C2() {
    
    float t    = TRACKER::stepSize;
    
    float tto2 = t*t/2;
    float twos = 2*TRACKER::stepSize;
    
    sPP[0] = t;
    sPP[1] = k1*tto2;
    sPP[2] = k2*tto2;
    sPP[3] = 1 + (-k1*k1 - k2*k2)*tto2;
    sPP[4] = -t*((k1-k1_cand)*t/twos - k1);
    sPP[5] = -t*((k2-k2_cand)*t/twos - k2);
    sPP[6] = -sPP[5];
    sPP[7] = -k1*k2*tto2;
    sPP[8] = 1-k2*k2*tto2;
    
    k1 = (1 - probeStepCount*t/TRACKER::probeLength)*k1 + probeStepCount*t/TRACKER::probeLength*k1_cand;
    k2 = (1 - probeStepCount*t/TRACKER::probeLength)*k2 + probeStepCount*t/TRACKER::probeLength*k2_cand;
}

inline void PTF::prepCandStepPropagator_C3(float r,float xc,float yc,float alpha,float theta) {
    
    float t             = TRACKER::stepSize;
    float tt            = t*t;
    float ttt           = tt*t;
    float rr            = r*r;
    float xcxc          = xc*xc;
    float ycyc          = yc*yc;
    float thetatheta    = theta*theta;
    float thetacube     = thetatheta*theta;
    float cosa          = std::cos(alpha);
    float sina          = std::sin(alpha);
    float cosacosa      = cosa*cosa;
    float s             = TRACKER::stepSize;
    float ss            = s*s;
    float sss           = ss*s;
    
    sPP[0] = -((2*cosa*r*tt*xc + 2*sina*r*tt*yc - 6 + (rr + xcxc + ycyc)*tt)*t)/6;
    sPP[1] = -((r*sina*theta*t - (3*cosa*r*s)/2 - (3*xc*s)/2)*tt)/(3*s);
    sPP[2] =  ((r*cosa*theta*t + (3*s*(r*sina + yc))/2)*tt)/(3*s);
    sPP[3] =  (-2*r*tt*(t*theta*yc + xc*s)*cosa + 2*r*tt*(t*theta*xc - yc*s)*sina - s*(-2 + (rr + xcxc + ycyc)*tt))/(2*s);
    sPP[4] =  1/(12*sss)*(t*(-7*rr*(t*theta*yc + (4*xc*s)/7)*tt*ss*cosacosa - 2*(-(7*r*tt*(t*theta*xc - (4*yc*s)/7)*s*sina)/2 + (-6 + (rr + 3*xcxc + ycyc)*tt)*ss + (5*ttt*xc*yc*theta*s)/2 + 4*tt*thetatheta)*r*s*cosa + (-4*t*xc*yc*sss + (-12 + (rr + 6*xcxc + ycyc)*tt)*theta*ss + 4*tt*thetacube)*r*t*sina + 2*ss*(-s*xc*(-6 + (rr + xcxc + ycyc)*tt) + rr*ttt*theta*yc)));
    sPP[5] =  -((r*(2*r*tt*xc*ss*cosa + (-6 + (rr + xcxc + 3*ycyc)*tt)*ss + 4*tt*thetatheta)*sina - 2*s*(rr*tt*yc*s*cosacosa - r*t*(t*xc*yc*s - 3*theta)*cosa - 3*(-2 + (rr + xcxc/3 + ycyc/3)*tt)*s*yc/2))*t)/(6*ss);
    sPP[6] = -sPP[5];
    sPP[7] =  -tt*(6*cosacosa*rr*t*theta + 3*cosa*sina*rr*s + 4*cosa*r*t*theta*xc - 2*sina*r*t*theta*yc + 3*cosa*r*yc*s + 3*sina*r*xc*s - 2*rr*t*theta + 3*xc*yc*s)/(6*s);
    sPP[8] =   (cosacosa*rr*tt*s - 2*r*ttt*theta*(r*sina + yc)*cosa - (2*sina*r*tt*yc - 2 + (rr + ycyc)*tt)*s)/(2*s);
    
}

inline void PTF::walkCandStep(float *init_p, float **init_F) {
    
    for (int i=0; i<3; i++) {
        init_p[i]     = p[i] + sPP[0]*F[0][i] +  sPP[1]*F[1][i]  +  sPP[2]*F[2][i];
        init_F[0][i]  =        sPP[3]*F[0][i] +  sPP[4]*F[1][i]  +  sPP[5]*F[2][i];
        init_F[2][i]  =        sPP[6]*F[0][i] +  sPP[7]*F[1][i]  +  sPP[8]*F[2][i];
    }
    
    normalize(init_F[0]);
    cross(init_F[1],init_F[2],init_F[0]);
    normalize(init_F[1]);
    cross(init_F[2],init_F[0],init_F[1]);
    
}

#endif
