#include "../../tracker_thread.h"
#include "algorithm_ptt.h"

/*
void TrackWith_PTT::calcLikelihoodAndPosterior() {

    curve->likelihood  	= 0;

    float p[3];
    float T[3];
    
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
*/


/*
void TrackWith_PTT::calcLikelihoodAndPosterior() {

    curve->likelihood  	= 0;

    float p[3];
    float T[3];
    float N1[3];
    float N2[3];
    
    
    float F[3][3];
    for (int i=0; i<3; i++) {
        p[i] = curve->p[i];
        for (int j=0; j<3; j++) {
            F[i][j] = curve->F[i][j];
        }
    }
    
    for (int q=0; q<TRACKER::probeQuality; q++) {
        
        size_t ind = curve->candidate_index;
        
        for (int i=0; i<3; i++) {
            p[i]  += probe_step_consts[ind][0]*F[i][0] +  probe_step_consts[ind][1]*F[i][1]  +  probe_step_consts[ind][2]*F[i][2];
            T[i]   = probe_step_consts[ind][3]*F[i][0] +  probe_step_consts[ind][4]*F[i][1]  +  probe_step_consts[ind][5]*F[i][2];
        }
        
        
        if (q<(TRACKER::probeQuality-1)) {
            
            for (int i=0; i<3; i++) {
                N2[i]  = probe_step_consts[ind][6]*F[i][0] +  probe_step_consts[ind][7]*F[i][1]  +  probe_step_consts[ind][8]*F[i][2];
            }
            
            cross(N1,N2,T);
            for (int i=0; i<3; i++) {
                F[i][0] =  T[i];
                F[i][1] = N1[i];
                F[i][2] = N2[i];
            }
        }
        
        
        if (TRACKER::probeCount==1) {
            
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
            
            
        } else {
            
            
            if (q==(TRACKER::probeQuality-1)) {
                for (int i=0; i<3; i++) {
                    N2[i]  = probe_step_consts[ind][6]*F[i][0] +  probe_step_consts[ind][7]*F[i][1]  +  probe_step_consts[ind][8]*F[i][2];
                }
                cross(N1,N2,T);
            }
            
            
            for (float c=0; c<TRACKER::probeCount; c++) {
                
                float pp[3];
                
                for (int i=0; i<3; i++) {
                    pp[i] = p[i] + N1[i]*TRACKER::probeRadius*std::cos(c*angularSeparation) + N2[i]*TRACKER::probeRadius*std::sin(c*angularSeparation);
                }
                
                float val;
                if (TRACKER::fodDiscretization==FODDISC_OFF) {
                    thread->tracker_FOD->getVal(pp,FOD);
                    val = SH::SH_amplitude(FOD,T);
                } else
                    val = thread->tracker_FOD->getFODval(pp,T);

                
                if ((TRACKER::checkWeakLinks==CHECKWEAKLINKS_ON) && (val < minFODamp)) {
                    curve->likelihood   = 0;
                    curve->posterior    = 0;
                    return;
                } else
                    curve->likelihood += val;

            }   
            
        }
        
        
        
    }

    curve->likelihood *= probeNormalizer;
    curve->likelihood  = std::pow(curve->likelihood,TRACKER::dataSupportExponent);
    curve->posterior   = curve->likelihood*curve->prior;

}
*/


