#include "../../tracker_thread.h"
#include "algorithm_ptt.h"
#include "fast_PTF.h"

void TrackWith_PTT::get_a_candidate_curve() {
    
    // float tau = INFINITY;
    
    //while (tau > TRACKER::maxTorsion) {
    
        // curve->setCandidateIndex(PTF_CONSTS::valid_indices.at(doRandomThings->uniform_int()));
        // float ang = std::acos((float(curve->k1_ind*curve->candidate_k1_ind + curve->k2_ind*curve->candidate_k2_ind))/(std::sqrt(float(curve->k1_ind*curve->k1_ind + curve->k2_ind*curve->k2_ind))*std::sqrt(float(curve->candidate_k1_ind*curve->candidate_k1_ind + curve->candidate_k2_ind *curve->candidate_k2_ind))));
        
        
        
        curve->getACandidatek1k2();
        
        
        //float ang = std::acos((float(curve->k1*curve->k1_cand + curve->k2*curve->k2_cand))/(std::sqrt(float(curve->k1*curve->k1 + curve->k2*curve->k2))*std::sqrt(float(curve->k1_cand*curve->k1_cand + curve->k2_cand *curve->k2_cand))));
        
        //if (ang>PI/2) ang -= PI/2;
        
        //tau = ang/probeLength;
        
    //}
    
    
	calcLikelihoodAndPosterior();
}


Propagation_Decision TrackWith_PTT::propagate(int stepCounter) {

    // Take a step forward
	curve->walk();

    // Estimate posterior
    if (stepCounter%maxEstInterval==0) {
        estimatePosteriorMax();
    }

	// Rejection sample
	rejectionSample();

	if (curve->posterior == -2) {
		return FAIL;
	} else if (curve->posterior == -1) {
		return STOP;
	} else
		return CONTINUE;

}
