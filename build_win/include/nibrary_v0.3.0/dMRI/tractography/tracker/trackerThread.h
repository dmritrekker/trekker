#pragma once

#include "base/nibr.h"
#include "../pathway/pathway.h"

namespace NIBR {

class TractographyAlgorithm;

class TrackingThread {
public:

	TrackingThread(int _seed_no);
	~TrackingThread();

	int                     id;
	int 					threadId;

	TractographyAlgorithm  *method;
	Walker             	   *walker;
	std::vector<Point> 		streamline;
	
	float*             		seed_coordinates;
	float*             		seed_init_direction;

	void                    init();
	void                    reset();
	void                    clear();
	bool 		            track(); // returns true if tracking was successful

};

}
