#include "algorithm_ptt.h"

#include "../../tracker_thread.h"

TrackWith_PTT::TrackWith_PTT() {

	streamline  		= NULL;
	doRandomThings 		= NULL;
	initial_curve		= NULL;
	curve 				= NULL;
	FOD 				= NULL;
    p                   = NULL;
    F                   = NULL;

	if (TRACKER::defaultsSet == true) {
		doRandomThings 				= new RandomDoer();
		initial_curve				= new PTF(doRandomThings);
		curve 						= new PTF(doRandomThings);
		FOD 						= new float[SH::numberOfSphericalHarmonicCoefficients];
		posteriorMax 				= 0.0;

		current_init_postEstItCount = static_cast<Tractogram_PTT*>(TRACKER::tractogram)->init_postEstItCount;
		current_prop_postEstItCount = static_cast<Tractogram_PTT*>(TRACKER::tractogram)->prop_postEstItCount;
        
        
        p           = new float[3];
        F           = new float*[3];
        F[0]        = new float[3];
        F[1]        = new float[3];
        F[2]        = new float[3];

	}
	
}

TrackWith_PTT::~TrackWith_PTT() {
	if (doRandomThings!=NULL) 	delete 	 doRandomThings;
	if (initial_curve!=NULL) 	delete 	 initial_curve;
	if (curve!=NULL) 			delete   curve;
	if (FOD!=NULL) 				delete[] FOD;
    
    if (F!=NULL) {
        delete[] F[0];
        delete[] F[1];
        delete[] F[2];
        delete[] F;
    }
    
    if (p!=NULL) 				delete[] p;
    
}


void TrackWith_PTT::setSeed() {

	curve->setPosition(thread->seed_coordinates);
    
    // Flush probs
    curve->likelihood 	= 0.0;
	curve->prior 		= 1.0;
	curve->posterior 	=-1.0; // This is used to check if the curve is swapped with a candidate curve
	
	initial_curve->swap(curve);

	current_init_postEstItCount 	= static_cast<Tractogram_PTT*>(TRACKER::tractogram)->init_postEstItCount;
	current_prop_postEstItCount 	= static_cast<Tractogram_PTT*>(TRACKER::tractogram)->prop_postEstItCount;

	streamline->sampling_init_postEstItCount 	= current_init_postEstItCount;
	streamline->sampling_prop_postEstItCount	= current_prop_postEstItCount;

}


void TrackWith_PTT::flip() {

	initial_curve->flip();
	curve->swap(initial_curve);
    posteriorMax = initialPosteriorMax;

}


void TrackWith_PTT::append() {
	this->streamline->push((void*)curve);
}

void TrackWith_PTT::removeLast() {
	this->streamline->pop_back();
}
