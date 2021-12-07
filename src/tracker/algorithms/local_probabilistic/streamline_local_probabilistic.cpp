#include "streamline_local_probabilistic.h"

Streamline_Local_Probabilistic::Streamline_Local_Probabilistic() {

	sampling_prop_postEstItCount 	= 0;
	sampling_prop_generated 		= 0;
	sampling_prop_fail 				= 0;
	sampling_prop_tries 			= 0;
	sampling_prop_reject 			= 0;

	sampling_init_postEstItCount 	= 0;
	sampling_init_generated 		= 0;
	sampling_init_fail 				= 0;
	sampling_init_tries 			= 0;
	sampling_init_reject 			= 0;

}

Streamline_Local_Probabilistic::~Streamline_Local_Probabilistic() { }

void Streamline_Local_Probabilistic::flush() {

	coordinates.clear();
	status 							= STREAMLINE_NOTSET;
	discardingReason 				= DIRCARDINGREASON_NOTSET;
	failingReason 					= FAILREASON_NOTSET;
	terminationReason_sideA 		= TERMINATIONREASON_NOTSET;
	terminationReason_sideB 		= TERMINATIONREASON_NOTSET;

	sampling_prop_postEstItCount 	= 0;
	sampling_prop_generated 		= 0;
	sampling_prop_fail 				= 0;
	sampling_prop_tries 			= 0;
	sampling_prop_reject 			= 0;

	sampling_init_postEstItCount 	= 0;
	sampling_init_generated 		= 0;
	sampling_init_fail 				= 0;
	sampling_init_tries 			= 0;
	sampling_init_reject 			= 0;

}

void Streamline_Local_Probabilistic::push(void *segment) {
	float *p = ((float *)segment);
	coordinates.push_back(Coordinate(p[0],p[1],p[2]));
}

void Streamline_Local_Probabilistic::pop_back() {
	coordinates.pop_back();
}


void Streamline_Local_Probabilistic::flip() {
	std::reverse(coordinates.begin(),coordinates.end());
}
