#include "../../tracker_thread.h"
#include "algorithm_ptt_with_parameter_priors.h"

void TrackWith_PTT_with_parameter_priors::estimatePosteriorMax() {

	posteriorMax = 0;

	for (size_t it=0; it<current_prop_postEstItCount; it++) {
		curve->getCandidate();
		if (curve->likelihood > posteriorMax)
			posteriorMax = curve->likelihood;
	}

    posteriorMax *= posteriorMaxScalingFactor;

}
