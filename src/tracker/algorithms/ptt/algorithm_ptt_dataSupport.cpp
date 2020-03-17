#include "../../tracker_thread.h"
#include "algorithm_ptt.h"

float TrackWith_PTT::getFODamp(float *pv, float *Tv) {
    
    if (TRACKER::fodDiscretization==FODDISC_OFF) {
        thread->tracker_FOD->getVal(pv,FOD);
        return SH::SH_amplitude(FOD,Tv);
    } else
        return thread->tracker_FOD->getFODval(pv,Tv);
    
}

void TrackWith_PTT::calcDataSupport() {
    
    curve->likelihood = curve->lastVal;
    curve->getCurrentCurve(p,F);
    curve->prepInitProbePropagator();
    
    for (int q=0; q<(TRACKER::probeQuality-1); q++) {
        
        
        for (int i=0; i<3; i++) {
            p[i]  += curve->PP[0]*F[0][i] +  curve->PP[1]*F[1][i]  +  curve->PP[2]*F[2][i];
            T[i]   = curve->PP[3]*F[0][i] +  curve->PP[4]*F[1][i]  +  curve->PP[5]*F[2][i];
        }
        normalize(T);
        
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
            
            curve->lastVal_cand = getFODamp(p,T);
            
            if ((TRACKER::checkWeakLinks==CHECKWEAKLINKS_ON) && (curve->lastVal_cand < TRACKER::weakLinkThresh)) {
                curve->likelihood  = 0;
                return;
            } else {
                curve->likelihood += curve->lastVal_cand;
            }
            
            
        } else {
            
            curve->lastVal_cand = 0;
            
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
                
                float val = getFODamp(pp,T);
                
                if ((TRACKER::checkWeakLinks==CHECKWEAKLINKS_ON) && (val < TRACKER::weakLinkThresh)) {
                    curve->likelihood    = 0;
                    return;
                } else {
                    curve->lastVal_cand += val;
                }

            } 
            
            curve->likelihood += curve->lastVal_cand;
            
        }
        
        curve->prepProbePropagator();
        
    }

    curve->likelihood *= TRACKER::probeNormalizer;
    curve->likelihood  = std::pow(curve->likelihood,TRACKER::dataSupportExponent);

}


// Need the data support for q=0 of the probe for initialization
// This is used after flipping the curve for tracking the other side
// In order to take into account asymmetric FODs,
// what needs to be saved is FOD amplitude along -T instead T
// FOD amp along T is not needed to be saved
// but is used to track the currect side
void TrackWith_PTT::calcInitDataSupport() {
    
    // Frist part of the probe
    curve->likelihood        = 0.0;
    curve->getCurrentCurve(p,F);
    
    // Save the first part for later flipping, N1 and N2 don't need to be changed
    curve->initFirstVal_cand = 0.0;
    float T_inv[3];
    T_inv[0] = -F[0][0];
    T_inv[1] = -F[0][1];
    T_inv[2] = -F[0][2];
    
    if (TRACKER::probeCount==1) {
        curve->likelihood        = getFODamp(p,F[0]);
        curve->initFirstVal_cand = getFODamp(p,T_inv);
    } else {
        
        for (float c=0; c<TRACKER::probeCount; c++) {
                    
            float pp[3];
            
            for (int i=0; i<3; i++) {
                pp[i] = p[i] + F[1][i]*TRACKER::probeRadius*std::cos(c*TRACKER::angularSeparation) + F[2][i]*TRACKER::probeRadius*std::sin(c*TRACKER::angularSeparation);
            }
            
            float val                 = getFODamp(pp,F[0]);
            curve->initFirstVal_cand += getFODamp(pp,T_inv);
            
            if ((TRACKER::checkWeakLinks==CHECKWEAKLINKS_ON) && (val < TRACKER::weakLinkThresh)) {
                curve->likelihood  = 0;
                return;
            } else {
                curve->likelihood += val;
            }

        }
        
    }
    curve->lastVal_cand = curve->likelihood;
    
    // Rest of the probe
    curve->prepInitProbePropagator();
    
    for (int q=0; q<TRACKER::probeQuality; q++) {
        
        
        for (int i=0; i<3; i++) {
            p[i]  += curve->PP[0]*F[0][i] +  curve->PP[1]*F[1][i]  +  curve->PP[2]*F[2][i];
            T[i]   = curve->PP[3]*F[0][i] +  curve->PP[4]*F[1][i]  +  curve->PP[5]*F[2][i];
        }
        normalize(T);
        
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
            
            curve->lastVal_cand = getFODamp(p,T);
            
            if ((TRACKER::checkWeakLinks==CHECKWEAKLINKS_ON) && (curve->lastVal_cand < TRACKER::weakLinkThresh)) {
                curve->likelihood  = 0;
                return;
            } else {
                curve->likelihood += curve->lastVal_cand;
            }
            
        } else {
            
            curve->lastVal_cand = 0;
            
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
                
                float val = getFODamp(pp,T);
                
                if ((TRACKER::checkWeakLinks==CHECKWEAKLINKS_ON) && (val < TRACKER::weakLinkThresh)) {
                    curve->likelihood    = 0;
                    return;
                } else {
                    curve->lastVal_cand += val;
                }

            } 
            
            curve->likelihood += curve->lastVal_cand;
            
        }
        
        curve->prepProbePropagator();
        
    }

    curve->likelihood *= TRACKER::probeNormalizer;
    curve->likelihood  = std::pow(curve->likelihood,TRACKER::dataSupportExponent);

}
