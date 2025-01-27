#pragma once

#include "base/nibr.h"
#include "../tracker/trackerThread.h"

namespace NIBR {

typedef enum {
	PROP_CONTINUE,
	PROP_STOP,
	PROP_FAIL
} Propagation_Decision;

typedef enum {
	ALGORITHM_NOTSET = -1,
	PTT,
	LOCAL_PROBABILISTIC
} Algorithm_Type;

class TractographyAlgorithm {

public:
	TractographyAlgorithm() {thread = NULL;}
	virtual ~TractographyAlgorithm() {}

	virtual Propagation_Decision 	initialize() 		 = 0;
	virtual Propagation_Decision 	propagate() 		 = 0;
	virtual Propagation_Decision 	flip() 				 = 0; // used before tracking the other side. Output tells whether propagation toward the other side is allowed or not.

	virtual void  setSeed() 		 = 0;
	virtual void  reset() 			 = 0; // resets the method except for the seed
	virtual void  append() 			 = 0; // adds the new element
	virtual float writeStepSize()    = 0; // returns the writeStepSize
	virtual int   appendInterval()   = 0; // returns the interval for appending a new point

	void setThread(TrackingThread* _thread) {thread = _thread;}
	TrackingThread* thread;

};

}