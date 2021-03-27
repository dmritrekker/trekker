#include "algorithm_ptt_with_parameter_priors.h"


void TrackWith_PTT_with_parameter_priors::get_initial_curve() {

	if (SEED::seedingMode==SEED_COORDINATES_WITH_DIRECTIONS)
		curve->getARandomFrame(thread->seed_init_direction);
	else
		curve->getARandomFrame();
    
	curve->getInitCandidate();
    
}


Initialization_Decision TrackWith_PTT_with_parameter_priors::initialize() {


	// Sample initial curve by rejection sampling
	int   tries;
	int   fail   	= 0;
	int   reject 	= 0;

    
	// Initial max estimate
	posteriorMax 	= 0;

	for (tries=0; tries < (int)current_init_postEstItCount; tries++) {

		get_initial_curve();
        
		if (curve->likelihood > posteriorMax) {
            // This candidate is now selected and it will be propagated
			posteriorMax        = curve->likelihood;
            curve->initFirstVal = curve->initFirstVal_cand;
            curve->lastVal      = curve->lastVal_cand;
			initial_curve->swap(curve);
		}
		
	}
    
	posteriorMax       *= std::pow(DEFAULT_PTT_WITH_PARAMETER_PRIORS_MAXPOSTESTCOMPENS,TRACKER::dataSupportExponent); // initial compensation for underestimation
    initialPosteriorMax = posteriorMax;
    
    
	if (TRACKER::atInit==ATINIT_USEBEST) {

		// Skip rejection sampling for initialization
		if (curve->likelihood < modMinFodAmp ) {
			curve->likelihood = -2;
            reject++;
        } else {
            curve->swap(initial_curve);
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
			} else if (doRandomThings->uniform_01()*posteriorMax < curve->likelihood ) {
                // This candidate is now selected and it will be propagated
                curve->initFirstVal = curve->initFirstVal_cand;
                curve->lastVal      = curve->lastVal_cand;
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

