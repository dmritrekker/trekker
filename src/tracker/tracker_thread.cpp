#include "tracker_thread.h"
#include "tracker_pathwayChecker.h"
#include "tracker_seeder.h"

#include "../base/algorithm.h"

#include "algorithms/ptt/algorithm_ptt.h"
#include "algorithms/local_probabilistic/algorithm_local_probabilistic.h"


void getStreamline(TrackingThread* tracker) {
	tracker->track(NULL);
    
    GENERAL::tracker_lock.lock();
    GENERAL::ready_thread_id=tracker->thread_id;
    tracker->updateTractogram();
    if (GENERAL::verboseLevel!=QUITE) TRACKER::tractogram->printSummary();
    
    std::lock_guard<std::mutex> lk(MT::exit_mx);
    MT::exit_cv.notify_all();
}

TrackingThread::TrackingThread() {

    thread_id                                   = 0;
    
	streamline 									= NULL;
	appendStreamline 							= false;
	seedNo 										= 0;
	trialNo 									= 0;

	timeIsUp 									= false;

	tracker_randomThings 						= new RandomDoer();
	tracker_FOD 								= new FOD_Image(*TRACKER::img_FOD);
    if (SEED::seedingMode==SEED_IMAGE) { 
        tracker_SEED = new ROI_Image(*SEED::img_SEED);
    }
    
	switch (TRACKER::algorithm) {
	case PTT:
		method 									= new TrackWith_PTT();
		break;
	case LOCAL_PROBABILISTIC:
		method 									= new TrackWith_Local_Probabilistic();
		break;
	default :
		break;
	}
	method->setThread(this);

    
	for (std::vector<ROI_Image*>::iterator it = PATHWAY::img_ROI.begin(); it != PATHWAY::img_ROI.end(); ++it) {
		tracker_ROI.push_back(new ROI_Image(**it));
		tracker_ROI_ready_status.push_back(false);
	}

	tracker_side 											= side_undefined;
	tracker_ROI_order 										= 0;

	report_success_REACHED_MAXLENGTH_LIMIT 					= 0;
	report_success_REACHED_MINDATASUPPORT_LIMIT 			= 0;
	report_success_SATISFIED_PATHWAY_RULES 					= 0;

	report_discard_TOO_SHORT 								= 0;
	report_discard_TOO_LONG 								= 0;
	report_discard_DISCARD_ROI_REACHED 						= 0;
	report_discard_REQUIRED_ROI_NOT_MET 				 	= 0;
	report_discard_REQUIRED_ROI_ORDER_NOT_MET 			 	= 0;
	report_discard_ENDED_INSIDE_DISCARD_ROI 			 	= 0;
    report_discard_REENTERED_SEED_ROI                       = 0;
	report_discard_REACHED_TIME_LIMIT 						= 0;

	report_failed_BY_THE_ALGORITHM_DURING_INITIALIZATION 	= 0;
	report_failed_BY_THE_ALGORITHM 						 	= 0;
	report_failed_REACHED_TRIAL_LIMIT_DURING_INITIALIZATION = 0;
	report_failed_REACHED_TRIAL_LIMIT 					 	= 0;
	report_UNEXPECTED_TRACKING_STATUS 					 	= 0;
    
	tracker_randomThings->init_uniform_int(SEED::seed_indices.size()-1);

}



TrackingThread::~TrackingThread() {

	tracker_FOD->destroyCopy();
	delete   tracker_FOD;
    if (SEED::seedingMode==SEED_IMAGE) {
        tracker_SEED->destroyCopy();
        delete tracker_SEED;
    }
	delete 	 tracker_randomThings;
	delete   method;

	for (std::vector<ROI_Image*>::iterator it = tracker_ROI.begin(); it != tracker_ROI.end(); ++it) {(*it)->destroyCopy(); delete *it;}

}

void TrackingThread::setThreadID(size_t id) {
    thread_id = id;
}


void TrackingThread::updateSeedNoAndTrialCount(size_t _seedNo,size_t _trialNo) {
	seedNo  	= _seedNo;
	trialNo 	= _trialNo;
	streamline 	= NULL;
}

void TrackingThread::updateTractogram() {
	if (appendStreamline) {
		TRACKER::tractogram->streamlines.push_back(streamline);
		TRACKER::tractogram->streamlineCount++;
	}
	TRACKER::tractogram->update(this);
}