/*
void TrackWith_PTT::calcLikelihoodAndPosterior() {

    curve->likelihood  	= 0;
    
    for (int i=0; i<3; i++) {
        p[i] = curve->p[i];
        for (int j=0; j<3; j++) {
            F[i][j] = curve->F[i][j];
        }
    }
    
    for (int q=0; q<TRACKER::probeQuality; q++) {
            
        size_t ind = curve->getCandidateIndex(q);
        
        for (int i=0; i<3; i++) {
            p[i]  += probe_step_consts[ind][0]*F[i][0] +  probe_step_consts[ind][1]*F[i][1]  +  probe_step_consts[ind][2]*F[i][2];
            T[i]   = probe_step_consts[ind][3]*F[i][0] +  probe_step_consts[ind][4]*F[i][1]  +  probe_step_consts[ind][5]*F[i][2];
        }
        
        
        if (q<(TRACKER::probeQuality-1)) {
            
            for (int i=0; i<3; i++) {
                N2[i]  = probe_step_consts[ind][6]*F[i][0] +  probe_step_consts[ind][7]*F[i][1]  +  probe_step_consts[ind][8]*F[i][2];
            }
            
            cross(N1,N2,T);
            for (int i=0; i<3; i++) {
                F[i][0] =  T[i];
                F[i][1] = N1[i];
                F[i][2] = N2[i];
            }
            
        }
        
        
        if (TRACKER::probeCount==1) {
            
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
            
            
        } else {
            
            
            if (q==(TRACKER::probeQuality-1)) {
                for (int i=0; i<3; i++) {
                    N2[i]  = probe_step_consts[ind][6]*F[i][0] +  probe_step_consts[ind][7]*F[i][1]  +  probe_step_consts[ind][8]*F[i][2];
                }
                cross(N1,N2,T);
            }
            
            
            for (float c=0; c<TRACKER::probeCount; c++) {
                
                float pp[3];
                
                for (int i=0; i<3; i++) {
                    pp[i] = p[i] + N1[i]*TRACKER::probeRadius*std::cos(c*angularSeparation) + N2[i]*TRACKER::probeRadius*std::sin(c*angularSeparation);
                }
                
                float val;
                if (TRACKER::fodDiscretization==FODDISC_OFF) {
                    thread->tracker_FOD->getVal(pp,FOD);
                    val = SH::SH_amplitude(FOD,T);
                } else
                    val = thread->tracker_FOD->getFODval(pp,T);

                
                if ((TRACKER::checkWeakLinks==CHECKWEAKLINKS_ON) && (val < minFODamp)) {
                    curve->likelihood   = 0;
                    curve->posterior    = 0;
                    return;
                } else
                    curve->likelihood += val;

            }   
            
        }
        
        
        
        
    }

    curve->likelihood *= probeNormalizer;
    curve->likelihood  = std::pow(curve->likelihood,TRACKER::dataSupportExponent);
    curve->posterior   = curve->likelihood*curve->prior;

}*/

float TrackWith_PTT::getDataSupport(float *p, float *T) {
    
    if (TRACKER::fodDiscretization==FODDISC_OFF) {
        thread->tracker_FOD->getVal(p,FOD);
        return SH::SH_amplitude(FOD,T);
    } else
        return thread->tracker_FOD->getFODval(p,T);
    
}

void TrackWith_PTT::calcLikelihoodAndPosterior() {
    
    // Simulate step for candidate
    curve->prepCandStepPropagator();
    curve->walkCandStep(p,F);
    curve->likelihood = getDataSupport(p,F[0]);
    
    curve->prepProbePropagator();
    
    for (int q=0; q<TRACKER::probeQuality; q++) {
        
        for (int i=0; i<3; i++) {
            p[i]  += curve->PP[0]*F[0][i] +  curve->PP[1]*F[1][i]  +  curve->PP[2]*F[2][i];
            T[i]   = curve->PP[3]*F[0][i] +  curve->PP[4]*F[1][i]  +  curve->PP[5]*F[2][i];
        }
        
        
        if (q<(TRACKER::probeQuality-1)) {
            
            for (int i=0; i<3; i++) {
                N2[i]  = curve->PP[6]*F[0][i] +  curve->PP[7]*F[1][i]  +  curve->PP[8]*F[2][i];
            }
            
            cross(N1,N2,T);
            for (int i=0; i<3; i++) {
                F[0][i] =  T[i];
                F[1][i] = N1[i];
                F[2][i] = N2[i];
            }
            
        }
        
        
        if (TRACKER::probeCount==1) {
            
            float val = getDataSupport(p,T);
            
            if ((TRACKER::checkWeakLinks==CHECKWEAKLINKS_ON) && (val < minFODamp)) {
                curve->likelihood   = 0;
                curve->posterior    = 0;
                return;
            } else
                curve->likelihood += val;
            
            
        } else {
            
            
            if (q==(TRACKER::probeQuality-1)) {
                for (int i=0; i<3; i++) {
                    N2[i]  = curve->PP[6]*F[0][i] +  curve->PP[7]*F[1][i]  +  curve->PP[8]*F[2][i];
                }
                cross(N1,N2,T);
            }
            
            
            for (float c=0; c<TRACKER::probeCount; c++) {
                
                float pp[3];
                
                for (int i=0; i<3; i++) {
                    pp[i] = p[i] + N1[i]*TRACKER::probeRadius*std::cos(c*TRACKER::angularSeparation) + N2[i]*TRACKER::probeRadius*std::sin(c*TRACKER::angularSeparation);
                }
                
                float val = getDataSupport(p,T);
                
                if ((TRACKER::checkWeakLinks==CHECKWEAKLINKS_ON) && (val < minFODamp)) {
                    curve->likelihood   = 0;
                    curve->posterior    = 0;
                    return;
                } else
                    curve->likelihood += val;

            }   
            
        }
        
        
        
        
    }

    curve->likelihood *= TRACKER::probeNormalizer;
    curve->likelihood  = std::pow(curve->likelihood,TRACKER::dataSupportExponent);
    curve->posterior   = curve->likelihood*curve->prior;

}
