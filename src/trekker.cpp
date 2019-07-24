#include "trekker.h"
#include "tracker/tracker_thread.h"

using namespace GENERAL;
using namespace TRACKER;
using namespace SEED;
using namespace PATHWAY;
using namespace OUTPUT;

Trekker::Trekker(int argc, char **argv) {
	InputParser input(argc, argv);

	timeUp = false;

	run();

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
}

Trekker::Trekker(std::string pathToFODimage) {

	int n = pathToFODimage.length();
	char* char_array = new char[n+1];
	strcpy(char_array, pathToFODimage.c_str());

	GENERAL::usingAPI 		= true;
	GENERAL::verboseLevel 	= QUITE;

	if(!img_FOD->readHeader(char_array)) {
		std::cout << "Cannot read FOD image: " << pathToFODimage << std::endl;
	}

	GENERAL::setDefaultParametersWhenNecessary();
	SEED::setDefaultParametersWhenNecessary();
	SEED::readSeedImage();
	TRACKER::setDefaultParametersWhenNecessary();
	OUTPUT::setDefaultParametersWhenNecessary();

	std::cout << "Reading FOD image: " << pathToFODimage << std::endl;
	TRACKER::readFODImage();

	// With below defined, tractogram update will not change how sampling is done
	// this will allow each streamline to be tracked the same way
	TRACKER::initMaxEstTrials = 50;
	TRACKER::propMaxEstTrials = 20;

	timeUp = false;


}

Trekker::~Trekker() {

}

void Trekker::set_seed_maxTrials(int n) {
	SEED::maxTrialsPerSeed = n;
}

void Trekker::set_numberOfThreads(int n) {

#ifdef ENABLE_MULTITHREADING
	GENERAL::numberOfThreads 		 = n;
#else
	std::cout << "Cannot set numberOfThreads to " << n << " since trekker is compiled without multithreading and numberOfThreads=1" << std::endl;
#endif

}

void Trekker::set_seeds(std::vector< std::vector<double> > seed_coordinates) {

	SEED::seedingMode 	= SEED_COORDINATES;
	SEED::count 		= seed_coordinates.size();
	SEED::seed_coordinates.clear();

	for (unsigned int i=0; i<seed_coordinates.size(); i++) {
		Coordinate tmp(seed_coordinates[i][0],seed_coordinates[i][1],seed_coordinates[i][2]);
		SEED::seed_coordinates.push_back(tmp);
	}

}


void Trekker::execute() {

	TRACKER::tractogram->reset();

	timeUp 				= false;
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



}

std::vector< std::vector< std::vector<double> > > Trekker::get_tractogram_coordinates() {

	std::vector<Streamline*> streamlines = TRACKER::tractogram->streamlines;

	std::vector< std::vector< std::vector<double> > > coordinates;

	for (size_t i=0; i<TRACKER::tractogram->streamlineCount; i++)
		if (streamlines[i]->status == STREAMLINE_GOOD) {
			std::vector<std::vector<double>> tmp;
			std::vector<double> x;
			std::vector<double> y;
			std::vector<double> z;
			for (std::vector<Coordinate>::iterator it = streamlines[i]->coordinates.begin(); it != streamlines[i]->coordinates.end(); ++it) {
				x.push_back(it->x);
				y.push_back(it->y);
				z.push_back(it->z);
			}
			tmp.push_back(x);
			tmp.push_back(y);
			tmp.push_back(z);
			coordinates.push_back(tmp);
		}

	return coordinates;
}

std::vector< std::vector< std::vector<double> > > Trekker::run() {

	execute();
	return get_tractogram_coordinates();
}
