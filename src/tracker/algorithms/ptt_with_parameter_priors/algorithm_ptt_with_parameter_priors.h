#ifndef SRC_TRACKER_ALGORITHMS_PTT_WITH_PARAMETER_PRIORS_H_
#define SRC_TRACKER_ALGORITHMS_PTT_WITH_PARAMETER_PRIORS_H_

#include "../../../base/algorithm.h"
#include "../../../math/sphericalHarmonics.h"
#include "PTF_with_parameter_priors.h"
#include "streamline_ptt_with_parameter_priors.h"
#include "tractogram_ptt_with_parameter_priors.h"
#include "algorithm_ptt_with_parameter_priors_defaults.h"

class TrackWith_PTT_with_parameter_priors : public TractographyAlgorithm {

public:

	TrackWith_PTT_with_parameter_priors();
	~TrackWith_PTT_with_parameter_priors();

	virtual Initialization_Decision initialize();
	virtual Propagation_Decision 	propagate(int stepCounter);
	virtual void setSeed();
	virtual void flip();
	virtual void append();
	virtual void removeLast();
	virtual void setStreamline(Streamline *_streamline) { streamline = (Streamline_PTT_with_parameter_priors*)_streamline;};
	virtual void setDefaultParametersWhenNecessary();
	virtual void print();

	PTF_with_parameter_priors 			*curve;

private:

	void 		 	 get_initial_curve();
	void 		 	 get_a_candidate_curve();
	void 			 estimatePosteriorMax();
	void 		 	 rejectionSample();
    void 		 	 updateTrackingParameters();

	PTF_with_parameter_priors 			*initial_curve;
    float 		 	 initialPosteriorMax;
    
	float 		 	 posteriorMax;
    float 		 	 posteriorMaxScalingFactor;
    
	size_t 		 	 current_init_postEstItCount;
	size_t 		 	 current_prop_postEstItCount;


	RandomDoer  	*doRandomThings;

	Streamline_PTT_with_parameter_priors 	*streamline;
    
};

#endif
