#include "../../tracker_thread.h"
#include "algorithm_ptt.h"

void TrackWith_PTT::estimatePosteriorMax() {

	posteriorMax = 0;

	for (size_t it=0; it<current_prop_postEstItCount; it++) {
		get_a_candidate_curve();
		if (curve->likelihood > posteriorMax)
			posteriorMax = curve->likelihood;
	}

    posteriorMax *= std::pow(DEFAULT_PTT_MAXPOSTESTCOMPENS,TRACKER::dataSupportExponent);

}
