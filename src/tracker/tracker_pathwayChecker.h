#ifndef SRC_TRACKER_PATHWAYCHECKER_H_
#define SRC_TRACKER_PATHWAYCHECKER_H_

#include "tracker_thread.h"

ROI_Rule_Decision TrackingThread::checkPathway() {
   
    // First handle the discard rules
    for (size_t i=0; i<tracker_ROI.size(); i++ ) {
   
        if (tracker_ROI[i]->type == roi_type_discard_if_enters) {
        
            float val = tracker_ROI[i]->getVal(streamline->coordinates.back());
            
            if (val>=0.5) {

                if (tracker_ROI[i]->side==side_undefined) {

                    streamline->discardingReason = DISCARD_ROI_REACHED;
                    return DISCARD_STREAMLINE;

                } else if (tracker_side==side_undefined) {

                    tracker_side=tracker_ROI[i]->side;
                    tracker_ROI_ready_status[i] = true;
                    streamline->discardingReason = DISCARD_ROI_REACHED;
                    return DISCARD_STREAMLINE;

                } else if ( ((tracker_side==side_A) && (tracker_ROI[i]->side==side_A)) || ((tracker_side==side_B) && (tracker_ROI[i]->side==side_B)) ) {

                    tracker_ROI_ready_status[i] = true;
                    streamline->discardingReason = DISCARD_ROI_REACHED;
                    return DISCARD_STREAMLINE;

                }
            }
            
        }
        
        
        
        if (tracker_ROI[i]->type == roi_type_discard_if_exits) {
        
            float val = tracker_ROI[i]->getVal(streamline->coordinates.back());
            
            if ((tracker_ROI[i]->entry_status!=entered) && (val>=0.5)) {
                tracker_ROI[i]->entry_status = entered;
                if (tracker_side==side_undefined)
                    tracker_side=tracker_ROI[i]->side;
                break;
            } else if ((tracker_ROI[i]->entry_status==entered) && (val<0.5)) {

                if (tracker_ROI[i]->side==side_undefined) {
                    streamline->discardingReason = DISCARD_ROI_REACHED;
                    return DISCARD_STREAMLINE;
                } else if ( ((tracker_side==side_A) && (tracker_ROI[i]->side==side_A)) || ((tracker_side==side_B) && (tracker_ROI[i]->side==side_B)) ) {
                    tracker_ROI_ready_status[i] = true;
                    streamline->discardingReason = DISCARD_ROI_REACHED;
                    return DISCARD_STREAMLINE;
                }
            }
            
        }
        
            
    }
    
    
    // Then handle the rest
	for (size_t i=0; i<tracker_ROI.size(); i++ ) {

		if ( tracker_ROI_ready_status[i]==false ) {

			float val 		= tracker_ROI[i]->getVal(streamline->coordinates.back());

			switch (tracker_ROI[i]->type) {
                
			case roi_type_req_entry: {

				if (tracker_ROI[i]->side==side_undefined) {

					if (val>=0.5)
						tracker_ROI_ready_status[i] = true;

				} else if (tracker_side==side_undefined) {

					if (val>=0.5) {

						tracker_side=tracker_ROI[i]->side;

						if ((PATHWAY::satisfy_requirements_in_order==IN_ORDER) && (tracker_side==side_A)) {
							if (PATHWAY::order_of_side_A_ROIs.at(tracker_ROI_order)!=tracker_ROI[i]->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
							else
								tracker_ROI_order++;
						}

						if ((PATHWAY::satisfy_requirements_in_order==IN_ORDER) && (tracker_side==side_B)) {
							if (PATHWAY::order_of_side_B_ROIs.at(tracker_ROI_order)!=tracker_ROI[i]->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
							else
								tracker_ROI_order++;
						}

						tracker_ROI_ready_status[i] = true;
					}

				} else if ( ((tracker_side==side_A) && (tracker_ROI[i]->side==side_A)) ) {

					if (val>=0.5) {

						if (PATHWAY::satisfy_requirements_in_order==IN_ORDER) {
							if (PATHWAY::order_of_side_A_ROIs.at(tracker_ROI_order)!=tracker_ROI[i]->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
							else
								tracker_ROI_order++;
						}

						tracker_ROI_ready_status[i] = true;
					}

				} else if ( ((tracker_side==side_B) && (tracker_ROI[i]->side==side_B)) ) {

					if (val>=0.5) {

						if (PATHWAY::satisfy_requirements_in_order==IN_ORDER) {
							if (PATHWAY::order_of_side_B_ROIs.at(tracker_ROI_order)!=tracker_ROI[i]->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
							else
								tracker_ROI_order++;
						}

						tracker_ROI_ready_status[i] = true;
					}

				}

				break;
			}

			case roi_type_req_exit: {

				if ((tracker_ROI[i]->entry_status!=entered) && (val>=0.5)) {
					tracker_ROI[i]->entry_status = entered;
					if (tracker_side==side_undefined)
						tracker_side=tracker_ROI[i]->side;
					break;
				} else if ((tracker_ROI[i]->entry_status==entered) && (val<0.5)) {

					if (tracker_ROI[i]->side==side_undefined) {
						tracker_ROI_ready_status[i] = true;
						break;
					} else if (PATHWAY::satisfy_requirements_in_order==IN_ORDER) {

						if (( ((tracker_side==side_A) && (tracker_ROI[i]->side==side_A)) )) {
							if (PATHWAY::order_of_side_A_ROIs.at(tracker_ROI_order)!=tracker_ROI[i]->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
							else {
								tracker_ROI_order++;
								tracker_ROI_ready_status[i] = true;
								break;
							}
						} else if (( ((tracker_side==side_B) && (tracker_ROI[i]->side==side_B)) )) {
							if (PATHWAY::order_of_side_A_ROIs.at(tracker_ROI_order)!=tracker_ROI[i]->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
							else {
								tracker_ROI_order++;
								tracker_ROI_ready_status[i] = true;
								break;
							}

						}

					} else {

						if ( ((tracker_side==side_A) && (tracker_ROI[i]->side==side_A)) || ((tracker_side==side_B) && (tracker_ROI[i]->side==side_B)) ) {
							tracker_ROI_ready_status[i] = true;
							break;
						}

					}

				}

				break;

			}

			case roi_type_stop_at_entry: {

				if (tracker_ROI[i]->side==side_undefined) {

					if (val>=0.5) {
						if (streamline->terminationReason_sideA==TERMINATIONREASON_NOTSET)
							streamline->terminationReason_sideA = STOP_ROI_REACHED;
						else
							streamline->terminationReason_sideB = STOP_ROI_REACHED;
						return STOP_TRACKING;
					}

				} else if (tracker_side==side_undefined) {

					if (val>=0.5) {

						tracker_side=tracker_ROI[i]->side;

						if ((PATHWAY::satisfy_requirements_in_order==IN_ORDER) && (tracker_side==side_A)) {
							if (PATHWAY::order_of_side_A_ROIs.at(tracker_ROI_order)!=tracker_ROI[i]->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
						}

						if ((PATHWAY::satisfy_requirements_in_order==IN_ORDER) && (tracker_side==side_B)) {
							if (PATHWAY::order_of_side_B_ROIs.at(tracker_ROI_order)!=tracker_ROI[i]->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
						}

						tracker_ROI_ready_status[i] = true;
						if       (tracker_side==side_A) streamline->terminationReason_sideA = STOP_ROI_REACHED;
						else if  (tracker_side==side_B) streamline->terminationReason_sideB = STOP_ROI_REACHED;
						return STOP_TRACKING;

					}

				} else if ( ((tracker_side==side_A) && (tracker_ROI[i]->side==side_A)) ) {

					if (val>=0.5) {

						if (PATHWAY::satisfy_requirements_in_order==IN_ORDER) {
							if (PATHWAY::order_of_side_A_ROIs.at(tracker_ROI_order)!=tracker_ROI[i]->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
						}

						tracker_ROI_ready_status[i] = true;
						streamline->terminationReason_sideA = STOP_ROI_REACHED;
						return STOP_TRACKING;

					}

				} else if ( ((tracker_side==side_B) && (tracker_ROI[i]->side==side_B)) ) {

					if (val>=0.5) {

						if (PATHWAY::satisfy_requirements_in_order==IN_ORDER) {
							if (PATHWAY::order_of_side_B_ROIs.at(tracker_ROI_order)!=tracker_ROI[i]->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
						}

						tracker_ROI_ready_status[i] = true;
						streamline->terminationReason_sideB = STOP_ROI_REACHED;
						return STOP_TRACKING;

					}

				}


				break;
			}

			case roi_type_stop_at_exit: {

				if ((tracker_ROI[i]->entry_status!=entered) && (val>=0.5)) {
					tracker_ROI[i]->entry_status = entered;

					if (tracker_side==side_undefined)
						tracker_side=tracker_ROI[i]->side;
					break;
				} else if ((tracker_ROI[i]->entry_status==entered) && (val<0.5)) {

					if (tracker_ROI[i]->side==side_undefined) {
						if (streamline->terminationReason_sideA==TERMINATIONREASON_NOTSET)
							streamline->terminationReason_sideA = STOP_ROI_REACHED;
						else
							streamline->terminationReason_sideB = STOP_ROI_REACHED;
						return STOP_TRACKING;
					} else if (PATHWAY::satisfy_requirements_in_order==IN_ORDER) {

						if (( ((tracker_side==side_A) && (tracker_ROI[i]->side==side_A)) )) {
							if (PATHWAY::order_of_side_A_ROIs.at(tracker_ROI_order)!=tracker_ROI[i]->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
							else {
								tracker_ROI_order++;
								tracker_ROI_ready_status[i] = true;
								streamline->terminationReason_sideA = STOP_ROI_REACHED;
								return STOP_TRACKING;
							}
						} else if (( ((tracker_side==side_B) && (tracker_ROI[i]->side==side_B)) )) {
							if (PATHWAY::order_of_side_B_ROIs.at(tracker_ROI_order)!=tracker_ROI[i]->self) {
								streamline->discardingReason=REQUIRED_ROI_ORDER_NOT_MET;
								return DISCARD_STREAMLINE;
							}
							else {
								tracker_ROI_order++;
								tracker_ROI_ready_status[i] = true;
								streamline->terminationReason_sideB = STOP_ROI_REACHED;
								return STOP_TRACKING;
							}

						}

					} else {

						if ( ((tracker_side==side_A) && (tracker_ROI[i]->side==side_A)) || ((tracker_side==side_B) && (tracker_ROI[i]->side==side_B)) ) {
							tracker_ROI_ready_status[i] = true;
							if       (tracker_side==side_A) streamline->terminationReason_sideA = STOP_ROI_REACHED;
							else if  (tracker_side==side_B) streamline->terminationReason_sideB = STOP_ROI_REACHED;
							return STOP_TRACKING;
						}

					}

				}

				break;
			}

			default: { break; }

			}

		}

	}


	return CONTINUE_TRACKING;

}

seedROI_Append_Decision TrackingThread::isInsideSeedROI() {
    
    float val 		= tracker_SEED->getVal(streamline->coordinates.back());
    
    if (val<0.5) {
        tracker_SEED->exit_status  = exited;
        tracker_SEED->entry_status = notEntered;
        return APPEND;
    } else if (tracker_SEED->entry_status == notEntered) {
        return REENTERED;
    } else {
        return DONTAPPEND;
    }
    
}

#endif
