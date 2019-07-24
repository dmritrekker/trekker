#include "algorithm_ptt.h"


void TrackWith_PTT::get_initial_curve() {

	if (SEED::seedingMode==SEED_COORDINATES_WITH_DIRECTIONS)
		curve->getARandomFrame(thread->seed_init_direction);
	else
		curve->getARandomFrame();

	curve->setCandidateIndex(PTF_CONSTS::valid_indices.at(doRandomThings->uniform_int()));
	calcLikelihoodAndPosterior();
}


Initialization_Decision TrackWith_PTT::initialize() {


	// Sample initial curve by rejection sampling
	curve->prior 	= 1;

	int   tries;
	int   fail   	= 0;
	int   reject 	= 0;

	// Initial estimation of posterior max
	posteriorMax 	= 0;

	for (tries=0; tries < (int)current_init_postEstItCount; tries++) {

		get_initial_curve();
		if (curve->posterior > posteriorMax) {
			posteriorMax = curve->posterior;
			curve->setToCandidate();
			initial_curve->swap(curve);
		}
	}
	posteriorMax *= DEFAULT_PTT_MAXPOSTESTCOMPENS; // initial compensation for underestimation

	if (TRACKER::atInit==ATINIT_USEBEST) {

		// Skip rejection sampling for initialization
		curve->swap(initial_curve);
		if (curve->posterior < minFODamp)
			curve->posterior = -2;

	} else {

		// Do rejection sampling for initialization
		for (tries=0; tries<TRACKER::triesPerRejectionSampling; tries++) {

			get_initial_curve();

			if (curve->posterior < minFODamp) {
				reject++;
			} else if (curve->posterior > posteriorMax) {
				fail++;
				curve->posterior = -2;
				break;
			} else if (doRandomThings->uniform_01()*posteriorMax < curve->posterior ) {
				curve->setToCandidate();
				initial_curve->swap(curve);
				if (GENERAL::verboseLevel > DETAILED) std::cout << "Initialization successful, posterior was: " << curve->posterior << std::endl;
				break;
			}

		}

	}


	if (tries==TRACKER::triesPerRejectionSampling) {
		return INIT_STOP;
	} else {
		// Update information for streamline ONLY when it is a valid initialization
		// This just makes sure that the displayed report is not dominated by tons of
		// trials made when streamlines does not initialize properly due to low data support
		// Otherwise, it does not change the tracking algorithm
		streamline->sampling_init_generated  		+= 1;
		streamline->sampling_init_tries  			+= tries;
		streamline->sampling_init_reject 			+= reject;
		if (curve->posterior==-2) {
			streamline->sampling_init_fail 			 = 1;
			return INIT_FAIL;
		}
		return INIT_CONTINUE;
	}


}

