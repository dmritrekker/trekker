#include "../../tracker_thread.h"
#include "algorithm_local_probabilistic.h"

void TrackWith_Local_Probabilistic::estimatePosteriorMax() {

	posteriorMax 		= 0;

	for (size_t it=0; it < current_prop_postEstItCount; it++) {
		float curAmp = get_a_candidate_curve();
		if (curAmp > posteriorMax)
			posteriorMax = curAmp;
	}
	posteriorMax *= DEFAULT_LOCAL_PROBABILISTIC_MAXPOSTESTCOMPENS;

}
