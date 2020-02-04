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
        std::cout << "TREKKER::Cannot read FOD image: " << pathToFODimage << std::endl;
    } else {
        
        GENERAL::setDefaultParametersWhenNecessary();
        SEED::setDefaultParametersWhenNecessary();
        TRACKER::setDefaultParametersWhenNecessary();

        std::cout << "TREKKER::Reading FOD image: " << pathToFODimage << std::endl;TRACKER::defaultsSet = true;
        TRACKER::readFODImage();
        
    }
    
    // Sampling is different when wrappers are used
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
    
    
    // Some checks for the wrappers
    if (GENERAL::usingAPI) {
        if (SEED::seedingMode==SEED_NOTSET) {
            std::cout << "TREKKER::Please input seed image or coordinates first" << std::endl << std::flush;
            return;
        }
        
        TRACKER::setMethodsDefaultParametersWhenNecessary();
        
        TRACKER::tractogram->reset();
    }
    //-----------------------------

	timeUp 				 = false;
	int  seedNo 		 = 0;

   	lineCountToFlush = 1;
    
	if (GENERAL::verboseLevel!=QUITE) {
		std::cout << "--------------------" << std::endl;
		std::cout << "Tracking" << std::endl << std::endl;;
		TRACKER::tractogram->printSummary();
	}

	int         numberOfThreadsToUse = ( (GENERAL::numberOfThreads<=SEED::count) ? GENERAL::numberOfThreads : SEED::count);
	
    std::thread             *threads = new std::thread[numberOfThreadsToUse];
	TrackingThread          *tracker = new TrackingThread[numberOfThreadsToUse];
	int                 finalThreads = 0;
    
    std::unique_lock<std::mutex> lk(GENERAL::exit_mx);
    
	for(seedNo=0; seedNo<numberOfThreadsToUse; seedNo++) {
		int threadNo = seedNo;
        
        tracker[threadNo].setThreadID(threadNo);
		tracker[threadNo].updateSeedNoAndTrialCount(seedNo,0);
        
        threads[threadNo] = std::thread(getStreamline, (tracker+threadNo));
        threads[threadNo].detach();
	}
    
	while(seedNo<SEED::count) {
    
        GENERAL::exit_cv.wait(lk);
        int tread_id = ready_thread_id;
        
		// timeUp case
		if ((tracker[tread_id].streamline->status==STREAMLINE_DISCARDED) && (tracker[tread_id].streamline->discardingReason==REACHED_TIME_LIMIT)) {
			finalThreads = 1; // 1 thread is done tracking
			timeUp 		 = true;
			break;
		} else if (tracker[tread_id].streamline->status==STREAMLINE_GOOD)
			tracker[tread_id].updateSeedNoAndTrialCount(seedNo++,0);
		else if (tracker[tread_id].streamline->tracking_tries>(unsigned int)SEED::maxTrialsPerSeed)
			tracker[tread_id].updateSeedNoAndTrialCount(seedNo++,0);
        
        threads[tread_id] = std::thread(getStreamline, (tracker+tread_id));
		threads[tread_id].detach();
        
        GENERAL::tracker_lock.unlock();
	}
	
	while (finalThreads<numberOfThreadsToUse) {
        
        GENERAL::exit_cv.wait(lk);
        int tread_id = ready_thread_id;

		// timeUp case
		if ((tracker[tread_id].streamline->status==STREAMLINE_DISCARDED) && (tracker[tread_id].streamline->discardingReason==REACHED_TIME_LIMIT)) {
			finalThreads++;
			timeUp = true;
		} else if (tracker[tread_id].streamline->status==STREAMLINE_GOOD) {
			finalThreads++;
		} else if (tracker[tread_id].streamline->tracking_tries>(unsigned int)SEED::maxTrialsPerSeed) {
			finalThreads++;
		} else {
            threads[tread_id] = std::thread(getStreamline, (tracker+tread_id));
            threads[tread_id].detach();
        }
        
        GENERAL::tracker_lock.unlock();        
	}

	delete[] threads;
	delete[] tracker;
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


void Trekker::printParameters() {
    TRACKER::print();
    SEED::print();
}

// Parameter setting

// Global config
void Trekker::numberOfThreads(int n) { GENERAL::numberOfThreads = n;}
void Trekker::timeLimit(int t) { GENERAL::timeLimit = t;}


// Tracker config
void Trekker::checkWeakLinks(bool q) { TRACKER::checkWeakLinks = q ? CHECKWEAKLINKS_ON : CHECKWEAKLINKS_OFF; }
void Trekker::stepSize(double _stepSize) { TRACKER::stepSize = _stepSize;}

void Trekker::minRadiusOfCurvature(double x) { TRACKER::minRadiusOfCurvature = x;}
void Trekker::minFODamp(double x) { TRACKER::minFODamp = x; }
void Trekker::maxEstInterval(int n) { TRACKER::maxEstInterval = n; }
void Trekker::dataSupportExponent(double x) { TRACKER::dataSupportExponent = x; }
void Trekker::minLength(double x) { TRACKER::minLength = x; }
void Trekker::maxLength(double x) { TRACKER::maxLength = x; }
void Trekker::atMaxLength(std::string aml) {
    
    if (aml=="discard")
        TRACKER::atMaxLength = ATMAXLENGTH_DISCARD;
    else if (aml=="stop")
        TRACKER::atMaxLength = ATMAXLENGTH_STOP;
    else {
        std::cout << "TREKKER::Unknown option for atMaxLength. It can be either \"discard\" or \"stop\"" << std::endl << std::flush;
        std::cout << "TREKKER::Trekker will continue using the existing setting:";
        if (TRACKER::atMaxLength == ATMAXLENGTH_DISCARD)
            std::cout << "discard";
        else
            std::cout << "stop";
        std::cout << std::endl << std::flush;
    }
    
}

