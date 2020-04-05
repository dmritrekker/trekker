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
    
    curve->getCurrentCurve(p,F);
    curve->prepInitProbePropagator();
    
    if (TRACKER::img_FOD->iseven) {
    
        curve->likelihood = curve->lastVal;
        
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
        
    } else {
        
        curve->likelihood = 0;
        
        float pn[3];
        float Tb[3];
        float Te[3];
        
        for (int q=0; q<(TRACKER::probeQuality-1); q++) {
            
            
            for (int i=0; i<3; i++) {
                pn[i]  = p[i] + curve->PP[0]*F[0][i] +  curve->PP[1]*F[1][i]  +  curve->PP[2]*F[2][i];
                T[i]   =        curve->PP[3]*F[0][i] +  curve->PP[4]*F[1][i]  +  curve->PP[5]*F[2][i];
                N2[i]  =        curve->PP[6]*F[0][i] +  curve->PP[7]*F[1][i]  +  curve->PP[8]*F[2][i];
                Tb[i]  = pn[i] - p[i];
            }
            normalize(T);
            cross(N1,N2,T);
            normalize(Tb);
            for (int i=0; i<3; i++) {
                Te[i]  = -Tb[i];
            }
            
            
            if (TRACKER::probeCount==1) {
                
                float link = (getFODamp(p,Tb) + getFODamp(pn,Te))/float(2.0);
                
                if ((TRACKER::checkWeakLinks==CHECKWEAKLINKS_ON) && (link < TRACKER::weakLinkThresh)) {
                    curve->likelihood  = 0;
                    return;
                } else {
                    curve->likelihood += link;
                }
                
            } else {
                
                for (float c=0; c<TRACKER::probeCount; c++) {
                    
                    float pp[3];
                    float ppn[3];
                    
                    for (int i=0; i<3; i++) {
                        pp[i]  = p[i]  + F[1][i]*TRACKER::probeRadius*std::cos(c*TRACKER::angularSeparation) + F[2][i]*TRACKER::probeRadius*std::sin(c*TRACKER::angularSeparation);
                        ppn[i] = pn[i] +   N1[i]*TRACKER::probeRadius*std::cos(c*TRACKER::angularSeparation) +   N2[i]*TRACKER::probeRadius*std::sin(c*TRACKER::angularSeparation);
                        Tb[i]  = ppn[i] - pp[i];
                    }
                    
                    normalize(Tb);
                    for (int i=0; i<3; i++) {
                        Te[i]  = -Tb[i];
                    }
                    
                    float link = (getFODamp(pp,Tb) + getFODamp(ppn,Te))/float(2.0);
                    
                    if ((TRACKER::checkWeakLinks==CHECKWEAKLINKS_ON) && (link < TRACKER::weakLinkThresh)) {
                        curve->likelihood  = 0;
                        return;
                    } else {
                        curve->likelihood += link;
                    }

                }
                
            }
            
            // Update F here
            if (q<(TRACKER::probeQuality-1)) {
                for (int i=0; i<3; i++) {
                       p[i] = pn[i];
                    F[0][i] =  T[i];
                    F[1][i] = N1[i];
                    F[2][i] = N2[i];
                }
            }
            
            curve->prepProbePropagator();
        }
        
        
        
    }

    curve->likelihood *= TRACKER::probeNormalizer;
    curve->likelihood  = std::pow(curve->likelihood,TRACKER::dataSupportExponent);

}


void TrackWith_PTT::calcInitDataSupport() {
    
    if (TRACKER::img_FOD->iseven) {
        
        // First part of the probe
        curve->likelihood        = 0.0;
        curve->getCurrentCurve(p,F);
        
        if (TRACKER::probeCount==1) {
            curve->likelihood        = getFODamp(p,F[0]);
        } else {
            
            for (float c=0; c<TRACKER::probeCount; c++) {
                        
                float pp[3];
                
                for (int i=0; i<3; i++) {
                    pp[i] = p[i] + F[1][i]*TRACKER::probeRadius*std::cos(c*TRACKER::angularSeparation) + F[2][i]*TRACKER::probeRadius*std::sin(c*TRACKER::angularSeparation);
                }
                
                float val                 = getFODamp(pp,F[0]);
                
                if ((TRACKER::checkWeakLinks==CHECKWEAKLINKS_ON) && (val < TRACKER::weakLinkThresh)) {
                    curve->likelihood  = 0;
                    return;
                } else {
                    curve->likelihood += val;
                }

            }
            
        }
        
        curve->initFirstVal_cand = curve->likelihood;
        curve->lastVal_cand      = curve->likelihood;
    }
    
    // initFirstVal_cand and lastVal_cand are not used in the asymmetric FOD case
    
    calcDataSupport();

}
