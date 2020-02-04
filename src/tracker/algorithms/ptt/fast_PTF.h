#ifndef SRC_FAST_PTF_H_
#define SRC_FAST_PTF_H_

#include "../../../config/config_tracker.h"
#include "../../../math/math_aux.h"
#include "../../../math/doRandomThings.h"
#include "fast_PTF_discretizer.h"

using namespace PTF_CONSTS;

class FPTF {

public:

	FPTF(RandomDoer *_rndmr);
	~FPTF();
	void  init_Frame();
    void  updateF();
	void  swap(FPTF *ptf);

	void  setPosition(Coordinate _p) {p[0] = _p.x;p[1] = _p.y;p[2] = _p.z;};
	void  setInitCandidateIndex(size_t _candidate_index);
    void  setCandidateIndex(size_t _candidate_index);
	void  setToCandidate();
    
    size_t  getCandidateIndex(int q);
	
	void  getARandomFrame();
	void  getARandomFrame(Coordinate _seed_init_direction);
	void  flip();
	void  walk();
	void  print();
    
    float getk1() {return float(k1_ind-k_resolution/2)*k_step;};
    float getk2() {return float(k2_ind-k_resolution/2)*k_step;};
    float getk()  {return std::sqrt(getk1()*getk1()+getk2()*getk2());};

	float  *p;			// Position

	int    k1_ind;
    int    k2_ind;
    int    candidate_k1_ind;
    int    candidate_k2_ind;
	
	size_t  index;
	float  *T; 			// Tangent
	float  *N1;
	float  *N2;
    float **F;          // TN1N2 frame

	size_t  candidate_index;

	float 	likelihood;	// Likelihood
	float 	prior;		// Prior probability
	float 	posterior;	// Posterior probability

	RandomDoer *rndmr;

};

inline void FPTF::setInitCandidateIndex(size_t _candidate_index) {
    
    candidate_index  = _candidate_index;
    
    if (TRACKER::algorithm==PTT_C2) {
        k1_ind           = candidate_index/PTFCOEFFMATRIXSIZE;
        k2_ind           = candidate_index%PTFCOEFFMATRIXSIZE; 
        candidate_k1_ind = candidate_index/PTFCOEFFMATRIXSIZE;
        candidate_k2_ind = candidate_index%PTFCOEFFMATRIXSIZE; 
    }
    
}

inline void FPTF::setCandidateIndex(size_t _candidate_index) {
    
    candidate_index  = _candidate_index;
    
    if (TRACKER::algorithm==PTT_C2) {
        candidate_k1_ind = candidate_index/PTFCOEFFMATRIXSIZE;
        candidate_k2_ind = candidate_index%PTFCOEFFMATRIXSIZE; 
    }
    
}

inline void  FPTF::setToCandidate() {
    
    if (TRACKER::algorithm==PTT_C2) {    
        float t = float(1)/float(TRACKER::probeQuality);
        k1_ind  = (1-t)*float(k1_ind) + t*float(candidate_k1_ind);
        k2_ind  = (1-t)*float(k2_ind) + t*float(candidate_k2_ind);
        index   = k1_ind*PTFCOEFFMATRIXSIZE + k2_ind;
    } else {
        index   = candidate_index;
    }
    
}

inline size_t FPTF::getCandidateIndex(int q) {
    
    if (TRACKER::algorithm==PTT_C2) {
        float t = float(q+1)/float(TRACKER::probeQuality);
        int i   = (1-t)*float(k1_ind) + t*float(candidate_k1_ind);
        int j   = (1-t)*float(k2_ind) + t*float(candidate_k2_ind);
        return i*PTFCOEFFMATRIXSIZE + j;
    } else
        return candidate_index;
    
}

#endif