void Trekker::writeInterval(int n) { TRACKER::writeInterval = n; }
void Trekker::directionality(std::string d) { 
    
    if (d=="two_sided")
        TRACKER::directionality = TWO_SIDED;
    else if (d=="one_sided")
        TRACKER::directionality = ONE_SIDED;
    else {
        std::cout << "TREKKER::Unknown option for directionality. It can be either \"one_sided\" or \"two_sided\"" << std::endl << std::flush;
        std::cout << "TREKKER::Trekker will continue using the existing setting:";
        if (TRACKER::directionality == TWO_SIDED)
            std::cout << "two_sided";
        else
            std::cout << "one_sided";
        std::cout << std::endl << std::flush;
    }

}

void Trekker::maxSamplingPerStep(int n) { TRACKER::triesPerRejectionSampling = n; }
void Trekker::initMaxEstTrials(int n) {TRACKER::initMaxEstTrials=n;}
void Trekker::propMaxEstTrials(int n) {TRACKER::propMaxEstTrials=n;}
void Trekker::useBestAtInit(bool q) {TRACKER::atInit = q ? ATINIT_USEBEST : ATINIT_REJECTIONSAMPLE;}

void Trekker::probeLength(double x) { TRACKER::probeLength = x;}
void Trekker::probeRadius(double x) { TRACKER::probeRadius = x;}
void Trekker::probeCount(int n) { TRACKER::probeCount  = n;}
void Trekker::probeQuality(int n) { TRACKER::probeQuality = n;}


// Seed config
void Trekker::seed_image(std::string pathToSeedImage) {
    
    int n = pathToSeedImage.length();
	char* char_array = new char[n+1];
	strcpy(char_array, pathToSeedImage.c_str());
    
    ROI_Image*  test = new ROI_Image;
    if(!test->readHeader(char_array)) {
        std::cout << "TREKKER::Cannot read seed image: " << char_array << std::endl;
    } else {
        delete img_SEED;
        img_SEED = new ROI_Image;
        SEED::img_SEED->readHeader(char_array);
        SEED::seedingMode = SEED_IMAGE;
        SEED::setDefaultParametersWhenNecessary();
        SEED::readSeedImage();
    }
    delete test;
    
}

void Trekker::seed_image_using_label(std::string pathToSeedImage, int label) {
    
    int n = pathToSeedImage.length();
	char* char_array = new char[n+1];
	strcpy(char_array, pathToSeedImage.c_str());
    
    ROI_Image*  test = new ROI_Image;
    if(!test->readHeader(char_array)) {
        std::cout << "TREKKER::Cannot read seed image: " << char_array << std::endl;
    } else {
        delete img_SEED;
        img_SEED = new ROI_Image;
        SEED::img_SEED->readHeader(char_array);
        SEED::seedingMode = SEED_IMAGE;
        SEED::img_SEED->setLabel(label);
        SEED::setDefaultParametersWhenNecessary();
        SEED::readSeedImage();
    }
    delete test;
    
}

void Trekker::seed_coordinates(std::vector< std::vector<double> > seed_coordinates) {

	SEED::seedingMode 	= SEED_COORDINATES;
    
	SEED::count 		= seed_coordinates.size();

    SEED::seed_coordinates.clear();
	for (unsigned int i=0; i<seed_coordinates.size(); i++) {
		Coordinate tmp(seed_coordinates[i][0],seed_coordinates[i][1],seed_coordinates[i][2]);
		SEED::seed_coordinates.push_back(tmp);
	}
	
	SEED::setDefaultParametersWhenNecessary();

}

void Trekker::seed_coordinates_with_directions(std::vector< std::vector<double> > seed_coordinates,std::vector< std::vector<double> > seed_init_directions) {

    if (seed_coordinates.size()!=seed_init_directions.size()) {
        std::cout << "TREKKER::Sizes of seed coordinates and directions do not match" << std::endl << std::flush;
        return;
    }
    
	SEED::seedingMode 	= SEED_COORDINATES_WITH_DIRECTIONS;
    
	SEED::count 		= seed_coordinates.size();
    
	SEED::seed_coordinates.clear();
	for (unsigned int i=0; i<seed_coordinates.size(); i++) {
		Coordinate tmp(seed_coordinates[i][0],seed_coordinates[i][1],seed_coordinates[i][2]);
		SEED::seed_coordinates.push_back(tmp);
	}
	
	SEED::seed_init_directions.clear();
	for (unsigned int i=0; i<seed_init_directions.size(); i++) {
		Coordinate tmp(seed_init_directions[i][0],seed_init_directions[i][1],seed_init_directions[i][2]);
		SEED::seed_init_directions.push_back(tmp);
	}
	
	SEED::setDefaultParametersWhenNecessary();

}

void Trekker::seed_count(int n) { SEED::count = (n==0) ? NOTSET : n; SEED::setDefaultParametersWhenNecessary(); }
void Trekker::seed_countPerVoxel(int n) { SEED::countPerVoxel = (n==0) ? NOTSET : n; SEED::setDefaultParametersWhenNecessary(); }
void Trekker::seed_maxTrials(int n) { SEED::maxTrialsPerSeed = n;}
