#include "../../tracker_thread.h"
#include "algorithm_local_probabilistic.h"

TrackWith_Local_Probabilistic::TrackWith_Local_Probabilistic() {

	streamline  		= NULL;
	doRandomThings 		= NULL;
	FOD 				= NULL;
	initial_point 		= NULL;
	initial_direction 	= NULL;
	current_point 		= NULL;
	previous_direction 	= NULL;
	candidate_direction = NULL;

	if (TRACKER::defaultsSet == true) {
		doRandomThings 						= new RandomDoer();

		FOD 								= new float[SH::numberOfSphericalHarmonicCoefficients];
		memset(FOD, 0, SH::numberOfSphericalHarmonicCoefficients*sizeof(float));

		posteriorMax 						= 0;

		initial_point 						= new float[3];
		initial_direction 					= new float[3];

		current_point 						= new float[3];
		previous_direction 					= new float[3];
		candidate_direction 				= new float[3];

		constant_asinterm 					= 2*std::asin (TRACKER::stepSize / (2.0 * TRACKER::minRadiusOfCurvature));

		current_init_postEstItCount 		= static_cast<Tractogram_Local_Probabilistic*>(TRACKER::tractogram)->init_postEstItCount;
		current_prop_postEstItCount 		= static_cast<Tractogram_Local_Probabilistic*>(TRACKER::tractogram)->prop_postEstItCount;
	}

}

TrackWith_Local_Probabilistic::~TrackWith_Local_Probabilistic() {

	if (doRandomThings!=NULL) 		delete 	 doRandomThings;
	if (FOD!=NULL) 					delete[] FOD;
	if (initial_point!=NULL) 		delete[] initial_point;
	if (initial_direction!=NULL) 	delete[] initial_direction;
	if (current_point!=NULL) 		delete[] current_point;
	if (previous_direction!=NULL) 	delete[] previous_direction;
	if (candidate_direction!=NULL) 	delete[] candidate_direction;

}

void TrackWith_Local_Probabilistic::setSeed() {

	initial_point[0] 		= thread->seed_coordinates.x;
	initial_point[1] 		= thread->seed_coordinates.y;
	initial_point[2] 		= thread->seed_coordinates.z;

	current_point[0] 		= thread->seed_coordinates.x;
	current_point[1] 		= thread->seed_coordinates.y;
	current_point[2] 		= thread->seed_coordinates.z;

	current_init_postEstItCount 	= static_cast<Tractogram_Local_Probabilistic*>(TRACKER::tractogram)->init_postEstItCount;
	current_prop_postEstItCount 	= static_cast<Tractogram_Local_Probabilistic*>(TRACKER::tractogram)->prop_postEstItCount;

	streamline->sampling_init_postEstItCount = current_init_postEstItCount;
	streamline->sampling_prop_postEstItCount = current_prop_postEstItCount;

	if (TRACKER::useMinFODampImage) TRACKER::img_minFODamp->getVal(current_point,&currMinFODamp);

}


void TrackWith_Local_Probabilistic::flip() {

	for (int i=0; i<3; i++) {
		initial_direction[i] *= -1;
		current_point[i]  	  = initial_point[i];
		previous_direction[i] = initial_direction[i];
	}

	if (TRACKER::useMinFODampImage) TRACKER::img_minFODamp->getVal(current_point,&currMinFODamp);

}


void TrackWith_Local_Probabilistic::append() {
	this->streamline->push(((void *)current_point));
}

void TrackWith_Local_Probabilistic::removeLast() {
	this->streamline->pop_back();
}
