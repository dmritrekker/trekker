#ifndef SRC_TRACKER_ALGORITHMS_PTT_TRACTOGRAM_PTT_H_
#define SRC_TRACKER_ALGORITHMS_PTT_TRACTOGRAM_PTT_H_

#include "../../base/tractogram.h"

#include "streamline_ptt.h"
#include "algorithm_ptt_defaults.h"

class Tractogram_PTT : public Tractogram {

public:

	Tractogram_PTT();
	virtual ~Tractogram_PTT() {};

	void writeOutput();
	void writeMetadataOutput();
	void printSummary();
	void update(TrackingThread *thread);


	size_t sampling_all_prop_postEstItCount;
	size_t sampling_all_prop_generated;
	size_t sampling_all_prop_tries;
	size_t sampling_all_prop_fail;
	size_t sampling_all_prop_reject;

	size_t prop_pooler;
	size_t prop_postEstItCount;
	size_t prop_numberOfRecentStreamlines;
	size_t prop_numberOfRecentFails;


	size_t sampling_all_init_postEstItCount;
	size_t sampling_all_init_generated;
	size_t sampling_all_init_tries;
	size_t sampling_all_init_fail;
	size_t sampling_all_init_reject;

	size_t init_postEstItCount;
	size_t init_numberOfRecentStreamlines;
	size_t init_numberOfRecentFails;

};

#endif
