#ifndef SRC_TRACKER_PATHWAYCHECKER_H_
#define SRC_TRACKER_PATHWAYCHECKER_H_

#include "tracker_thread.h"

ROI_Rule_Decision TrackingThread::checkPathway() {

	std::vector<bool>::iterator bit = tracker_ROI_ready_status.begin();

	for (std::vector<ROI_Image*>::iterator it = tracker_ROI.begin(); it != tracker_ROI.end(); it++) {

		if ( (*bit)==false ) {

			float val 		= (*it)->getVal(streamline->coordinates.back());

			switch ((*it)->type) {
			case roi_type_req_entry: {

				if ((*it)->side==side_undefined) {

					if (val>=0.5)
						(*bit) = true;

				} else if (tracker_side==side_undefined) {

					if (val>=0.5) {

						tracker_side=(*it)->side;

						if ((PATHWAY::satisfy_requirements_in_order==IN_ORDER) && (tracker_side==side_A)) {
							if (PATHWAY::order_of_side_A_ROIs.at(tracker_ROI_order)!=(*it)->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
							else
								tracker_ROI_order++;
						}

						if ((PATHWAY::satisfy_requirements_in_order==IN_ORDER) && (tracker_side==side_B)) {
							if (PATHWAY::order_of_side_B_ROIs.at(tracker_ROI_order)!=(*it)->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
							else
								tracker_ROI_order++;
						}

						(*bit) = true;
					}

				} else if ( ((tracker_side==side_A) && ((*it)->side==side_A)) ) {

					if (val>=0.5) {

						if (PATHWAY::satisfy_requirements_in_order==IN_ORDER) {
							if (PATHWAY::order_of_side_A_ROIs.at(tracker_ROI_order)!=(*it)->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
							else
								tracker_ROI_order++;
						}

						(*bit) = true;
					}

				} else if ( ((tracker_side==side_B) && ((*it)->side==side_B)) ) {

					if (val>=0.5) {

						if (PATHWAY::satisfy_requirements_in_order==IN_ORDER) {
							if (PATHWAY::order_of_side_B_ROIs.at(tracker_ROI_order)!=(*it)->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
							else
								tracker_ROI_order++;
						}

						(*bit) = true;
					}

				}

				break;
			}

			case roi_type_req_exit: {

				if (((*it)->entry_status!=entered) && (val>=0.5)) {
					(*it)->entry_status = entered;
					if (tracker_side==side_undefined)
						tracker_side=(*it)->side;
					break;
				} else if (((*it)->entry_status==entered) && (val<0.5)) {

					if ((*it)->side==side_undefined) {
						(*bit) = true;
						break;
					} else if (PATHWAY::satisfy_requirements_in_order==IN_ORDER) {

						if (( ((tracker_side==side_A) && ((*it)->side==side_A)) )) {
							if (PATHWAY::order_of_side_A_ROIs.at(tracker_ROI_order)!=(*it)->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
							else {
								tracker_ROI_order++;
								(*bit) = true;
								break;
							}
						} else if (( ((tracker_side==side_B) && ((*it)->side==side_B)) )) {
							if (PATHWAY::order_of_side_A_ROIs.at(tracker_ROI_order)!=(*it)->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
							else {
								tracker_ROI_order++;
								(*bit) = true;
								break;
							}

						}

					} else {

						if ( ((tracker_side==side_A) && ((*it)->side==side_A)) || ((tracker_side==side_B) && ((*it)->side==side_B)) ) {
							(*bit) = true;
							break;
						}

					}

				}

				break;

			}

			case roi_type_stop_at_entry: {

				if ((*it)->side==side_undefined) {

					if (val>=0.5) {
						if (streamline->terminationReason_sideA==TERMINATIONREASON_NOTSET)
							streamline->terminationReason_sideA = STOP_ROI_REACHED;
						else
							streamline->terminationReason_sideB = STOP_ROI_REACHED;
						return STOP_TRACKING;
					}

				} else if (tracker_side==side_undefined) {

					if (val>=0.5) {

						tracker_side=(*it)->side;

						if ((PATHWAY::satisfy_requirements_in_order==IN_ORDER) && (tracker_side==side_A)) {
							if (PATHWAY::order_of_side_A_ROIs.at(tracker_ROI_order)!=(*it)->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
						}

						if ((PATHWAY::satisfy_requirements_in_order==IN_ORDER) && (tracker_side==side_B)) {
							if (PATHWAY::order_of_side_B_ROIs.at(tracker_ROI_order)!=(*it)->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
						}

						(*bit) = true;
						if       (tracker_side==side_A) streamline->terminationReason_sideA = STOP_ROI_REACHED;
						else if  (tracker_side==side_B) streamline->terminationReason_sideB = STOP_ROI_REACHED;
						return STOP_TRACKING;

					}

				} else if ( ((tracker_side==side_A) && ((*it)->side==side_A)) ) {

					if (val>=0.5) {

						if (PATHWAY::satisfy_requirements_in_order==IN_ORDER) {
							if (PATHWAY::order_of_side_A_ROIs.at(tracker_ROI_order)!=(*it)->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
						}

						(*bit) = true;
						streamline->terminationReason_sideA = STOP_ROI_REACHED;
						return STOP_TRACKING;

					}

				} else if ( ((tracker_side==side_B) && ((*it)->side==side_B)) ) {

					if (val>=0.5) {

						if (PATHWAY::satisfy_requirements_in_order==IN_ORDER) {
							if (PATHWAY::order_of_side_B_ROIs.at(tracker_ROI_order)!=(*it)->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
						}

						(*bit) = true;
						streamline->terminationReason_sideB = STOP_ROI_REACHED;
						return STOP_TRACKING;

					}

				}


				break;
			}

			case roi_type_stop_at_exit: {

				if (((*it)->entry_status!=entered) && (val>=0.5)) {
					(*it)->entry_status = entered;

					if (tracker_side==side_undefined)
						tracker_side=(*it)->side;
					break;
				} else if (((*it)->entry_status==entered) && (val<0.5)) {

					if ((*it)->side==side_undefined) {
						if (streamline->terminationReason_sideA==TERMINATIONREASON_NOTSET)
							streamline->terminationReason_sideA = STOP_ROI_REACHED;
						else
							streamline->terminationReason_sideB = STOP_ROI_REACHED;
						return STOP_TRACKING;
					} else if (PATHWAY::satisfy_requirements_in_order==IN_ORDER) {

						if (( ((tracker_side==side_A) && ((*it)->side==side_A)) )) {
							if (PATHWAY::order_of_side_A_ROIs.at(tracker_ROI_order)!=(*it)->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
							else {
								tracker_ROI_order++;
								(*bit) = true;
								streamline->terminationReason_sideA = STOP_ROI_REACHED;
								return STOP_TRACKING;
							}
						} else if (( ((tracker_side==side_B) && ((*it)->side==side_B)) )) {
							if (PATHWAY::order_of_side_B_ROIs.at(tracker_ROI_order)!=(*it)->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
							else {
								tracker_ROI_order++;
								(*bit) = true;
								streamline->terminationReason_sideB = STOP_ROI_REACHED;
								return STOP_TRACKING;
							}

						}

					} else {

						if ( ((tracker_side==side_A) && ((*it)->side==side_A)) || ((tracker_side==side_B) && ((*it)->side==side_B)) ) {
							(*bit) = true;
							if       (tracker_side==side_A) streamline->terminationReason_sideA = STOP_ROI_REACHED;
							else if  (tracker_side==side_B) streamline->terminationReason_sideB = STOP_ROI_REACHED;
							return STOP_TRACKING;
						}

					}

				}

				break;
			}

			case roi_type_discard_if_enters: {

				if (val>=0.5) {

					if ((*it)->side==side_undefined) {

						streamline->discardingReason = DISCARD_ROI_REACHED;
						return DISCARD_STREAMLINE;

					} else if (tracker_side==side_undefined) {

						tracker_side=(*it)->side;
						(*bit) = true;
						streamline->discardingReason = DISCARD_ROI_REACHED;
						return DISCARD_STREAMLINE;

					} else if ( ((tracker_side==side_A) && ((*it)->side==side_A)) || ((tracker_side==side_B) && ((*it)->side==side_B)) ) {

						(*bit) = true;
						streamline->discardingReason = DISCARD_ROI_REACHED;
						return DISCARD_STREAMLINE;

					}
				}

				break;
			}

			case roi_type_discard_if_exits: {

				if (((*it)->entry_status!=entered) && (val>=0.5)) {
					(*it)->entry_status = entered;
					if (tracker_side==side_undefined)
						tracker_side=(*it)->side;
					break;
				} else if (((*it)->entry_status==entered) && (val<0.5)) {

					if ((*it)->side==side_undefined) {
						streamline->discardingReason = DISCARD_ROI_REACHED;
						return DISCARD_STREAMLINE;
					} else if ( ((tracker_side==side_A) && ((*it)->side==side_A)) || ((tracker_side==side_B) && ((*it)->side==side_B)) ) {
						(*bit) = true;
						streamline->discardingReason = DISCARD_ROI_REACHED;
						return DISCARD_STREAMLINE;
					}

				}

				break;
			}

			default: break;

			}

		}

		bit++;
	}


	return CONTINUE_TRACKING;

}


#endif
