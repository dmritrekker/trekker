#ifndef SRC_PTF_H_
#define SRC_PTF_H_

#include "../../../config/config_tracker.h"
#include "../../../math/math_aux.h"
#include "../../../math/doRandomThings.h"

#define SMALL 0.0001
#define BIG   10000

class PTF {

public:

	PTF(RandomDoer *_rndmr);
	~PTF();
	void  init_Frame();
	void  swap(PTF *ptf);

	void  setPosition(Coordinate _p) {p[0] = _p.x;p[1] = _p.y;p[2] = _p.z;};
    void  getACandidatek1k2();
    void  initkT(PTF *ptf);    
    
    void  prepInitProbePropagator();
    void  prepProbePropagator();
    void  prepProbePropagator_C1();
    void  prepProbePropagator_C2();
    void  prepProbePropagator_C3();
    
    void  prepCandStepPropagator();
    void  prepCandStepPropagator_C1();
    void  prepCandStepPropagator_C2();
    void  prepCandStepPropagator_C3();
    
    void  walk();

    void  getCurrentCurve(float* p, float **F);
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
	
	bool    initialized;
    
    int     probeStepCount;

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
    
    bool   C2_useC1;
    float  C2_k1_step;
    float  C2_k2_step;
    bool   C3_useC1;
    bool   C3_useC2;
    float  C3_alpha;
    float  C3_theta;
    float  C3_thetaStep;
    float  C3_sign;
    float  C3_xc;
    float  C3_yc;
    float  C3_r;
    
    
    float  *sPP;        // Propagator for step
    
    
	RandomDoer *rndmr;

};



inline void PTF::getACandidatek1k2() {
    rndmr->getARandomPointWithinDisk(&k1_cand, &k2_cand, TRACKER::maxCurvature);
}


inline void PTF::getCurrentCurve(float* _p, float **_F) {
    
    for (int i=0; i<3; i++) {
        _p[i] 	= p[i];
        for (int j=0; j<3; j++) {
            _F[i][j] = F[i][j];
        }
        
	}
    
}


inline void PTF::prepProbePropagator_C1() {
    
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
    
    }
    
}



inline void PTF::prepProbePropagator_C2() {
    
    float t     = TRACKER::probeStepSize;
    float to2   = t/float(2);
    float tto2  = t*t/float(2);
        
    PP[0] = t;
    PP[1] = probe_k1*tto2;
    PP[2] = probe_k2*tto2;
    PP[3] = 1 + (-probe_k1*probe_k1 - probe_k2*probe_k2)*tto2;
    PP[4] =  (probe_k1+probe_k1+C2_k1_step)*to2;
    PP[5] =  (probe_k2+probe_k2+C2_k2_step)*to2;
    PP[6] = -(probe_k2+probe_k2+C2_k2_step)*to2;
    PP[7] = -probe_k1*probe_k2*tto2;
    PP[8] = 1-probe_k2*probe_k2*tto2;
        
    probe_k1 += C2_k1_step;
    probe_k2 += C2_k2_step;
    
}



