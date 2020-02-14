#ifndef SRC_TRACKER_ALGORITHMS_ALGORITHM_LOCAL_PROBABILISTIC_H_
#define SRC_TRACKER_ALGORITHMS_ALGORITHM_LOCAL_PROBABILISTIC_H_

#include "../../../base/algorithm.h"
#include "../../../math/sphericalHarmonics.h"

#include "streamline_local_probabilistic.h"
#include "tractogram_local_probabilistic.h"
#include "algorithm_local_probabilistic_defaults.h"

class TrackWith_Local_Probabilistic : public TractographyAlgorithm {

public:

	TrackWith_Local_Probabilistic();
	~TrackWith_Local_Probabilistic();

	virtual Initialization_Decision initialize();
	virtual Propagation_Decision	propagate(int count);
	virtual void setSeed();
	virtual void flip();
	virtual void append();
	virtual void removeLast();
	virtual void setStreamline(Streamline *_streamline) { streamline = (Streamline_Local_Probabilistic*)_streamline;};
	virtual void setDefaultParametersWhenNecessary();
	virtual void print();




private:

	float 		 get_initial_curve();
	float 		 get_a_candidate_curve();
	float 		 rejectionSample();
	void 		 estimatePosteriorMax();


	float 		 posteriorMax;
	size_t 		 current_init_postEstItCount;
	size_t 		 current_prop_postEstItCount;

	float  	    *initial_point;
	float 	    *initial_direction;

	float 		*current_point;
	float       *previous_direction;
	float       *candidate_direction;

	float 		*FOD;

	RandomDoer  *doRandomThings;
	Streamline_Local_Probabilistic *streamline;

	float 		 constant_asinterm;

};

#endif
