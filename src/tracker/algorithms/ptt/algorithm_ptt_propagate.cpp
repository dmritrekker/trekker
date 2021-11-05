#include "../../tracker_thread.h"
#include "algorithm_ptt.h"

Propagation_Decision TrackWith_PTT::propagate(int stepCounter) {
    
    // Take a step forward
	curve->walk();

	// Update currMinFODamp
	if (TRACKER::useMinFODampImage) {
		float tmp;
		TRACKER::img_minFODamp->getVal(curve->p,&tmp);
		currMinFODamp = std::pow(tmp,TRACKER::dataSupportExponent);	
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