inline void PTF::prepProbePropagator_C3() {
    
    float t             = TRACKER::probeStepSize;
    float tt            = t*t;
    float ttt           = tt*t;
    float rr            = C3_r*C3_r;
    float xcxc          = C3_xc*C3_xc;
    float ycyc          = C3_yc*C3_yc;
    float thetatheta    = C3_thetaStep*C3_thetaStep;
    float thetacube     = thetatheta*C3_thetaStep;
    
    float alpha         = C3_alpha + probeStepCount*C3_thetaStep;
    float cosa          = std::cos(alpha);
    float sina          = std::sin(alpha);
    float cosacosa      = cosa*cosa;
    
    PP[0] = -((2*cosa*C3_r*tt*C3_xc + 2*sina*C3_r*tt*C3_yc - 6 + (rr + xcxc + ycyc)*tt)*t)/6;
    PP[1] = -((C3_r*sina*C3_thetaStep*t - (3*cosa*C3_r*t)/2 - (3*C3_xc*t)/2)*tt)/(3*t);
    PP[2] =  ((C3_r*cosa*C3_thetaStep*t + (3*t*(C3_r*sina + C3_yc))/2)*tt)/(3*t);
    PP[3] =  (-2*C3_r*tt*(t*C3_thetaStep*C3_yc + C3_xc*t)*cosa + 2*C3_r*tt*(t*C3_thetaStep*C3_xc - C3_yc*t)*sina - t*(-2 + (rr + xcxc + ycyc)*tt))/(2*t);
    PP[4] =  1/(12*ttt)*(t*(-7*rr*(t*C3_thetaStep*C3_yc + (4*C3_xc*t)/7)*tt*tt*cosacosa - 2*(-(7*C3_r*tt*(t*C3_thetaStep*C3_xc - (4*C3_yc*t)/7)*t*sina)/2 + (-6 + (rr + 3*xcxc + ycyc)*tt)*tt + (5*ttt*C3_xc*C3_yc*C3_thetaStep*t)/2 + 4*tt*thetatheta)*C3_r*t*cosa + (-4*t*C3_xc*C3_yc*ttt + (-12 + (rr + 6*xcxc + ycyc)*tt)*C3_thetaStep*tt + 4*tt*thetacube)*C3_r*t*sina + 2*tt*(-t*C3_xc*(-6 + (rr + xcxc + ycyc)*tt) + rr*ttt*C3_thetaStep*C3_yc)));
    PP[5] =  -((C3_r*(2*C3_r*tt*C3_xc*tt*cosa + (-6 + (rr + xcxc + 3*ycyc)*tt)*tt + 4*tt*thetatheta)*sina - 2*t*(rr*tt*C3_yc*t*cosacosa - C3_r*t*(t*C3_xc*C3_yc*t - 3*C3_thetaStep)*cosa - 3*(-2 + (rr + xcxc/3 + ycyc/3)*tt)*t*C3_yc/2))*t)/(6*tt);
    PP[6] = ((C3_r*(2*C3_r*tt*C3_xc*tt*cosa + (-6 + (rr + xcxc + 3*ycyc)*tt)*tt + 4*tt*thetatheta)*sina - 2*t*(rr*tt*C3_yc*t*cosacosa - C3_r*t*(t*C3_xc*C3_yc*t - 3*C3_thetaStep)*cosa - 3*(-2 + (rr + xcxc/3 + ycyc/3)*tt)*t*C3_yc/2))*t)/(6*tt);
    PP[7] = -tt*(6*cosacosa*rr*t*C3_thetaStep + 3*cosa*sina*rr*t + 4*cosa*C3_r*t*C3_thetaStep*C3_xc - 2*sina*C3_r*t*C3_thetaStep*C3_yc + 3*cosa*C3_r*C3_yc*t + 3*sina*C3_r*C3_xc*t - 2*rr*t*C3_thetaStep + 3*C3_xc*C3_yc*t)/(6*t);
    PP[8] = (cosacosa*rr*tt*t - 2*C3_r*ttt*C3_thetaStep*(C3_r*sina + C3_yc)*cosa - (2*sina*C3_r*tt*C3_yc - 2 + (rr + ycyc)*tt)*t)/(2*t);
 
}





inline void PTF::prepCandStepPropagator_C1() {
    
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
    
    }
    
    k1 = k1_cand;
    k2 = k2_cand;
    
}


inline void PTF::prepCandStepPropagator_C2() {
    
    k1_cand     = (1 - TRACKER::probeStepSize/TRACKER::probeLength)*k1 + TRACKER::probeStepSize/TRACKER::probeLength*k1_cand;
    k2_cand     = (1 - TRACKER::probeStepSize/TRACKER::probeLength)*k2 + TRACKER::probeStepSize/TRACKER::probeLength*k2_cand;
    
    float t     = TRACKER::stepSize;
    float to2   = t/float(2);
    float tto2  = t*t/float(2);
        
    sPP[0] = t;
    sPP[1] = k1*tto2;
    sPP[2] = k2*tto2;
    sPP[3] = 1 + (-k1*k1 - k2*k2)*tto2;
    sPP[4] =  (k1+k1_cand)*to2;
    sPP[5] =  (k2+k2_cand)*to2;
    sPP[6] = -(k2+k2_cand)*to2;
    sPP[7] = -k1*k2*tto2;
    sPP[8] = 1-k2*k2*tto2;
    
    k1 = k1_cand;
    k2 = k2_cand;
    
}


