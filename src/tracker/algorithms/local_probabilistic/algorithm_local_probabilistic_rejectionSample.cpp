#include "algorithm_local_probabilistic.h"

float TrackWith_Local_Probabilistic::rejectionSample() {

	int   tries;
	int   fail   	= 0;
	int   reject 	= 0;
	float curAmp 	= 0;


	for (tries=0; tries<TRACKER::triesPerRejectionSampling; tries++) {

		curAmp = get_a_candidate_curve();

		if (curAmp < minFODamp) {
			reject++;
		} else if (curAmp > posteriorMax) {
			fail++;
			curAmp = -2;
			break;
		} else if ( doRandomThings->uniform_01()*posteriorMax < curAmp ) {
			previous_direction[0] = candidate_direction[0];
			previous_direction[1] = candidate_direction[1];
			previous_direction[2] = candidate_direction[2];
			break;
		}

	}

	if (tries==TRACKER::triesPerRejectionSampling) {
		curAmp = -1;
	} else {
		// Update information for streamline ONLY when it is a legitimate propagation
		// This just makes sure that the displayed report is not dominated by tons of
		// trials made when streamlines terminate due to low data support
		// Otherwise, it does not change the tracking algorithm
		streamline->sampling_prop_generated       	+= 1;
		streamline->sampling_prop_tries  			+= tries;
		streamline->sampling_prop_reject 			+= reject;
		if (curAmp==-2)
			streamline->sampling_prop_fail 			 = 1;
	}

	return curAmp;

}
