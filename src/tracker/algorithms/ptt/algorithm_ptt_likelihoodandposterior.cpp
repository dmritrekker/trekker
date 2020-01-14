#include "../../tracker_thread.h"
#include "algorithm_ptt.h"

void TrackWith_PTT::calcLikelihoodAndPosterior() {

	curve->likelihood  	= 0;

    float T[3];
    float p[3];
    
	for (int q=(TRACKER::probeQuality-1); q>=0; q--) {
            
        size_t indQ = q*numelk  + curve->candidate_index;
                
        T[0] = probe_consts[indQ][0]*curve->F[0][0] + probe_consts[indQ][1]*curve->F[0][1] + probe_consts[indQ][2]*curve->F[0][2];
        T[1] = probe_consts[indQ][0]*curve->F[1][0] + probe_consts[indQ][1]*curve->F[1][1] + probe_consts[indQ][2]*curve->F[1][2];
        T[2] = probe_consts[indQ][0]*curve->F[2][0] + probe_consts[indQ][1]*curve->F[2][1] + probe_consts[indQ][2]*curve->F[2][2];
        
		for (int c=0; c<TRACKER::probeCount; c++) {

			size_t indC = c*numelkq + indQ;
            
            p[0] = curve->p[0] + probe_consts[indC][3]*curve->F[0][0] + probe_consts[indC][4]*curve->F[0][1] + probe_consts[indC][5]*curve->F[0][2];
            p[1] = curve->p[1] + probe_consts[indC][3]*curve->F[1][0] + probe_consts[indC][4]*curve->F[1][1] + probe_consts[indC][5]*curve->F[1][2];
            p[2] = curve->p[2] + probe_consts[indC][3]*curve->F[2][0] + probe_consts[indC][4]*curve->F[2][1] + probe_consts[indC][5]*curve->F[2][2];
            
            float val;
            if (TRACKER::fodDiscretization==FODDISC_OFF) {
                thread->tracker_FOD->getVal(p,FOD);
                val = SH::SH_amplitude(FOD,T);
            } else
                val = thread->tracker_FOD->getFODval(p,T);

            
            if ((TRACKER::checkWeakLinks==CHECKWEAKLINKS_ON) && (val < minFODamp)) {
                curve->likelihood   = 0;
                curve->posterior    = 0;
                return;
            } else
                curve->likelihood += val;

		}
		
	}

	curve->likelihood *= probeNormalizer;
    curve->likelihood  = std::pow(curve->likelihood,TRACKER::dataSupportExponent);
	curve->posterior   = curve->likelihood*curve->prior;

}