inline void PTF::prepCandStepPropagator_C3() {
    
    C3_thetaStep        = 2*C3_theta*TRACKER::stepSize/TRACKER::probeLength;
    
    float t             = TRACKER::stepSize;
    float tt            = t*t;
    float ttt           = tt*t;
    float rr            = C3_r*C3_r;
    float xcxc          = C3_xc*C3_xc;
    float ycyc          = C3_yc*C3_yc;
    float thetatheta    = C3_thetaStep*C3_thetaStep;
    float thetacube     = thetatheta*C3_thetaStep;
    
    float alpha         = C3_alpha;
    float cosa          = std::cos(alpha);
    float sina          = std::sin(alpha);
    float cosacosa      = cosa*cosa;
    
    
    
    sPP[0] = -((2*cosa*C3_r*tt*C3_xc + 2*sina*C3_r*tt*C3_yc - 6 + (rr + xcxc + ycyc)*tt)*t)/6;
    sPP[1] = -((C3_r*sina*C3_thetaStep*t - (3*cosa*C3_r*t)/2 - (3*C3_xc*t)/2)*tt)/(3*t);
    sPP[2] =  ((C3_r*cosa*C3_thetaStep*t + (3*t*(C3_r*sina + C3_yc))/2)*tt)/(3*t);
    sPP[3] =  (-2*C3_r*tt*(t*C3_thetaStep*C3_yc + C3_xc*t)*cosa + 2*C3_r*tt*(t*C3_thetaStep*C3_xc - C3_yc*t)*sina - t*(-2 + (rr + xcxc + ycyc)*tt))/(2*t);
    sPP[4] =  1/(12*ttt)*(t*(-7*rr*(t*C3_thetaStep*C3_yc + (4*C3_xc*t)/7)*tt*tt*cosacosa - 2*(-(7*C3_r*tt*(t*C3_thetaStep*C3_xc - (4*C3_yc*t)/7)*t*sina)/2 + (-6 + (rr + 3*xcxc + ycyc)*tt)*tt + (5*ttt*C3_xc*C3_yc*C3_thetaStep*t)/2 + 4*tt*thetatheta)*C3_r*t*cosa + (-4*t*C3_xc*C3_yc*ttt + (-12 + (rr + 6*xcxc + ycyc)*tt)*C3_thetaStep*tt + 4*tt*thetacube)*C3_r*t*sina + 2*tt*(-t*C3_xc*(-6 + (rr + xcxc + ycyc)*tt) + rr*ttt*C3_thetaStep*C3_yc)));
    sPP[5] =  -((C3_r*(2*C3_r*tt*C3_xc*tt*cosa + (-6 + (rr + xcxc + 3*ycyc)*tt)*tt + 4*tt*thetatheta)*sina - 2*t*(rr*tt*C3_yc*t*cosacosa - C3_r*t*(t*C3_xc*C3_yc*t - 3*C3_thetaStep)*cosa - 3*(-2 + (rr + xcxc/3 + ycyc/3)*tt)*t*C3_yc/2))*t)/(6*tt);
    sPP[6] = ((C3_r*(2*C3_r*tt*C3_xc*tt*cosa + (-6 + (rr + xcxc + 3*ycyc)*tt)*tt + 4*tt*thetatheta)*sina - 2*t*(rr*tt*C3_yc*t*cosacosa - C3_r*t*(t*C3_xc*C3_yc*t - 3*C3_thetaStep)*cosa - 3*(-2 + (rr + xcxc/3 + ycyc/3)*tt)*t*C3_yc/2))*t)/(6*tt);
    sPP[7] = -tt*(6*cosacosa*rr*t*C3_thetaStep + 3*cosa*sina*rr*t + 4*cosa*C3_r*t*C3_thetaStep*C3_xc - 2*sina*C3_r*t*C3_thetaStep*C3_yc + 3*cosa*C3_r*C3_yc*t + 3*sina*C3_r*C3_xc*t - 2*rr*t*C3_thetaStep + 3*C3_xc*C3_yc*t)/(6*t);
    sPP[8] = (cosacosa*rr*tt*t - 2*C3_r*ttt*C3_thetaStep*(C3_r*sina + C3_yc)*cosa - (2*sina*C3_r*tt*C3_yc - 2 + (rr + ycyc)*tt)*t)/(2*t);
    
    
    C3_sign      = (C3_theta > 0) ? 1 : -1;
    float ang    =  C3_alpha + C3_thetaStep;
    
    k1           = C3_r*std::cos(ang) + C3_xc;
    k2           = C3_r*std::sin(ang) + C3_yc;
    
    kT1          = std::cos(ang + C3_sign*M_PI_2);
    kT2          = std::sin(ang + C3_sign*M_PI_2);
    
}

