#ifndef SRC_STREAMLINE_H_
#define SRC_STREAMLINE_H_

#include <iostream>
#include <vector>
#include <algorithm> // Used to reverse the vector for two-way tracking
#include "../config/config_output.h"
#include "../config/config_tracker.h"
#include "../math/coordinate.h"
#include "../math/doRandomThings.h"

typedef enum {
	STREAMLINE_NOTSET = -1,
	STREAMLINE_GOOD,
	STREAMLINE_DISCARDED,
	STREAMLINE_FAIL
} StreamlineStatus;

typedef enum {
	DIRCARDINGREASON_NOTSET,
	TOO_SHORT,
	TOO_LONG,
	DISCARD_ROI_REACHED,
	REQUIRED_ROI_NOT_MET,
	REQUIRED_ROI_ORDER_NOT_MET,
	ENDED_INSIDE_DISCARD_ROI,
    REENTERED_SEED_ROI,
	REACHED_TIME_LIMIT
} StreamlineDiscardingReason;

typedef enum {
	FAILREASON_NOTSET,
	REACHED_INITIALIZATION_TRIAL_LIMIT,
	DISCARDED_BY_THE_ALGORITHM_DURING_INITIALIZATION,
	DISCARDED_BY_THE_ALGORITHM
} StreamlineFailingReason;


typedef enum {
	TERMINATIONREASON_NOTSET,
	MIN_DATASUPPORT_REACHED,
	MAX_LENGTH_REACHED,
	STOP_ROI_REACHED
} StreamlineTerminationReason;

class Streamline {

public:
	Streamline();
	virtual ~Streamline() {};
	float getLength(){return ((float)coordinates.size()-1)*TRACKER::writeStepSize;};
	virtual void flush() 		= 0;
	virtual void push(void *) 	= 0;
	virtual void flip() 		= 0;

	Coordinate              	seed_coordinates;
	std::vector<Coordinate> 	coordinates;

	StreamlineDiscardingReason 	discardingReason;
	StreamlineFailingReason 	failingReason;
	StreamlineTerminationReason terminationReason_sideA;
	StreamlineTerminationReason terminationReason_sideB;

	StreamlineStatus status;

	size_t tracking_tries;

};

#endif
