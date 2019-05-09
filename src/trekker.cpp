#include "tracker/tracker_thread.h"

#ifdef ENABLE_MULTITHREADING

void* getStreamline(void *_tracker) {
	TrackingThread* tracker 	= ((TrackingThread *)_tracker);
	tracker->track(NULL);
	pthread_mutex_lock(&GENERAL::lock);
	tracker->isReady 	= true;
	tracker->updateTractogram();
	sem_post(&GENERAL::exit_sem);
	pthread_exit(NULL);
	return NULL;
}

#endif


int main (int argc, char **argv) {

	InputParser input(argc, argv);

	bool timeUp 		= false;
	int  seedNo 		= 0;

	int lineCountToFlush = 1;

	if (GENERAL::verboseLevel!=QUITE) {
		std::cout << "--------------------" << std::endl;
		std::cout << "Tracking" << std::endl << std::endl;;
		TRACKER::tractogram->printSummary(lineCountToFlush);
	}

#ifdef ENABLE_MULTITHREADING
	int         numberOfThreadsToUse = ( (GENERAL::numberOfThreads<=SEED::count) ? GENERAL::numberOfThreads : SEED::count);
	pthread_t               *threads = new pthread_t[numberOfThreadsToUse];
	TrackingThread          *tracker = new TrackingThread[numberOfThreadsToUse];
	int                 finalThreads = 0;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_mutex_init(&GENERAL::lock, NULL);

	sem_init (&GENERAL::exit_sem, 0, 0);
	for(seedNo=0; seedNo<numberOfThreadsToUse; seedNo++){
		int threadNo = seedNo;
		tracker[threadNo].updateSeedNoAndTrialCount(seedNo,0);
		pthread_create(threads+threadNo, &attr, getStreamline, (void *)(tracker+threadNo));
	}

	while(seedNo<SEED::count) {

		sem_wait (&GENERAL::exit_sem);

		int threadNo;
		for (threadNo=0; threadNo<numberOfThreadsToUse; threadNo++)
			if (tracker[threadNo].isReady==true) {
				if (GENERAL::verboseLevel!=QUITE) TRACKER::tractogram->printSummary(lineCountToFlush);
				tracker[threadNo].isReady=false;
				pthread_mutex_unlock(&GENERAL::lock);
				break;
			}

		// timeUp case
		if ((tracker[threadNo].streamline->status==STREAMLINE_DISCARDED) && (tracker[threadNo].streamline->discardingReason==REACHED_TIME_LIMIT)) {
			finalThreads = 1; // 1 thread is done tracking
			timeUp 		 = true;
			break;
		}

		if (tracker[threadNo].streamline->status==STREAMLINE_GOOD)
			tracker[threadNo].updateSeedNoAndTrialCount(seedNo++,0);
		else if (tracker[threadNo].streamline->tracking_tries>(unsigned int)SEED::maxTrialsPerSeed)
			tracker[threadNo].updateSeedNoAndTrialCount(seedNo++,0);

		pthread_create(threads+threadNo, &attr, getStreamline, (void *)(tracker+threadNo));
	}

	while (finalThreads<numberOfThreadsToUse) {

		sem_wait (&GENERAL::exit_sem);

		int threadNo;
		for (threadNo=0; threadNo<numberOfThreadsToUse; threadNo++)
			if (tracker[threadNo].isReady==true) {
				if (GENERAL::verboseLevel!=QUITE) TRACKER::tractogram->printSummary(lineCountToFlush);
				tracker[threadNo].isReady=false;
				pthread_mutex_unlock(&GENERAL::lock);
				break;
			}

		// timeUp case
		if ((tracker[threadNo].streamline->status==STREAMLINE_DISCARDED) && (tracker[threadNo].streamline->discardingReason==REACHED_TIME_LIMIT)) {
			finalThreads++;
			timeUp = true;
		} else if (tracker[threadNo].streamline->status==STREAMLINE_GOOD) {
			finalThreads++;
		} else if (tracker[threadNo].streamline->tracking_tries>(unsigned int)SEED::maxTrialsPerSeed) {
			finalThreads++;
		} else
			pthread_create(threads+threadNo, &attr, getStreamline, (void *)(tracker+threadNo));

	}


	pthread_mutex_destroy(&GENERAL::lock);
	delete[] threads;
	delete[] tracker;

#else
	TrackingThread *tracker = new TrackingThread;
	int trialNo = 0;

	do {
		tracker->updateSeedNoAndTrialCount(seedNo,trialNo);
		tracker->track(NULL);
		tracker->updateTractogram();
		if (GENERAL::verboseLevel!=QUITE) TRACKER::tractogram->printSummary(lineCountToFlush);

		if ((tracker->streamline->status==STREAMLINE_DISCARDED) && (tracker->streamline->discardingReason==REACHED_TIME_LIMIT)) {
			timeUp = true;
			break;
		} else if (tracker->streamline->status!=STREAMLINE_GOOD) {

			trialNo = int(tracker->trialNo);

			if (trialNo>maxTrialsPerSeed) {
				seedNo++;
				trialNo = 0;
			}

		} else {
			seedNo++;
			trialNo = 0;
		}

	} while (seedNo<SEED::count);


	delete tracker;
#endif

	if (GENERAL::verboseLevel!=QUITE) {
		std::cout << std::endl << "--------------------" << std::endl;

		if (timeUp)
			std::cout << std::endl << "Time is up!" << std::endl;

		std::cout << std::endl << "Writing output to " << OUTPUT::outputFilePath << std::endl;
	}
	TRACKER::tractogram->writeOutput();
	TRACKER::tractogram->writeMetadataOutput();

	if (GENERAL::verboseLevel!=QUITE)
		std::cout << "Done!" << std::endl;

	return EXIT_SUCCESS;

}