inline void PTF::prepInitProbePropagator() {
    
    probeStepCount = 0;
    
    if (TRACKER::algorithm == PTT_C1) {
        prepProbePropagator_C1();
        probeStepCount = 1;
        return;
    }
    
    if (TRACKER::algorithm == PTT_C2) {
        
        C2_useC1 = false;
        if ((std::fabs(k1-k1_cand)<SMALL) && (std::fabs(k2-k2_cand)<SMALL)) {
            C2_useC1 = true;
            prepProbePropagator_C1();
        } else {
            C2_k1_step = (k1_cand-k1)/(TRACKER::probeQuality-1);
            C2_k2_step = (k2_cand-k2)/(TRACKER::probeQuality-1);
            probe_k1   = k1;
            probe_k2   = k2;
            prepProbePropagator_C2();
        }
        probeStepCount = 1;
        return;
    }
    
    // If PTT_C3
    C3_useC1 = false;
    if (initialized==false) {
        C3_useC1 = true;
    } else {
        
        bool isAcceptable = false;
        
        while (isAcceptable==false) {
        
            float q1 = k1_cand - k1;
            float q2 = k2_cand - k2;
            
            float nq = std::sqrt(q1*q1+q2*q2);
            
            C3_useC1 = false;
            if (nq<SMALL) {
                C3_useC1    = true;
                isAcceptable  = true;
            } else {
            
                float cosTheta= (q1*kT1+q2*kT2)/nq;
                
                C3_useC2 = false;
                if (abs(abs(cosTheta)-1)<SMALL) {
                    C3_useC2    = true;
                    isAcceptable  = true;
                } else {
                
                    C3_theta       = std::acos(cosTheta);
                    float sinTheta = std::sin(C3_theta);
                    float cqt      = kT2*q1-kT1*q2;
                    
                    float dirC1, dirC2;
                    
                    if (cqt>0) {
                        C3_theta = -C3_theta;
                        dirC1    =  kT2;
                        dirC2    = -kT1;
                    } else {
                        dirC1    = -kT2;
                        dirC2    =  kT1;
                    }
                    
                    
                    C3_r         = nq/(2*sinTheta);
                    C3_xc        = dirC1*C3_r + k1;
                    C3_yc        = dirC2*C3_r + k2;
                    C3_alpha     = std::atan2(k2-C3_yc,k1-C3_xc);
                    C3_thetaStep = 2*C3_theta/float(TRACKER::probeQuality-1);
                    
                    
                    // TODO: Need to apply constraints to eliminate erronous propagations
                    // An example given below, but this is only heuristic and currently does not produce reasonable results
                    if (C3_r < (TRACKER::maxCurvature)) {
                        isAcceptable = true;
                    } else if (std::fabs(2*C3_theta) < M_PI) {
                        C3_useC2     = true;
                        isAcceptable = true;
                    } else {
                        rndmr->getARandomPointWithinDisk(&k1_cand, &k2_cand, TRACKER::maxCurvature);
                    }
                    
                    
                }
            
            }
            
        }
    }
    
    if (C3_useC1) {
        prepProbePropagator_C1();
    } else if (C3_useC2) {
        C2_k1_step = (k1_cand-k1)/float(TRACKER::probeQuality-1);
        C2_k2_step = (k2_cand-k2)/float(TRACKER::probeQuality-1);
        probe_k1   = k1;
        probe_k2   = k2;
        prepProbePropagator_C2();
    } else {
        prepProbePropagator_C3();
        
    }
    
    probeStepCount = 1;
    
}




inline void PTF::prepProbePropagator() {
    
    
    // Nothing needs to be done for PTT_C1
    if (TRACKER::algorithm==PTT_C1) {
        return;
    }
    
    if (TRACKER::algorithm==PTT_C2) {
        prepProbePropagator_C2();
        probeStepCount++;
        return;
    }
    
    if (C3_useC1) {
        ; // Nothing needed in this case since PP is same
    } else if (C3_useC2) {
        prepProbePropagator_C2();
    } else {
        prepProbePropagator_C3();
    }
    
    probeStepCount++;
    return;
    

}

inline void PTF::prepCandStepPropagator() {
    
    if (TRACKER::algorithm==PTT_C1) {
        prepCandStepPropagator_C1();
        return;
    }
    
    if (TRACKER::algorithm==PTT_C2) {
        
        if (C2_useC1) {
            prepCandStepPropagator_C1();
        } else {
            prepCandStepPropagator_C2();
        }       
        
        return;
    }
    
    // If PTT_C3
    if (C3_useC1) {
        prepCandStepPropagator_C1();
    } else if (C3_useC2) {
        prepCandStepPropagator_C2();
    } else {
        prepCandStepPropagator_C3();
    }
    
    return;
    
    
}

inline void PTF::walk() {
    
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
    
	likelihood 	=   0.0;
    
}

#endif
