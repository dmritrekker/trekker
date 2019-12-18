#include "../../tracker_thread.h"
#include "algorithm_ptt.h"
#include "fast_PTF.h"

void TrackWith_PTT::get_a_candidate_curve() {
	curve->setCandidateIndex(PTF_CONSTS::valid_indices.at(doRandomThings->uniform_int()));
	calcLikelihoodAndPosterior();
}

Propagation_Decision TrackWith_PTT::propagate(int stepCounter) {

	curve->walk();

    // Estimate posterior
    if (stepCounter%maxEstInterval==0)
        estimatePosteriorMax();

	// Rejection sample
	rejectionSample();

	if (curve->posterior == -2) {
		return FAIL;
	} else if (curve->posterior == -1) {
		return STOP;
	} else
		return CONTINUE;

}
