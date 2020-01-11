#ifndef SRC_FAST_PTF_H_
#define SRC_FAST_PTF_H_

#include "../../../config/config_tracker.h"
#include "../../../math/math_aux.h"
#include "../../../math/doRandomThings.h"
#include "fast_PTF_discretizer.h"

using namespace PTF_CONSTS;

class FPTF {

public:

	FPTF(float *_p);
	FPTF(Coordinate _p);
	FPTF(RandomDoer *_rndmr);
	~FPTF();
	void  init_Frame();
    void  updateF();
	void  swap(FPTF *ptf);

	void  setPosition(Coordinate _p)					{p[0] = _p.x;p[1] = _p.y;p[2] = _p.z;};
	void  setIndex(size_t _index) 						{index           = _index;};
	void  setCandidateIndex(size_t _candidate_index) 	{candidate_index = _candidate_index;};
	void  setToCandidate();

	void  flushProbs();
	void  getARandomFrame();
	void  getARandomFrame(Coordinate _seed_init_direction);
	void  flip();
	void  walk();
	void  print();

	float  *p;			// Position

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

#endif
