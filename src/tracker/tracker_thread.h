#ifndef SRC_TRACKER_TRACKER_THREAD_H_
#define SRC_TRACKER_TRACKER_THREAD_H_

#include "../base/inputParser.h"
#include "../base/tractogram.h"

using namespace GENERAL;
using namespace TRACKER;
using namespace SEED;
using namespace PATHWAY;

typedef enum {
	CONTINUE_TRACKING,
	STOP_TRACKING,
	DISCARD_STREAMLINE,
	FAIL_STREAMLINE
} ROI_Rule_Decision;

class TractographyAlgorithm;
class TrackWith_Trekker;
class Tractogram;

class TrackingThread {
public:

	TrackingThread();
	~TrackingThread();

	void 		 			updateSeedNoAndTrialCount(size_t _seedNo,size_t _trialNo);
	void 		 			updateTractogram();

	bool 		 			isReady;
	bool 					timeIsUp;

	size_t 					report_success_REACHED_MAXLENGTH_LIMIT;
	size_t 					report_success_REACHED_MINDATASUPPORT_LIMIT;
	size_t 					report_success_SATISFIED_PATHWAY_RULES;

	size_t 					report_discard_TOO_SHORT;
	size_t 					report_discard_TOO_LONG;
	size_t 					report_discard_DISCARD_ROI_REACHED;
	size_t 					report_discard_REQUIRED_ROI_NOT_MET;
	size_t 					report_discard_REQUIRED_ROI_ORDER_NOT_MET;
	size_t 					report_discard_ENDED_INSIDE_DISCARD_ROI;
	size_t 					report_discard_REACHED_TIME_LIMIT;

	size_t 					report_failed_BY_THE_ALGORITHM_DURING_INITIALIZATION;
	size_t 					report_failed_BY_THE_ALGORITHM;
	size_t 					report_failed_REACHED_TRIAL_LIMIT_DURING_INITIALIZATION;
	size_t 					report_failed_REACHED_TRIAL_LIMIT;
	size_t 					report_UNEXPECTED_TRACKING_STATUS;

	TractographyAlgorithm  *method;
	Streamline             *streamline;
	bool 					appendStreamline;
	size_t 					trialNo;

	size_t 	            	seedNo;
	Coordinate              seed_coordinates;
	Coordinate              seed_init_direction;

	FOD_Image      		   *tracker_FOD;
	ROI_Image      		   *tracker_SEED;

	std::vector<ROI_Image*> tracker_ROI;
	std::vector<bool> 		tracker_ROI_ready_status;

	Tracking_Side 			tracker_side;
	int 					tracker_ROI_order;

	RandomDoer             *tracker_randomThings;

	void 		            getSeed();
	void 		            getSeed(Coordinate* point);
	void 		            track(Coordinate* point);
	ROI_Rule_Decision 	    checkPathway();
	StreamlineStatus 	    run(bool side);

};

#endif
