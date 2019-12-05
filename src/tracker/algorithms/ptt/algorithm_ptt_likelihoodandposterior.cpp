#include "../../tracker_thread.h"
#include "algorithm_ptt.h"

void TrackWith_PTT::calcLikelihoodAndPosterior() {

	curve->likelihood  	= 0;

	for (int q=(TRACKER::probeQuality-1); q>=0; q--) {
		for (int c=0; c<TRACKER::probeCount; c++) {

			float T[3];
			float p[3];

			size_t ind = c*PTF_CONSTS::numelkq + q*PTF_CONSTS::numelk  + curve->candidate_index;

			for (int i=0; i<3; i++) {
				// TODO: Computation of T multiple times is not necessary and can be removed
				T[i] =               PTF_CONSTS::probe_consts[ind][0]*curve->T[i] + PTF_CONSTS::probe_consts[ind][1]*curve->N1[i] + PTF_CONSTS::probe_consts[ind][2]*curve->N2[i];
				p[i] = curve->p[i] + PTF_CONSTS::probe_consts[ind][3]*curve->T[i] + PTF_CONSTS::probe_consts[ind][4]*curve->N1[i] + PTF_CONSTS::probe_consts[ind][5]*curve->N2[i];
			}

			normalize(T);
			thread->tracker_FOD->getVal(p,FOD);
			curve->likelihood += SH::SH_amplitude(FOD,T);

		}
	}

	curve->likelihood *= PTF_CONSTS::probeNormalizer;
    curve->likelihood  = std::pow(curve->likelihood,TRACKER::dataSupportExponent);
	curve->posterior   = curve->likelihood*curve->prior;

}
