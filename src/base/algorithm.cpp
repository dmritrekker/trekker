#include "algorithm.h"

TractographyAlgorithm::TractographyAlgorithm(){
	thread = NULL;
}

TractographyAlgorithm::~TractographyAlgorithm(){ }

void TractographyAlgorithm::setThread(TrackingThread* _thread) {
	thread = _thread;
}
