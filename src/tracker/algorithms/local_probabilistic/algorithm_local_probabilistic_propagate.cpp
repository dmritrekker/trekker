#include "../../tracker_thread.h"
#include "../../../math/math_aux.h"
#include "algorithm_local_probabilistic.h"

float TrackWith_Local_Probabilistic::get_a_candidate_curve() {

	float v[3];

	do {
		v[0] = doRandomThings->uniform_m1_p1();
		v[1] = doRandomThings->uniform_m1_p1();
	} while (v[0]*v[0] + v[1]*v[1] > 1);

	v[0] *= constant_asinterm;
	v[1] *= constant_asinterm;

	v[2] = 1.0 - (v[0]*v[0] + v[1]*v[1]);
	v[2] = v[2] < 0.0 ? 0.0 : std::sqrt (v[2]);

	if (previous_direction[0]*previous_direction[0] + previous_direction[1]*previous_direction[1] < 1e-4) {

		candidate_direction[0] = v[0];
		candidate_direction[1] = v[1];
		candidate_direction[2] = previous_direction[2] > 0.0 ? v[2] : -v[2];

	} else {

		float y[]        = { previous_direction[0], previous_direction[1], 0.0 };
		normalize(y);


		float x[]        = { -y[1], y[0], 0.0 };
		float y2[]       = { -x[1]*previous_direction[2], x[0]*previous_direction[2], x[1]*previous_direction[0] - x[0]*previous_direction[1] };
		normalize(y2);

		float cx         = v[0]*x[0] + v[1]*x[1];
		float cy         = v[0]*y[0] + v[1]*y[1];

		candidate_direction[0]      	= cx*x[0] + cy*y2[0] + v[2]*previous_direction[0];
		candidate_direction[1]      	= cx*x[1] + cy*y2[1] + v[2]*previous_direction[1];
		candidate_direction[2]      	= cy*y2[2] + v[2]*previous_direction[2];

	}

    if (TRACKER::fodDiscretization==FODDISC_OFF) {
        return SH::SH_amplitude(FOD,candidate_direction);
    } else
        return thread->tracker_FOD->getFODval(current_point,candidate_direction);

}

Propagation_Decision TrackWith_Local_Probabilistic::propagate(int ) {

	for (int i=0; i<3; i++)
		current_point[i] += TRACKER::stepSize*previous_direction[i];
	
    if (TRACKER::fodDiscretization==FODDISC_OFF) {
        thread->tracker_FOD->getVal(current_point,FOD);
    }

	// Estimate posterior maximum
	estimatePosteriorMax();

	// Rejection sample
	float samplingOutput = rejectionSample();

	if (samplingOutput==-2) {
		return FAIL;
	} else if (samplingOutput < TRACKER::minFODamp) {
		return STOP;
	} else
		return CONTINUE;

}
