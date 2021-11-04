#include "algorithm_ptt.h"


void TrackWith_PTT::get_initial_curve() {

	if (SEED::seedingMode==SEED_COORDINATES_WITH_DIRECTIONS)
		curve->getARandomFrame(thread->seed_init_direction);
	else
		curve->getARandomFrame();
    
	curve->getInitCandidate();
    
}


Initialization_Decision TrackWith_PTT::initialize() {

	// Sample initial curve by rejection sampling
	int   tries;
	int   fail   	= 0;
	int   reject 	= 0;

    
	// Initial max estimate
	posteriorMax 	= 0;

	for (tries=0; tries < (int)current_init_postEstItCount; tries++) {

		get_initial_curve();
        
		if (curve->likelihood > posteriorMax) {
			posteriorMax = curve->likelihood;
			initial_curve->swap(curve);
		}
		
	}
    
	// initial compensation for underestimation
	posteriorMax        = std::pow(posteriorMax*DEFAULT_PTT_MAXPOSTESTCOMPENS,TRACKER::dataSupportExponent);
	if (GENERAL::verboseLevel > DETAILED) std::cout << "posteriorMax: " << posteriorMax << std::endl;

	if (TRACKER::atInit==ATINIT_USEBEST) {

		// Skip rejection sampling for initialization
		if (curve->likelihood < modMinFodAmp ) {
			curve->likelihood = -2;
            reject++;
        } else {
			if (GENERAL::verboseLevel > DETAILED) std::cout << "Initialization successful, likelihood was: " << curve->likelihood << std::endl;
		}

	} else {

		// Do rejection sampling for initialization
		for (tries=0; tries<TRACKER::triesPerRejectionSampling; tries++) {

			get_initial_curve();

			if (curve->likelihood < modMinFodAmp) {
				reject++;
			} else if (curve->likelihood > posteriorMax) {
				fail++;
				curve->likelihood = -2;
				break;
			} else if (doRandomThings->uniform_01()*posteriorMax <= curve->likelihood ) { // Equal helps to sample extrema
                // This candidate is now selected and it will be propagated
				initial_curve->swap(curve);
				if (GENERAL::verboseLevel > DETAILED) std::cout << "Initialization successful, likelihood was: " << curve->likelihood << std::endl;
				break;
			}

		}
		
        if (tries==TRACKER::triesPerRejectionSampling)
            return INIT_STOP;

	}
	
	// Update information for streamline ONLY when it is a valid initialization
    // This just makes sure that the displayed report is not dominated by tons of
    // trials made when streamlines does not initialize properly due to low data support
    // Otherwise, it does not change the tracking algorithm
    streamline->sampling_init_generated  		+= 1;
    streamline->sampling_init_tries  			+= tries;
    streamline->sampling_init_reject 			+= reject;
    if (curve->likelihood==-2) {
        streamline->sampling_init_fail 			 = 1;
        return INIT_FAIL;
    }
    return INIT_CONTINUE;


}