void TrackingThread::track(Coordinate *point) {

	if (GENERAL::verboseLevel > ON) std::cout << "Seed no: " << seedNo << std::endl;

	if (streamline==NULL) {
		switch (TRACKER::algorithm) {
		case PTT:
			streamline 		= new Streamline_PTT();
			break;
		case LOCAL_PROBABILISTIC:
			streamline 		= new Streamline_Local_Probabilistic();
			break;
		default :
			break;
		}
		appendStreamline = true;
	} else
		appendStreamline = false;



	method->setStreamline(streamline);

	report_success_REACHED_MAXLENGTH_LIMIT 					= 0;
	report_success_REACHED_MINDATASUPPORT_LIMIT 			= 0;
	report_success_SATISFIED_PATHWAY_RULES 					= 0;

	report_discard_TOO_SHORT 								= 0;
	report_discard_TOO_LONG 								= 0;
	report_discard_DISCARD_ROI_REACHED 						= 0;
	report_discard_REQUIRED_ROI_NOT_MET 				 	= 0;
	report_discard_REQUIRED_ROI_ORDER_NOT_MET 			 	= 0;
	report_discard_ENDED_INSIDE_DISCARD_ROI 			 	= 0;
	report_discard_REACHED_TIME_LIMIT 						= 0;

	report_failed_BY_THE_ALGORITHM_DURING_INITIALIZATION 	= 0;
	report_failed_BY_THE_ALGORITHM 						 	= 0;
	report_failed_REACHED_TRIAL_LIMIT_DURING_INITIALIZATION = 0;
	report_failed_REACHED_TRIAL_LIMIT 					 	= 0;
	report_UNEXPECTED_TRACKING_STATUS 					 	= 0;

	int tries = trialNo;

    // Loop until success - fail and discard cases break the loop
	while (streamline->status != STREAMLINE_GOOD) {

		// Clean streamline from the previous trial
		streamline->flush();

		// Initialize pathway rules
		tracker_side 		= side_undefined;
		tracker_ROI_order 	= 0;

		std::vector<bool>::iterator bit = tracker_ROI_ready_status.begin();
		for (std::vector<ROI_Image*>::iterator it = tracker_ROI.begin(); it != tracker_ROI.end(); ++it) {

			(*it)->entry_status 	= entry_status_undefined;
			(*it)->exit_status 		= exit_status_undefined;

			if ( (*it)->type==roi_type_discard_if_ends_inside )
				(*bit) = true;
			else
				(*bit) = false;
			bit++;

		}

		// Get and set new seed
		if (point==NULL) getSeed();
		else			 getSeed(point);

		streamline->seed_coordinates = seed_coordinates;
		if ( GENERAL::verboseLevel > DETAILED ) {
			std::cout << "seed_coordinates : "; seed_coordinates.print(); 			std::cout << std::endl;
			std::cout << "seed_direction   : "; seed_init_direction.print(); 		std::cout << std::endl;
		}
		method->setSeed();

        // Initialize streamline
		Initialization_Decision init_decision = method->initialize();
        
        if (GENERAL::runTime()>GENERAL::timeLimit) {
            
            timeIsUp 						= true;
            streamline->discardingReason 	= REACHED_TIME_LIMIT;
            streamline->status 				= STREAMLINE_DISCARDED;
            
        } else {
            
            if (init_decision==INIT_STOP) {
                streamline->failingReason 		= REACHED_INITIALIZATION_TRIAL_LIMIT;
                streamline->status 				= STREAMLINE_FAIL;
            }

            if (init_decision==INIT_FAIL) {
                streamline->failingReason 		= DISCARDED_BY_THE_ALGORITHM_DURING_INITIALIZATION;
                streamline->status 				= STREAMLINE_FAIL;
            }
            
        }
		
		// If initialization is OK, continue tracking
		if (init_decision==INIT_CONTINUE) {

			// Track first side
			streamline->status 				= run(true);

			// Check if ROI rules are OK on this side
			if (streamline->status == STREAMLINE_GOOD) {
				if (tracker_side==side_A) {
					std::vector<bool>::iterator lbit = tracker_ROI_ready_status.begin();
					for (std::vector<ROI_Image*>::iterator it = tracker_ROI.begin(); it != tracker_ROI.end(); ++it) {
						if ( ((*it)->side==side_A) && (((*it)->type==roi_type_req_entry) || ((*it)->type==roi_type_req_exit)) && ((*lbit)==false) ) {
							streamline->discardingReason 	= REQUIRED_ROI_NOT_MET;
							streamline->status 				= STREAMLINE_DISCARDED;
							break;
						}
						lbit++;
					}
				} else if (tracker_side==side_B) {
					std::vector<bool>::iterator lbit = tracker_ROI_ready_status.begin();
					for (std::vector<ROI_Image*>::iterator it = tracker_ROI.begin(); it != tracker_ROI.end(); ++it) {
						if ( ( (*it)->side==side_B) && (((*it)->type==roi_type_req_entry) || ((*it)->type==roi_type_req_exit)) && ((*lbit)==false) ) {
							streamline->discardingReason 	= REQUIRED_ROI_NOT_MET;
							streamline->status 				= STREAMLINE_DISCARDED;
							break;
						}
						lbit++;
					}
				} else {
					for (std::vector<ROI_Image*>::iterator it = tracker_ROI.begin(); it != tracker_ROI.end(); ++it) {
						(*it)->entry_status 	= entry_status_undefined;
						(*it)->exit_status 		= exit_status_undefined;
					}
				}
			}


			// Track the other side
			if ((TRACKER::directionality==TWO_SIDED) && (streamline->status == STREAMLINE_GOOD)) {

				streamline->status = STREAMLINE_NOTSET;

				// Flip tracking algorithm BEFORE flipping streamline
				method->flip();

				// Flip streamline AFTER flipping method
				streamline->flip();

				// Flip tracking side
				if      (tracker_side==side_A) 	tracker_side=side_B;
				else if (tracker_side==side_B) 	tracker_side=side_A;

				// Reset the required ROI order
				tracker_ROI_order = 0;

				// Track other side
				streamline->status = run(false);

			}


			if (streamline->status == STREAMLINE_GOOD) {
				// Check length constraint
				if (streamline->getLength() < TRACKER::minLength) {
					streamline->discardingReason 	= TOO_SHORT;
					streamline->status 				= STREAMLINE_DISCARDED;
				}

				// Final check of ROI rules
				std::vector<bool>::iterator lbit = tracker_ROI_ready_status.begin();
				for (std::vector<ROI_Image*>::iterator it = PATHWAY::img_ROI.begin(); it != PATHWAY::img_ROI.end(); ++it) {
					if ( (((*it)->type==roi_type_req_entry) || ((*it)->type==roi_type_req_exit)) && ((*lbit)==false) ){
						streamline->discardingReason 	= REQUIRED_ROI_NOT_MET;
						streamline->status 				= STREAMLINE_DISCARDED;
						break;
					}
					lbit++;
				}
			}

		}
		
		tries++;
        
        // At this point tracking of the streamline is complete and one of the following decisions is made:
        // - STREAMLINE_GOOD
        // - STREAMLINE_DISCARDED
        // - STREAMLINE_FAIL
        

		if (streamline->status == STREAMLINE_DISCARDED) {

			switch (streamline->discardingReason) {
			case DIRCARDINGREASON_NOTSET:
				if (GENERAL::verboseLevel > ON) std::cout << "DIRCARDINGREASON_NOTSET";
				report_UNEXPECTED_TRACKING_STATUS++;
				break;
			case TOO_SHORT:
				if (GENERAL::verboseLevel > ON) std::cout << "TOO_SHORT";
				report_discard_TOO_SHORT++;
				break;
			case TOO_LONG:
				if (GENERAL::verboseLevel > ON) std::cout << "TOO_LONG";
				report_discard_TOO_LONG++;
				break;
			case DISCARD_ROI_REACHED:
				if (GENERAL::verboseLevel > ON) std::cout << "DISCARD_ROI_REACHED";
				report_discard_DISCARD_ROI_REACHED++;
				break;
			case REQUIRED_ROI_NOT_MET:
				if (GENERAL::verboseLevel > ON) std::cout << "REQUIRED_ROI_NOT_MET";
				report_discard_REQUIRED_ROI_NOT_MET++;
				break;
			case REQUIRED_ROI_ORDER_NOT_MET:
				if (GENERAL::verboseLevel > ON) std::cout << "REQUIRED_ROI_ORDER_NOT_MET";
				report_discard_REQUIRED_ROI_ORDER_NOT_MET++;
				break;
			case ENDED_INSIDE_DISCARD_ROI:
				if (GENERAL::verboseLevel > ON) std::cout << "ENDED_INSIDE_DISCARD_ROI";
				report_discard_ENDED_INSIDE_DISCARD_ROI++;
				break;
            case REENTERED_SEED_ROI:
				if (GENERAL::verboseLevel > ON) std::cout << "REENTERED_SEED_ROI";
				report_discard_REENTERED_SEED_ROI++;
				break;
			case REACHED_TIME_LIMIT:
				if (GENERAL::verboseLevel > ON) std::cout << "REACHED TIME LIMIT";
				report_discard_REACHED_TIME_LIMIT++;
				break;
			default:
				break;
			}
			if (GENERAL::verboseLevel > ON) std::cout << std::endl;
		}

		if (streamline->status == STREAMLINE_FAIL) {
			switch (streamline->failingReason) {
			case FAILREASON_NOTSET:
				if (GENERAL::verboseLevel > ON) std::cout << "FAILINGREASON_NOTSET";
				report_UNEXPECTED_TRACKING_STATUS++;
				break;
			case REACHED_INITIALIZATION_TRIAL_LIMIT:
				if (GENERAL::verboseLevel > ON) std::cout << "INITIALIZATION_FAILED (all " << tries << " candidates failed)";
				report_failed_REACHED_TRIAL_LIMIT_DURING_INITIALIZATION++;
				break;
			case DISCARDED_BY_THE_ALGORITHM_DURING_INITIALIZATION:
				if (GENERAL::verboseLevel > ON) std::cout << "DISCARDED_BY_THE_ALGORITHM_DURING_INITIALIZATION";
				report_failed_BY_THE_ALGORITHM_DURING_INITIALIZATION++;
				break;
			case DISCARDED_BY_THE_ALGORITHM:
				if (GENERAL::verboseLevel > ON) std::cout << "DISCARDED_BY_THE_ALGORITHM";
				report_failed_BY_THE_ALGORITHM++;
				break;
			}
			if (GENERAL::verboseLevel > ON) std::cout << std::endl;
		}


		// The algorithm discards this streamline, this is considered as a fail case for the tracker
		// Therefore it will be immediately reported to the tractogram in order to update settings for the algorithm
		// so that such a case will not be caused by the algorithm again

		if (report_failed_BY_THE_ALGORITHM_DURING_INITIALIZATION>0)       break;
		if (report_failed_BY_THE_ALGORITHM>0)                             break;
		if (report_failed_REACHED_TRIAL_LIMIT_DURING_INITIALIZATION>0)    break;
		if (report_discard_REACHED_TIME_LIMIT>0)                          break;

	}

	if (streamline->status == STREAMLINE_GOOD) {
		if      ((streamline->terminationReason_sideA==MAX_LENGTH_REACHED)      || (streamline->terminationReason_sideB==MAX_LENGTH_REACHED)     )
			report_success_REACHED_MAXLENGTH_LIMIT++;
		else if ((streamline->terminationReason_sideA==MIN_DATASUPPORT_REACHED) || (streamline->terminationReason_sideB==MIN_DATASUPPORT_REACHED))
			report_success_REACHED_MINDATASUPPORT_LIMIT++;
		else
			report_success_SATISFIED_PATHWAY_RULES++;
	}

	streamline->tracking_tries = tries;
	trialNo 				  += tries;
}

