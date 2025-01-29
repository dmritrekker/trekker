#pragma once

#include "base/nibr.h"
#include "../../tracker/tracker.h"
#include "math/core.h"
#include "algorithm_ptt.h"
#include <cmath>
#include <random>
#include <cfloat>
#include <iostream>

namespace NIBR {

// Parallel transport frame
class PTF {

public:

    // For each streamline, create a new PTF object with tracking parameters
    PTF(TrackWith_PTT *_tracker);
    ~PTF();

    float  *p; // Last position
    float **F; // Frame
    float  k1;
    float  k2;

    float       likelihood;
    RandomDoer  doRandomThings;
    
    // First set the (initial) position of the parallel transport frame (PTF), i.e. set the seed point
    void  setPosition(float* pos);
    
    // After initial position is set, a random PTF (a walking frame, i.e., 3 orthonormal vectors (F), plus 2 scalars, i.e., k1 and k2) is set with this function.
    // Optionally, the tangential component of PTF can be user provided with the input initDir parameter.
    // Use initDir=NULL if initDir is not available.
    // A point + PTF parametrizes a curve that is named the "probe". Using probe parameters (probeLength, probeRadius, probeQuality, probeCount),
    // a short fiber bundle segment is modelled. 
    // This function does NOT pick the initial curve. It only returns the datasupport (likelihood) value for a randomly picked candidate.
    float getInitCandidate(float *initDir);
    
    // Propagates the last position (p) by stepSize amount, using the parameters of the last candidate.
    void  walk();
    
    // Using the current position, pick a random curve parametrization. The walking frame (F) is same, only the k1 and k2 are randomly picked. This was a smooth curve is sampled.
    // This function does NOT pick the next curve. It only returns the datasupport (likelihood) value for the randomly picked candidate.
    float getCandidate();

    // Returns the data support for a given k1-k2 pair
    float calcDataSupport(float _k1, float _k2) {k1_cand = _k1; k2_cand = _k2; return calcDataSupport();}

    // Copy
    void  copy(PTF *ptf);

    // Copies PTF parameters then flips the curve. This function can be used after the initial curve is picked in order to save a copy of the curve for tracking towards the other side.  
    void  getFlippedCopy(PTF *ptf);

    // After the candidate is found in the algorithm, use resetFirstVal. 
    // Likelihood is computed by adding values along probeQuality. The first value will be same for all the candidates. So there is no need to repeat computation.
    void  resetFirstVal() {firstVal = NAN;}

    // Initial posterior max is used after flipping
    void   setInitPosteriorMax(float postMax) {initPosteriorMax = postMax;}
    float  getInitPosteriorMax() {return initPosteriorMax;}

    // Refreshes parameters for the current position of the curve
    void  refreshParams();
    void  print();

private:

    TrackWith_PTT* tracker;

    float  k1_cand;
    float  k2_cand;
    float *PP; // Propagator
    
    void   getARandomFrame(float* _dir);
    float  calcLocalDataSupport(float* _p, float* _dir);
    float  calcDataSupport();
    void   prepPropagator(float t);
    
    float  angularSeparation;
    float  probeStepSize;
    float  probeNormalizer;
    
    float  firstVal;
    float  initFirstVal;        // This is the initial first val that is saved to be used after flipping
    float  initPosteriorMax;    // This is the posterior max at initialization


};


inline void PTF::prepPropagator(float t) {
    
    if ( (std::fabs(k1_cand)<EPS4) && (std::fabs(k2_cand)<EPS4) ) {
        
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
        
        if (std::fabs(k1_cand)<EPS4) k1_cand = EPS4;
        if (std::fabs(k2_cand)<EPS4) k2_cand = EPS4;

        float k     = std::sqrt(k1_cand*k1_cand+k2_cand*k2_cand);
        float sinkt = std::sin(k*t);
        float coskt = std::cos(k*t);
        float kk    = 1/(k*k);
        
        PP[0] = sinkt/k;
        PP[1] = k1_cand*(1-coskt)*kk;
        PP[2] = k2_cand*(1-coskt)*kk;
        PP[3] = coskt;
        PP[4] = k1_cand*sinkt/k;
        PP[5] = k2_cand*sinkt/k;
        PP[6] = -PP[5];
        PP[7] = k1_cand*k2_cand*(coskt-1)*kk;
        PP[8] = (k1_cand*k1_cand+k2_cand*k2_cand*coskt)*kk;
    }
    
}

inline void PTF::walk() {
    
    prepPropagator(tracker->stepSize);
    k1 = k1_cand;
    k2 = k2_cand;
    
    float T[3];
    
    for (int i=0; i<3; i++) {
        p[i]   += PP[0]*F[0][i] +  PP[1]*F[1][i]  +  PP[2]*F[2][i];
        T[i]    = PP[3]*F[0][i] +  PP[4]*F[1][i]  +  PP[5]*F[2][i];
        F[2][i] = PP[6]*F[0][i] +  PP[7]*F[1][i]  +  PP[8]*F[2][i];
    }

    normalize(T);
    cross(F[1],F[2],T);
    normalize(F[1]);
    cross(F[2],T,F[1]);
    
    F[0][0]=T[0];
    F[0][1]=T[1];
    F[0][2]=T[2];
    
    likelihood 	= 0.0;
    
}

inline void PTF::refreshParams() {
    angularSeparation = 2.0*M_PI/float(tracker->probeCount);
    probeStepSize     = tracker->probeLength/(tracker->probeQuality-1);
    if (TRACKER::params_ptt.img_FOD->getSHorder()%2==0) {
        probeNormalizer   = 1/float(tracker->probeQuality*tracker->probeCount);
    } else {
        probeNormalizer   = 1/float((tracker->probeQuality-1)*tracker->probeCount);
    }
}

}