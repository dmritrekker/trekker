#include "algorithm_ptt.h"

#include "../../tracker_thread.h"

TrackWith_PTT::TrackWith_PTT() {

	streamline  		= NULL;
	doRandomThings 		= NULL;
	initial_curve		= NULL;
	curve 				= NULL;
	FOD 				= NULL;

	if (TRACKER::defaultsSet == true) {
		doRandomThings 				= new RandomDoer();
		initial_curve				= new FPTF(doRandomThings);
		curve 						= new FPTF(doRandomThings);
		FOD 						= new float[SH::numberOfSphericalHarmonicCoefficients];
		memset(FOD, 0, SH::numberOfSphericalHarmonicCoefficients*sizeof(float));
		posteriorMax 				= 0.0;

		current_init_postEstItCount = static_cast<Tractogram_PTT*>(TRACKER::tractogram)->init_postEstItCount;
		current_prop_postEstItCount = static_cast<Tractogram_PTT*>(TRACKER::tractogram)->prop_postEstItCount;

		doRandomThings->init_uniform_int(PTF_CONSTS::validIndexCount-1);
	}

}

TrackWith_PTT::~TrackWith_PTT() {
	if (doRandomThings!=NULL) 	delete 	 doRandomThings;
	if (initial_curve!=NULL) 	delete 	 initial_curve;
	if (curve!=NULL) 			delete   curve;
	if (FOD!=NULL) 				delete[] FOD;
}


void TrackWith_PTT::setSeed() {

	curve->setPosition(thread->seed_coordinates);
	curve->flushProbs();
	initial_curve->swap(curve);

	current_init_postEstItCount 	= static_cast<Tractogram_PTT*>(TRACKER::tractogram)->init_postEstItCount;
	current_prop_postEstItCount 	= static_cast<Tractogram_PTT*>(TRACKER::tractogram)->prop_postEstItCount;

	streamline->sampling_init_postEstItCount 	= current_init_postEstItCount;
	streamline->sampling_prop_postEstItCount	= current_prop_postEstItCount;

}


void TrackWith_PTT::flip() {

	initial_curve->flip();
	curve->swap(initial_curve);

}


void TrackWith_PTT::append() {
	this->streamline->push((void*)curve);
}

void TrackWith_PTT::removeLast() {
	this->streamline->pop_back();
}
