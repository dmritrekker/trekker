#ifndef SRC_TRACTOGRAM_H_
#define SRC_TRACTOGRAM_H_

#include <stdio.h>
#include <fstream>
#include "inputParser.h"
#include "streamline.h"
#include "../tracker/tracker_thread.h"


class TrackingThread;

class Tractogram {

public:
	Tractogram();
	virtual ~Tractogram();
	virtual void reset();

	virtual void writeOutput() = 0;
	virtual void writeMetadataOutput() = 0;
	virtual void printSummary() = 0;
	virtual void update(TrackingThread *thread) = 0;

	std::vector<Streamline*> streamlines;
	size_t  streamlineCount;


protected:

	size_t 	total_propagationCount;

	int 	totalPointCount;
	int 	countGoodStreamlines;
	std::vector<int> pointCounts;

	FILE* 	writeBaseOutput();
	FILE* 	writeBaseMetadataOutput();
	void 	writeTrackingReportInMetadataOutput(FILE* out);
	void 	printBaseSummary();
	void 	baseUpdate(TrackingThread *thread);

	size_t 	total_tried;
	size_t 	total_generated;
	size_t 	total_success;
	size_t  total_discard;
	size_t 	total_fail;

	void swapByteOrder_int(int& a);
	void swapByteOrder_float(float& a);

private:

	size_t 	total_success_REACHED_MAXLENGTH_LIMIT;
	size_t 	total_success_REACHED_MINDATASUPPORT_LIMIT;
	size_t 	total_success_SATISFIED_PATHWAY_RULES;

	size_t 	total_discard_TOO_SHORT;
	size_t 	total_discard_TOO_LONG;
	size_t 	total_discard_DISCARD_ROI_REACHED;
	size_t 	total_discard_REQUIRED_ROI_NOT_MET;
	size_t 	total_discard_REQUIRED_ROI_ORDER_NOT_MET;
	size_t 	total_discard_ENDED_INSIDE_DISCARD_ROI;
	size_t 	total_discard_REACHED_TIME_LIMIT;

	size_t  total_failed_DISCARDED_BY_THE_ALGORITHM_DURING_INITIALIZATION;
	size_t 	total_failed_DISCARDED_BY_THE_ALGORITHM;
	size_t  total_failed_REACHED_TRIAL_LIMIT_DURING_INITIALIZATION;
	size_t  total_failed_REACHED_TRIAL_LIMIT;

	size_t 	total_UNEXPECTED_TRACKING_STATUS;

};

#endif
