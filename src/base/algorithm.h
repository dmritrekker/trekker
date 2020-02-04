#ifndef SRC_TRACKER_ALGORITHM_H_
#define SRC_TRACKER_ALGORITHM_H_

#include "../math/coordinate.h"
#include "tractogram.h"

typedef enum {
	CONTINUE,
	STOP,
	FAIL
} Propagation_Decision;

typedef enum {
	INIT_CONTINUE,
	INIT_STOP,
	INIT_FAIL
}
Initialization_Decision;

class TrackingThread;

class TractographyAlgorithm {

public:
	TractographyAlgorithm();
	virtual ~TractographyAlgorithm() 	= 0;

	virtual Initialization_Decision initialize() 			= 0;
	virtual Propagation_Decision    propagate(int count) 	= 0;

	virtual void setSeed() 				= 0;
	virtual void flip() 				= 0; // used before tracking the other side
	virtual void append() 				= 0; // adds the new element
	virtual void removeLast() 			= 0; // removes the last element

	void 		 setThread(TrackingThread* _thread);
	virtual void setStreamline(Streamline *_streamline) 	= 0;
    
	virtual void setDefaultParametersWhenNecessary() = 0;
	virtual void print() = 0;

	TrackingThread* thread;

};



#endif
