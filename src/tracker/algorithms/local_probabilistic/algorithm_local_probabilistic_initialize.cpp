#include "../../tracker_thread.h"
#include "algorithm_local_probabilistic.h"

void TrackWith_Local_Probabilistic::get_initial_curve() {

	if (SEED::seedingMode==SEED_COORDINATES_WITH_DIRECTIONS) {
		candidate_direction[0] = thread->seed_init_direction.x;
		candidate_direction[1] = thread->seed_init_direction.y;
		candidate_direction[2] = thread->seed_init_direction.z;
	} else
		doRandomThings->getAUnitRandomVector(candidate_direction);

}

Initialization_Decision TrackWith_Local_Probabilistic::initialize() {

	posteriorMax 	= 0;
	thread->tracker_FOD->getVal(current_point,FOD);

	int   tries;
	int   fail   	= 0;
	int   reject 	= 0;
	float curAmp 	= 0;


	// Estimate posterior maximum
	for (tries=0; tries < (int)current_init_postEstItCount; tries++) {
		get_initial_curve();
		curAmp = SH::SH_amplitude(FOD, candidate_direction);
		if (curAmp > posteriorMax) {
			posteriorMax = curAmp;
			initial_direction[0] = previous_direction[0] = candidate_direction[0];
			initial_direction[1] = previous_direction[1] = candidate_direction[1];
			initial_direction[2] = previous_direction[2] = candidate_direction[2];
		}
	}

	curAmp 		  = posteriorMax;
	posteriorMax *= DEFAULT_LOCAL_PROBABILISTIC_MAXPOSTESTCOMPENS;

	if (TRACKER::atInit==ATINIT_USEBEST) {

		// Skip rejection sampling for initialization
		if (curAmp < minFODamp)
			curAmp = -2;

	} else {

		// Do rejection sampling for initialization
		for (tries=0; tries<TRACKER::triesPerRejectionSampling; tries++) {

			get_initial_curve();
			curAmp = SH::SH_amplitude(FOD, candidate_direction);

			if (curAmp < minFODamp) {
				reject++;
			} else if (curAmp > posteriorMax) {
				fail++;
				curAmp = -2;
				break;
			} else if (doRandomThings->uniform_01()*posteriorMax < curAmp ) {

				initial_direction[0] = previous_direction[0] = candidate_direction[0];
				initial_direction[1] = previous_direction[1] = candidate_direction[1];
				initial_direction[2] = previous_direction[2] = candidate_direction[2];

				if (GENERAL::verboseLevel > DETAILED) std::cout << "Initialization successful, posterior was: " << curAmp << std::endl;
				break;
			}

		}

	}


	if (tries==TRACKER::triesPerRejectionSampling) {
		return INIT_STOP;
	} else {
		// Update information for streamline ONLY when it is a valid initialization
		// This just makes sure that the displayed report is not dominated by tons of
		// trials made when streamlines doe not initialize properly due to low data support
		// Otherwise, it does not change the tracking algorithm
		streamline->sampling_init_generated  		+= 1;
		streamline->sampling_init_tries  			+= tries;
		streamline->sampling_init_reject 			+= reject;
		if (curAmp==-2) {
			streamline->sampling_init_fail 			 = 1;
			return INIT_FAIL;
		}
		return INIT_CONTINUE;
	}

}
