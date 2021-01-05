#include "algorithm_ptt_with_parameter_priors.h"

#include "../../tracker_thread.h"

TrackWith_PTT_with_parameter_priors::TrackWith_PTT_with_parameter_priors() {

	streamline  		= NULL;
	doRandomThings 		= NULL;
	initial_curve		= NULL;
	curve 				= NULL;

	if (TRACKER::defaultsSet == true) {
		doRandomThings 				= new RandomDoer();
		initial_curve				= new PTF_with_parameter_priors(doRandomThings);
		curve 						= new PTF_with_parameter_priors(doRandomThings);
        
		posteriorMax 				= 0.0;
        posteriorMaxScalingFactor   = std::pow(DEFAULT_PTT_WITH_PARAMETER_PRIORS_MAXPOSTESTCOMPENS,TRACKER::dataSupportExponent);

		current_init_postEstItCount = static_cast<Tractogram_PTT_with_parameter_priors*>(TRACKER::tractogram)->init_postEstItCount;
		current_prop_postEstItCount = static_cast<Tractogram_PTT_with_parameter_priors*>(TRACKER::tractogram)->prop_postEstItCount;   
	}
	
}


TrackWith_PTT_with_parameter_priors::~TrackWith_PTT_with_parameter_priors() {
	if (doRandomThings!=NULL) 	delete 	 doRandomThings;
	if (initial_curve!=NULL) 	delete 	 initial_curve;
	if (curve!=NULL) 			delete   curve;
}


void TrackWith_PTT_with_parameter_priors::setSeed() {

	curve->setPosition(thread->seed_coordinates);
    
    // Flush probs
    curve->likelihood 	= 0.0;
	
	initial_curve->swap(curve);

	current_init_postEstItCount 	= static_cast<Tractogram_PTT_with_parameter_priors*>(TRACKER::tractogram)->init_postEstItCount;
	current_prop_postEstItCount 	= static_cast<Tractogram_PTT_with_parameter_priors*>(TRACKER::tractogram)->prop_postEstItCount;

	streamline->sampling_init_postEstItCount 	= current_init_postEstItCount;
	streamline->sampling_prop_postEstItCount	= current_prop_postEstItCount;

}


void TrackWith_PTT_with_parameter_priors::flip() {

	initial_curve->flip();
	curve->swap(initial_curve);
    posteriorMax = initialPosteriorMax;

}


void TrackWith_PTT_with_parameter_priors::append() {
	
    this->streamline->push((void*)curve);
    
    
    /*
    if (this->streamline->coordinates.size()==1) {
        
    curve->prepInitProbePropagator();
    
    for (int i=0; i<3; i++) {
        curve->_p[i] 	= curve->p[i];
        for (int j=0; j<3; j++) {
            curve->_F[i][j] = curve->F[i][j];
        }
	}
    
    for (int q=0; q<(TRACKER::probeQuality-1); q++) {
            
            
        for (int i=0; i<3; i++) {
            curve->_p[i]  += curve->PP[0]*curve->_F[0][i] +  curve->PP[1]*curve->_F[1][i]  +  curve->PP[2]*curve->_F[2][i];
            curve->_T[i]   = curve->PP[3]*curve->_F[0][i] +  curve->PP[4]*curve->_F[1][i]  +  curve->PP[5]*curve->_F[2][i];
        }
        normalize(curve->_T);
        
        for (int i=0; i<3; i++) {
            curve->_N2[i]  = curve->PP[6]*curve->_F[0][i] +  curve->PP[7]*curve->_F[1][i]  +  curve->PP[8]*curve->_F[2][i];
        }
        
        cross(curve->_N1,curve->_N2,curve->_T);
        for (int i=0; i<3; i++) {
            curve->_F[0][i] = curve->_T[i];
            curve->_F[1][i] = curve->_N1[i];
            curve->_F[2][i] = curve->_N2[i];
        }
            
        
        
        
        if (TRACKER::probeCount>1) {
            
            if (q==(TRACKER::probeQuality-1)) {
                for (int i=0; i<3; i++) {
                    curve->_N2[i]  = curve->PP[6]*curve->_F[0][i] +  curve->PP[7]*curve->_F[1][i]  +  curve->PP[8]*curve->_F[2][i];
                }
                cross(curve->_N1,curve->_N2,curve->_T);
            }
            
            
            for (float c=0; c<TRACKER::probeCount; c++) {
                
                float pp[3];
                
                for (int i=0; i<3; i++) {
                    pp[i] = curve->_p[i] + curve->_N1[i]*TRACKER::probeRadius*std::cos(c*TRACKER::angularSeparation) + curve->_N2[i]*TRACKER::probeRadius*std::sin(c*TRACKER::angularSeparation);
                }

                PTF_with_parameter_priors *tmp = new PTF_with_parameter_priors(new RandomDoer());
                tmp->swap(curve);
                tmp->p[0] = pp[0];
                tmp->p[1] = pp[1];
                tmp->p[2] = pp[2];
                this->streamline->push((void*)tmp);
                delete tmp;
                

            } 
            
        }
        
        curve->prepProbePropagator();
    }
    
    }
    */
    
}


void TrackWith_PTT_with_parameter_priors::removeLast() {
	this->streamline->pop_back();
}
