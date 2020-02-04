#include "../../tracker_thread.h"
#include "algorithm_ptt.h"

Propagation_Decision TrackWith_PTT::propagate(int stepCounter) {        
    
    // Take a step forward
	curve->walk();
    
    // Save tangent in k1-k2 space for C3 Propagation_Decision
    if (curve->initialized==false) {
        curve->initkT(initial_curve);
    }
    
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