StreamlineStatus TrackingThread::run(bool side) {

	int countWriter 	= 0;
	int stepCounter     = 0;    
    
	if (side == true) {
		method->append();
    }

	ROI_Rule_Decision roiDecision 	= checkPathway();
    
    if ( (OUTPUT::dontWriteSegmentsInSeedROI==WRITE_ON) && (TRACKER::directionality==ONE_SIDED) && (SEED::seedingMode==SEED_IMAGE) && (isInsideSeedROI()==DONTAPPEND) ) {
        tracker_SEED->entry_status = entered;
        tracker_SEED->exit_status  = notExited;
        method->removeLast();
    }

	while (roiDecision==CONTINUE_TRACKING) {

		if (GENERAL::runTime()>GENERAL::timeLimit) {
			timeIsUp 						= true;
			streamline->discardingReason 	= REACHED_TIME_LIMIT;
			return STREAMLINE_DISCARDED;
		}

		Propagation_Decision propDecision = method->propagate(++stepCounter);

		if (propDecision==FAIL){
			streamline->failingReason = DISCARDED_BY_THE_ALGORITHM;
			return STREAMLINE_FAIL;
		} else if (propDecision==STOP) {

			if       (tracker_side==side_A) streamline->terminationReason_sideA = MIN_DATASUPPORT_REACHED;
			else if  (tracker_side==side_B) streamline->terminationReason_sideB = MIN_DATASUPPORT_REACHED;
			else if  (streamline->terminationReason_sideA==TERMINATIONREASON_NOTSET) streamline->terminationReason_sideA = MIN_DATASUPPORT_REACHED;
			else streamline->terminationReason_sideB = MIN_DATASUPPORT_REACHED;

			roiDecision = STOP_TRACKING;
			break;
		}
		
        if ( (TRACKER::directionality==ONE_SIDED) && (SEED::seedingMode==SEED_IMAGE) && (OUTPUT::dontWriteSegmentsInSeedROI==WRITE_ON) && (tracker_SEED->exit_status == notExited) && (isInsideSeedROI()==APPEND) ) {
            method->append();     // Make a dummy append to update seed coordinate
            streamline->seed_coordinates = streamline->coordinates.back();
            method->removeLast(); // Revert back
            countWriter = 0;      // Initialize countWrite to begin writing at correct intervals
        }
        


		// Increment first so that the first element after initialization is not written
		// since it already written after initialization
		if (++countWriter % TRACKER::writeInterval == 0) {

			method->append();

			if (tracker_FOD->checkWorldBounds(streamline->coordinates.back().x,streamline->coordinates.back().y,streamline->coordinates.back().z)==0) {
				method->removeLast();
				if   (tracker_side==side_A) streamline->terminationReason_sideA = MIN_DATASUPPORT_REACHED;
				else 						streamline->terminationReason_sideB = MIN_DATASUPPORT_REACHED;
				roiDecision = STOP_TRACKING;
				break;
			}

			if (streamline->getLength() > TRACKER::maxLength) {

				if (TRACKER::atMaxLength==ATMAXLENGTH_DISCARD) {
					streamline->discardingReason = TOO_LONG;
					roiDecision = DISCARD_STREAMLINE;
				} else { // TRACKER::atMaxLength==ATMAXLENGTH_STOP
					if   (tracker_side==side_A) streamline->terminationReason_sideA = MAX_LENGTH_REACHED;
					else 						streamline->terminationReason_sideB = MAX_LENGTH_REACHED;
					roiDecision = STOP_TRACKING;
				}

				break;
			}

			roiDecision = checkPathway();
            
            // Skip appending of streamline if requested
            if ( (OUTPUT::dontWriteSegmentsInSeedROI) && (TRACKER::directionality==ONE_SIDED) && (SEED::seedingMode==SEED_IMAGE) ) {
                switch (isInsideSeedROI()) {
                    case DONTAPPEND: {
                        method->removeLast();
                        break;
                    }
                    case REENTERED: {
                        streamline->discardingReason = REENTERED_SEED_ROI;
                        roiDecision = DISCARD_STREAMLINE;
                        break;
                    }
                    case APPEND: {
                        break; // DO NOTHING DIFFERENT
                    }
                }
            }
            
		}
		

	}

	if (roiDecision==DISCARD_STREAMLINE)
		return STREAMLINE_DISCARDED;

	// Discard if streamline ends inside a region where is not allowed
	for (std::vector<ROI_Image*>::iterator it = tracker_ROI.begin(); it != tracker_ROI.end(); it++) {
		if ((*it)->type == roi_type_discard_if_ends_inside) {
			float val 		= (*it)->getVal(streamline->coordinates.back());
			if (val>=0.5) {
				if ( ((*it)->side==side_undefined) || (tracker_side==side_undefined) || ( ((tracker_side==side_A) && ((*it)->side==side_A)) || ((tracker_side==side_B) && ((*it)->side==side_B)) ) ) {
					streamline->discardingReason = ENDED_INSIDE_DISCARD_ROI;
					return STREAMLINE_DISCARDED;
				}
			}

		}
	}

	return STREAMLINE_GOOD;

}
