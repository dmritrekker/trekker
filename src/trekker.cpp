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
        
    } else {
        
        GENERAL::setDefaultParametersWhenNecessary();
        SEED::setDefaultParametersWhenNecessary();
        SEED::readSeedImage();
        TRACKER::setDefaultParametersWhenNecessary();
        OUTPUT::setDefaultParametersWhenNecessary();

        std::cout << "Reading FOD image: " << pathToFODimage << std::endl;
        TRACKER::readFODImage();
        
    }
    
    // With below defined, tractogram update will not change how sampling is done
	// this will allow each streamline to be tracked the same way
	TRACKER::initMaxEstTrials = 50;
	TRACKER::propMaxEstTrials = 20;

	timeUp = false;
}

Trekker::~Trekker() {
    if (GENERAL::usingAPI==true) {
        TRACKER::cleanConfigTracker();
        SEED::cleanConfigSeeding();
        PATHWAY::cleanConfigROI();
    }
}

void Trekker::execute() {

	TRACKER::tractogram->reset();

	timeUp 				 = false;
	int  seedNo 		 = 0;

	int lineCountToFlush = 1;

	if (GENERAL::verboseLevel!=QUITE) {
		std::cout << "--------------------" << std::endl;
		std::cout << "Tracking" << std::endl << std::endl;;
		TRACKER::tractogram->printSummary(lineCountToFlush);
	}

	int         numberOfThreadsToUse = ( (GENERAL::numberOfThreads<=SEED::count) ? GENERAL::numberOfThreads : SEED::count);
	
    std::thread             *threads = new std::thread[numberOfThreadsToUse];
	TrackingThread          *tracker = new TrackingThread[numberOfThreadsToUse];
	int                 finalThreads = 0;
 
	// sem_init (&GENERAL::exit_sem, 0, 0);
 
	for(seedNo=0; seedNo<numberOfThreadsToUse; seedNo++) {
		int threadNo = seedNo;
		tracker[threadNo].updateSeedNoAndTrialCount(seedNo,0);
        
        threads[threadNo] = std::thread(getStreamline, (tracker+threadNo));
        threads[threadNo].detach();
	}
    
    std::unique_lock<std::mutex> lk(GENERAL::exit_mx);
	while(seedNo<SEED::count) {

		// sem_wait (&GENERAL::exit_sem);
        GENERAL::exit_cv.wait(lk);

		int threadNo;
		for (threadNo=0; threadNo<numberOfThreadsToUse; threadNo++)
			if (tracker[threadNo].isReady==true) {
				if (GENERAL::verboseLevel!=QUITE) TRACKER::tractogram->printSummary(lineCountToFlush);
				tracker[threadNo].isReady=false;
                GENERAL::tracker_lock.unlock();
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
        
        threads[threadNo] = std::thread(getStreamline, (tracker+threadNo));
		threads[threadNo].detach();
        
	}

	while (finalThreads<numberOfThreadsToUse) {

		// sem_wait (&GENERAL::exit_sem);
        GENERAL::exit_cv.wait(lk);

		int threadNo;
		for (threadNo=0; threadNo<numberOfThreadsToUse; threadNo++)
			if (tracker[threadNo].isReady==true) {
				if (GENERAL::verboseLevel!=QUITE) TRACKER::tractogram->printSummary(lineCountToFlush);
				tracker[threadNo].isReady=false;
                GENERAL::tracker_lock.unlock();
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
		} else {
            threads[threadNo] = std::thread(getStreamline, (tracker+threadNo));
            threads[threadNo].detach();
        }

	}

	delete[] threads;
	delete[] tracker;
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

// Parameter setting

void Trekker::set_numberOfThreads(int n) {
	GENERAL::numberOfThreads = n;
}

void Trekker::set_timeLimit(int t) {
    GENERAL::timeLimit = t;
}

void Trekker::set_seed_maxTrials(int n) {
	SEED::maxTrialsPerSeed = n;
}

void Trekker::set_minFODamp(double _minFODamp) {
	TRACKER::minFODamp = _minFODamp;
}

void Trekker::set_stepSize(double _stepSize) {
	TRACKER::stepSize = _stepSize;
}

void Trekker::set_minRadiusOfCurvature(double x) {
	TRACKER::minRadiusOfCurvature = x;
}

void Trekker::set_minLength(double x) {
	TRACKER::minLength = x;
}

void Trekker::set_maxLength(double x) {
	TRACKER::maxLength = x;
}

void Trekker::set_writeInterval(int n) {
	TRACKER::writeInterval = n;
}

void Trekker::set_probeLength(double x) {
	TRACKER::probeLength = x;
}

void Trekker::set_probeRadius(double x) {
	TRACKER::probeRadius = x;
}

void Trekker::set_probeCount(int n) {
	TRACKER::probeCount = n;
}

void Trekker::set_probeQuality(int n) {
	TRACKER::probeQuality = n;
}
