#include "../../tracker_thread.h"
#include "algorithm_ptt.h"

Propagation_Decision TrackWith_PTT::propagate(int stepCounter) {
    
    // Take a step forward
	curve->walk();
    
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
