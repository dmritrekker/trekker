#include "algorithm_ptt.h"

#include "../../tracker_thread.h"

TrackWith_PTT::TrackWith_PTT() {

	streamline  		= NULL;
	doRandomThings 		= NULL;
	initial_curve		= NULL;
	curve 				= NULL;

	if (TRACKER::defaultsSet == true) {
		doRandomThings 				= new RandomDoer();
		initial_curve				= new PTF(doRandomThings);
		curve 						= new PTF(doRandomThings);
        
		posteriorMax 				= 0.0f;
		current_init_postEstItCount = static_cast<Tractogram_PTT*>(TRACKER::tractogram)->init_postEstItCount;
		current_prop_postEstItCount = static_cast<Tractogram_PTT*>(TRACKER::tractogram)->prop_postEstItCount;   
	}
	
}


TrackWith_PTT::~TrackWith_PTT() {
	if (doRandomThings!=NULL) 	delete 	 doRandomThings;
	if (initial_curve!=NULL) 	delete 	 initial_curve;
	if (curve!=NULL) 			delete   curve;
}


void TrackWith_PTT::setSeed() {

	curve->setPosition(thread->seed_coordinates);
    
    // Flush probs
    curve->likelihood 	= 0.0;
	
	initial_curve->swap(curve);

	current_init_postEstItCount 	= static_cast<Tractogram_PTT*>(TRACKER::tractogram)->init_postEstItCount;
	current_prop_postEstItCount 	= static_cast<Tractogram_PTT*>(TRACKER::tractogram)->prop_postEstItCount;

	streamline->sampling_init_postEstItCount 	= current_init_postEstItCount;
	streamline->sampling_prop_postEstItCount	= current_prop_postEstItCount;

	// Update currMinFODamp
	if (TRACKER::useMinFODampImage) {
		float tmp;
		TRACKER::img_minFODamp->getVal(curve->p,&tmp);
		currMinFODamp = std::pow(tmp,TRACKER::dataSupportExponent);	
	} else {
		currMinFODamp = std::pow(minFODamp,TRACKER::dataSupportExponent);	
	}

}


void TrackWith_PTT::flip() {
	initial_curve->flip();
	curve->swap(initial_curve);

	// Update currMinFODamp
	if (TRACKER::useMinFODampImage) {
		float tmp;
		TRACKER::img_minFODamp->getVal(curve->p,&tmp);
		currMinFODamp = std::pow(tmp,TRACKER::dataSupportExponent);	
	} else {
		currMinFODamp = std::pow(minFODamp,TRACKER::dataSupportExponent);	
	}
}


void TrackWith_PTT::append() {
    this->streamline->push((void*)curve);    
}


void TrackWith_PTT::removeLast() {
	this->streamline->pop_back();
}
