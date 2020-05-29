#include "algorithm_ptt.h"

void TrackWith_PTT::rejectionSample() {

	int tries;
	int reject 	= 0;

	for (tries=0; tries<TRACKER::triesPerRejectionSampling; tries++) {

		curve->getCandidate();

		if (curve->likelihood < modMinFodAmp ) {
			reject++;
		} else if (curve->likelihood > posteriorMax) {
			curve->likelihood = -2;
			break;
		} else if (doRandomThings->uniform_01()*posteriorMax < curve->likelihood ) {
            // This candidate is now selected and it will be propagated
            curve->lastVal = curve->lastVal_cand;
			break;
		}

	}
	
	if (tries==TRACKER::triesPerRejectionSampling) {
		curve->likelihood = -1;
	} else {
		// Update information for streamline ONLY when it is a legitimate propagation
		// This just makes sure that the displayed report is not dominated by tons of
		// trials made when streamlines terminate due to low data support
		// Otherwise, it does not change the tracking algorithm
		streamline->sampling_prop_generated       	+= 1;
		streamline->sampling_prop_tries  			+= tries;
		streamline->sampling_prop_reject 			+= reject;
		if (curve->likelihood==-2)
			streamline->sampling_prop_fail 			 = 1;
	}

}
