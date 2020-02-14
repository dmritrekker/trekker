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
    
    curve->likelihood = 0;
    curve->getCurrentCurve(p,F);
    curve->prepInitProbePropagator();
    
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
            
            float val = getFODamp(p,T);
            
            if ((TRACKER::checkWeakLinks==CHECKWEAKLINKS_ON) && (val < TRACKER::weakLinkThresh)) {
                curve->likelihood   = 0;
                return;
            } else {
                curve->likelihood += val;
            }
            
            
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
                
                float val = getFODamp(pp,T);
                
                if ((TRACKER::checkWeakLinks==CHECKWEAKLINKS_ON) && (val < TRACKER::weakLinkThresh)) {
                    curve->likelihood   = 0;
                    return;
                } else {
                    curve->likelihood += val;
                }

            }   
            
        }
        
        curve->prepProbePropagator();
        
    }

    curve->likelihood *= TRACKER::probeNormalizer;
    curve->likelihood  = std::pow(curve->likelihood,TRACKER::dataSupportExponent);

}
