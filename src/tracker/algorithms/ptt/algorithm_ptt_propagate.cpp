#include "../../tracker_thread.h"
#include "algorithm_ptt.h"

Propagation_Decision TrackWith_PTT::propagate(int stepCounter) {
    
    // Take a step forward
	curve->walk();
    
    // Save tangent in k1-k2 space for C3 Propagation_Decision
    // At stepCounter=1 only one k1-k2 is drawn
    // To initialize kT, stepCounter=2 is needed
    if (stepCounter==2) {
        curve->initkT(initial_curve);
    }
    
    // Estimate posterior
    if (stepCounter%maxEstInterval==0) {
        estimatePosteriorMax();
    }

	// Rejection sample
	rejectionSample();

	if (curve->likelihood == -2) {
		return FAIL;
	} else if (curve->likelihood == -1) {
		return STOP;
	} else
		return CONTINUE;

}
