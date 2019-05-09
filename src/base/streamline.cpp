#include "streamline.h"

Streamline::Streamline() {
	status 										= STREAMLINE_NOTSET;
	discardingReason 							= DIRCARDINGREASON_NOTSET;
	failingReason 								= FAILREASON_NOTSET;
	terminationReason_sideA 					= TERMINATIONREASON_NOTSET;
	terminationReason_sideB 					= TERMINATIONREASON_NOTSET;
	tracking_tries 								= 0;
	seed_coordinates 							= Coordinate();
}
