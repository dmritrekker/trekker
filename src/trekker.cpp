#include "trekker.h"
#include "tracker/tracker_thread.h"

using namespace GENERAL;
using namespace TRACKER;
using namespace SEED;
using namespace PATHWAY;
using namespace OUTPUT;

bool isInitialized() { return GENERAL::initialized; }

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

// Reset parameters
void resetAllParameters() {
    
    // Clean existing SEED and PATHWAY options
    SEED::cleanConfigSeeding();
    PATHWAY::cleanConfigROI();
    
    // General config
    GENERAL::usingAPI 		            = true;
    GENERAL::verboseLevel               = QUITE;
    GENERAL::timeLimit                  = NOTSET;
    GENERAL::numberOfThreads            = NOTSET;
    
    // Tracker config
    if (img_FOD->discretizationFlag == false) TRACKER::orderOfDirections = ORDEROFDIRECTIONS_NOTSET;
    TRACKER::algorithm                  = ALGORITHM_NOTSET;
    TRACKER::stepSize                   = NOTSET;
    TRACKER::minRadiusOfCurvature   	= NOTSET;
    TRACKER::minFODamp      			= NOTSET;
    TRACKER::dataSupportExponent      	= NOTSET;
    TRACKER::maxEstInterval             = NOTSET;
    TRACKER::minLength      			= NOTSET;
    TRACKER::maxLength      			= NOTSET;
    TRACKER::atMaxLength 				= ATMAXLENGTH_NOTSET;
    TRACKER::writeInterval  			= NOTSET;
    TRACKER::directionality 			= DIRECTIONALITY_NOTSET;
    TRACKER::triesPerRejectionSampling  = NOTSET;
    TRACKER::initMaxEstTrials 			= 50; // Sampling is different when wrappers are used. With this defined, tractogram update will not change how sampling is done
    TRACKER::propMaxEstTrials    		= 20; // Sampling is different when wrappers are used. With this defined, tractogram update will not change how sampling is done
    TRACKER::atInit 					= ATINIT_NOTSET;
    TRACKER::probeLength    			= NOTSET;
    TRACKER::probeRadius    			= NOTSET;
    TRACKER::probeQuality   			= NOTSET;
    TRACKER::probeCount 			    = NOTSET;
    TRACKER::checkWeakLinks             = CHECKWEAKLINKS_NOTSET;
    TRACKER::weakLinkThresh             = NOTSET;    
    
    // Seed config
    SEED::seedingMode                   = SEED_NOTSET;
    SEED::count 					    = NOTSET;
    SEED::countPerVoxel 			    = NOTSET;
    SEED::maxTrialsPerSeed 		        = NOTSET;
    
    // Set default parameters
    GENERAL::setDefaultParametersWhenNecessary();
    SEED::setDefaultParametersWhenNecessary();
    TRACKER::defaultsSet = false;
    TRACKER::setDefaultParametersWhenNecessary();
    TRACKER::defaultsSet = true;
    
}

void changeOrderOfDirections(std::string ood) {
    
    TRACKER::orderOfDirectionsTextInput=ood;
    
    if      (ood=="")    TRACKER::orderOfDirections = XYZ;
    else if (ood=="XYZ") TRACKER::orderOfDirections = XYZ;
    else if (ood=="XYz") TRACKER::orderOfDirections = XYz;
    else if (ood=="XyZ") TRACKER::orderOfDirections = XyZ;
    else if (ood=="Xyz") TRACKER::orderOfDirections = Xyz;
    else if (ood=="xYZ") TRACKER::orderOfDirections = xYZ;
    else if (ood=="xYz") TRACKER::orderOfDirections = xYz;
    else if (ood=="xyZ") TRACKER::orderOfDirections = xyZ;
    else if (ood=="xyz") TRACKER::orderOfDirections = xyz;
	else if (ood=="XZY") TRACKER::orderOfDirections = XZY;
    else if (ood=="XZy") TRACKER::orderOfDirections = XZy;
    else if (ood=="XzY") TRACKER::orderOfDirections = XzY;
    else if (ood=="Xzy") TRACKER::orderOfDirections = Xzy;
    else if (ood=="xZY") TRACKER::orderOfDirections = xZY;
    else if (ood=="xZy") TRACKER::orderOfDirections = xZy;
    else if (ood=="xzY") TRACKER::orderOfDirections = xzY;
    else if (ood=="xzy") TRACKER::orderOfDirections = xzy;
    else if (ood=="YXZ") TRACKER::orderOfDirections = YXZ;
    else if (ood=="YXz") TRACKER::orderOfDirections = YXz;
    else if (ood=="YxZ") TRACKER::orderOfDirections = YxZ;
    else if (ood=="Yxz") TRACKER::orderOfDirections = Yxz;
    else if (ood=="yXZ") TRACKER::orderOfDirections = yXZ;
    else if (ood=="yXz") TRACKER::orderOfDirections = yXz;
    else if (ood=="yxZ") TRACKER::orderOfDirections = yxZ;
    else if (ood=="yxz") TRACKER::orderOfDirections = yxz;
    else if (ood=="YZX") TRACKER::orderOfDirections = YZX;
    else if (ood=="YZx") TRACKER::orderOfDirections = YZx;
    else if (ood=="YzX") TRACKER::orderOfDirections = YzX;
    else if (ood=="Yzx") TRACKER::orderOfDirections = Yzx;
    else if (ood=="yZX") TRACKER::orderOfDirections = yZX;
    else if (ood=="yZx") TRACKER::orderOfDirections = yZx;
    else if (ood=="yzX") TRACKER::orderOfDirections = yzX;
    else if (ood=="yzx") TRACKER::orderOfDirections = yzx;
    else if (ood=="ZYX") TRACKER::orderOfDirections = ZYX;
    else if (ood=="ZYx") TRACKER::orderOfDirections = ZYx;
    else if (ood=="ZyX") TRACKER::orderOfDirections = ZyX;
    else if (ood=="Zyx") TRACKER::orderOfDirections = Zyx;
    else if (ood=="zYX") TRACKER::orderOfDirections = zYX;
    else if (ood=="zYx") TRACKER::orderOfDirections = zYx;
    else if (ood=="zyX") TRACKER::orderOfDirections = zyX;
    else if (ood=="zyx") TRACKER::orderOfDirections = zyx;
    else if (ood=="ZXY") TRACKER::orderOfDirections = ZXY;
    else if (ood=="ZXy") TRACKER::orderOfDirections = ZXy;
    else if (ood=="ZxY") TRACKER::orderOfDirections = ZxY;
    else if (ood=="Zxy") TRACKER::orderOfDirections = Zxy;
    else if (ood=="zXY") TRACKER::orderOfDirections = zXY;
    else if (ood=="zXy") TRACKER::orderOfDirections = zXy;
    else if (ood=="zxY") TRACKER::orderOfDirections = zxY;
    else if (ood=="zxy") TRACKER::orderOfDirections = zxy;
    else {
		std::cout << "TREKKER::Unknown order of directions: " << ood << ", valid options are e.g.\"xYz\", \"ZyX\" etc. "<< std::endl;
	}
    
}

void Trekker::resetParameters() { resetAllParameters(); }

void checkFOD(std::string pathToFODimage, bool discretizationFlag, bool sphericalFunctionFlag, std::string pathToSphericalDomain, std::string symasym ) {
    int n = pathToFODimage.length();
	char* char_array = new char[n+1];
	strcpy(char_array, pathToFODimage.c_str());

    if (img_FOD  == NULL) { img_FOD  = new FOD_Image; }
    if (img_SEED == NULL) { img_SEED = new ROI_Image; }
    
	if(!img_FOD->readHeader(char_array)) {
        
        std::cout << "TREKKER::Cannot read FOD image: " << pathToFODimage << std::endl;
        
    } else {

        std::cout << "TREKKER::Reading FOD image: " << pathToFODimage << std::endl << std::flush;
        
        if (discretizationFlag==false) {
            std::cout << "TREKKER::Discretization is off" << std::endl << std::flush;
            TRACKER::fodDiscretization  = FODDISC_OFF;
            img_FOD->discretizationFlag = false;
        } else {
            std::cout << "TREKKER::Discretization is on" << std::endl << std::flush;
            TRACKER::fodDiscretization  = FODDISC_ON;
            img_FOD->discretizationFlag = true;
        }
        
        img_FOD->isspheresliced = sphericalFunctionFlag;
        
        if (sphericalFunctionFlag) {
            
            // Check symmetry options
            if ((symasym!="sym") && (symasym!="asym")) {
                std::cout << "TREKKER::Please indicate if FOD is symmetric or asymmetric using options \"sym\" or \"asym\" " << std::endl << std::flush;
            } else if (symasym=="sym") {
                img_FOD->iseven = true;
            } else {
                img_FOD->iseven = false;
            }
            
            // Read spherical domain
            int ns = pathToSphericalDomain.length();
            char* psd = new char[ns+1];
            strcpy(psd, pathToSphericalDomain.c_str());
            
            std::string directions;
            std::ifstream sphere(psd);

            if (!sphere.good()) {
                std::cout << "TREKKER::Cannot read sphere vertices from " << psd << std::endl << std::flush;
            }

            int lineNo = 1;
            bool readError = false;
            float p[3];
            while(std::getline(sphere,directions)) {
                if (!directions.empty()) {
                    std::stringstream xyz(directions);
                    if (xyz.good()) xyz >> p[0]; else { readError = true; break; }
                    if (xyz.good()) xyz >> p[1]; else { readError = true; break; }
                    if (xyz.good()) xyz >> p[2]; else { readError = true; break; }
                    normalize(p);
                    img_FOD->inpSphCoords.push_back(Coordinate(p[0],p[1],p[2]));
                }
                lineNo++;
            }
            sphere.close();
            if (readError) {
                std::cout << "TREKKER::Cannot read sphere vertices from " << psd << ", line " << lineNo << std::endl << std::flush;
            }

            if ((img_FOD->nim->nt - img_FOD->inpSphCoords.size()) !=0 ) {
                std::cout << "TREKKER::Number of sphere vertices does not match the number of volumes in the FOD image" << std::endl << std::flush;
            }
            
            img_FOD->sphereFileName     = pathToSphericalDomain;
            
        }
        
        
        resetAllParameters();
        TRACKER::readFODImage();
        
    } 
    
    GENERAL::initialized = true;
}

Trekker::Trekker(std::string pathToFODimage) {
    changeOrderOfDirections("XYZ"); // If not set here, previous parameter in the namespace will be used since this parameter cannot be reset
    checkFOD(pathToFODimage, true, false, "", "" );
    timeUp = false;
}

Trekker::Trekker(std::string pathToFODimage, bool discretizationFlag) {
    changeOrderOfDirections("XYZ"); // If not set here, previous parameter in the namespace will be used since this parameter cannot be reset
    checkFOD(pathToFODimage, discretizationFlag, false, "", "" );
    timeUp = false;
}

Trekker::Trekker(std::string pathToFODimage, std::string ood) {
    changeOrderOfDirections(ood);
    checkFOD(pathToFODimage, true, false, "", "" );
    timeUp = false;
}

Trekker::Trekker(std::string pathToFODimage, std::string ood, bool discretizationFlag) {
    changeOrderOfDirections(ood);
    checkFOD(pathToFODimage, discretizationFlag, false, "", "" );
    timeUp = false;
}

Trekker::Trekker(std::string pathToFODimage, bool discretizationFlag, std::string ood) {
    Trekker(pathToFODimage,ood,discretizationFlag);
}


Trekker::Trekker(std::string pathToFODimage, std::string pathToSphericalDomain, std::string symasym) {
    changeOrderOfDirections("XYZ"); // If not set here, previous parameter in the namespace will be used since this parameter cannot be reset
    checkFOD(pathToFODimage, true, true, pathToSphericalDomain, symasym );
    timeUp = false;
}

Trekker::Trekker(std::string pathToFODimage, std::string pathToSphericalDomain, std::string symasym, bool discretizationFlag) {
    changeOrderOfDirections("XYZ"); // If not set here, previous parameter in the namespace will be used since this parameter cannot be reset
    checkFOD(pathToFODimage, discretizationFlag, true, pathToSphericalDomain, symasym );
    timeUp = false;
}

Trekker::Trekker(std::string pathToFODimage, std::string pathToSphericalDomain, std::string symasym, std::string ood) {
    changeOrderOfDirections(ood);
    checkFOD(pathToFODimage, true, true, pathToSphericalDomain, symasym );
    timeUp = false;
}

Trekker::Trekker(std::string pathToFODimage, std::string pathToSphericalDomain, std::string symasym, std::string ood, bool discretizationFlag) {
    changeOrderOfDirections(ood);
    checkFOD(pathToFODimage, discretizationFlag, true, pathToSphericalDomain, symasym );
    timeUp = false;
}

Trekker::Trekker(std::string pathToFODimage, std::string pathToSphericalDomain, std::string symasym, bool discretizationFlag, std::string ood) {
    Trekker(pathToFODimage,pathToSphericalDomain,symasym,ood,discretizationFlag);
}


Trekker::~Trekker() {
    
    if (GENERAL::usingAPI==true) {
        TRACKER::cleanConfigTracker();
        SEED::cleanConfigSeeding();
        PATHWAY::cleanConfigROI();        
    }
    
    GENERAL::initialized = false;
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
    GENERAL::print();
    TRACKER::print();
    SEED::print();
    PATHWAY::print();
}

// Parameter setting

// Global config
void Trekker::numberOfThreads(int n) { GENERAL::numberOfThreads = n;}
void Trekker::timeLimit(int t) { GENERAL::timeLimit = t;}


// Tracker config

void Trekker::orderOfDirections(std::string ood) {
    if (img_FOD->discretizationFlag == false)
        changeOrderOfDirections(ood);
    else
        std::cout << "TREKKER::Can't change order of directions since FOD is already discretized."<< std::endl;
}



void Trekker::algorithm(std::string alg) {
    
    if (alg=="ptt C1")      TRACKER::algorithm = PTT_C1;
    else if (alg=="ptt C2") TRACKER::algorithm = PTT_C2;
    else {
		std::cout << "TREKKER::Unknown algorithm: " << alg << ", valid options are \"ptt C1\" and \"ptt C2\". "<< std::endl;
	}
    
}

void Trekker::stepSize(double _stepSize) { TRACKER::stepSize = _stepSize;}
void Trekker::minRadiusOfCurvature(double x) { TRACKER::minRadiusOfCurvature = x;}
void Trekker::minFODamp(double x) { TRACKER::minFODamp = x; }
void Trekker::ignoreWeakLinks(double x) { TRACKER::weakLinkThresh = x; TRACKER::checkWeakLinks  = CHECKWEAKLINKS_ON; }
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

void se(std::string s, int l, bool q) {
    
    SEED::cleanConfigSeeding();
    
    int n   = s.length();
    char* f = new char[n+1];
    strcpy(f, s.c_str());
    
    ROI_Image*  test = new ROI_Image;
    if(!test->readHeader(f)) {
        std::cout << "TREKKER::Cannot read seed image: " << f << std::endl;
    } else {
        img_SEED = new ROI_Image;
        SEED::img_SEED->readHeader(f);
        SEED::seedingMode = SEED_IMAGE;
        if (q) SEED::img_SEED->setLabel(l);
        SEED::setDefaultParametersWhenNecessary();
        SEED::readSeedImage();
    }
    
    delete test;
    delete[] f;
    
}

void Trekker::seed_image(std::string s)         { se(s,0,false); }
void Trekker::seed_image(std::string s, int l)  { se(s,l,true);  }



void Trekker::seed_coordinates(std::vector< std::vector<double> > seed_coordinates) {

    SEED::cleanConfigSeeding();
    SEED::img_SEED               = new ROI_Image; // This is necessary for tracker_thread to work, this will be deleted at exit
	SEED::seedingMode 	         = SEED_COORDINATES;
	SEED::count 		         = seed_coordinates.size();
    SEED::seed_coordinate_fname  = "";

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
    
    SEED::cleanConfigSeeding();
    SEED::img_SEED                   = new ROI_Image; // This is necessary for tracker_thread to work, this will be deleted at exit
	SEED::seedingMode 	             = SEED_COORDINATES_WITH_DIRECTIONS;
	SEED::count 		             = seed_coordinates.size();
    SEED::seed_coordinate_fname      = "";
    SEED::seed_init_directions_fname = "";
    
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


// Pathway config

void Trekker::clearPathwayRules() {PATHWAY::cleanConfigROI();}

ROI_Image* checkPathway(std::string s) {
    
    int n   = s.length();
    char* f = new char[n+1];
    strcpy(f, s.c_str());
    
    ROI_Image *tmp = new ROI_Image;
    if(!tmp->readHeader(f)) {
        std::cout << "TREKKER::Cannot read pathway image: " << f << std::endl;
        delete[] f;
        return NULL;
    }
    delete[] f;
    return tmp;
}

void addPathway(ROI_Image* tmp, int l, bool q, Tracking_Side side) {
    if (q) tmp->setLabel(l);
    tmp->side = side;
    tmp->readImage();
    if (side == side_undefined) PATHWAY::order_of_ROIs.push_back(tmp);
    if (side == side_A)         PATHWAY::order_of_side_A_ROIs.push_back(tmp);
    if (side == side_B)         PATHWAY::order_of_side_B_ROIs.push_back(tmp);
    PATHWAY::img_ROI.push_back(tmp);
}


// require_entry
void pren(std::string s, int l, bool q, Tracking_Side side) {
    ROI_Image *tmp = checkPathway(s);
    if (tmp!=NULL) {
        tmp->type = roi_type_req_entry;
        addPathway(tmp,l,q,side);
    }
}

void Trekker::pathway_require_entry(std::string s)          { pren(s, 0, false, side_undefined); }
void Trekker::pathway_require_entry(std::string s, int l)   { pren(s, l, true,  side_undefined); }
void Trekker::pathway_A_require_entry(std::string s)        { pren(s, 0, false, side_A); }
void Trekker::pathway_A_require_entry(std::string s, int l) { pren(s, l, true,  side_A); }
void Trekker::pathway_B_require_entry(std::string s)        { pren(s, 0, false, side_B); }
void Trekker::pathway_B_require_entry(std::string s, int l) { pren(s, l, true,  side_B); }



// require_exit
void prex(std::string s, int l, bool q, Tracking_Side side) {
    ROI_Image *tmp = checkPathway(s);
    if (tmp!=NULL) {
        tmp->type = roi_type_req_exit;
        addPathway(tmp,l,q,side);
    }
}

void Trekker::pathway_require_exit(std::string s)          { prex(s, 0, false, side_undefined); }
void Trekker::pathway_require_exit(std::string s, int l)   { prex(s, l, true,  side_undefined); }
void Trekker::pathway_A_require_exit(std::string s)        { prex(s, 0, false, side_A); }
void Trekker::pathway_A_require_exit(std::string s, int l) { prex(s, l, true,  side_A); }
void Trekker::pathway_B_require_exit(std::string s)        { prex(s, 0, false, side_B); }
void Trekker::pathway_B_require_exit(std::string s, int l) { prex(s, l, true,  side_B); }




// stop_at_entry
void sten(std::string s, int l, bool q, Tracking_Side side) {
    ROI_Image *tmp = checkPathway(s);
    if (tmp!=NULL) {
        tmp->type = roi_type_stop_at_entry;
        addPathway(tmp,l,q,side);
    }
}

void Trekker::pathway_stop_at_entry(std::string s)          { sten(s, 0, false, side_undefined); }
void Trekker::pathway_stop_at_entry(std::string s, int l)   { sten(s, l, true,  side_undefined); }
void Trekker::pathway_A_stop_at_entry(std::string s)        { sten(s, 0, false, side_A); }
void Trekker::pathway_A_stop_at_entry(std::string s, int l) { sten(s, l, true,  side_A); }
void Trekker::pathway_B_stop_at_entry(std::string s)        { sten(s, 0, false, side_B); }
void Trekker::pathway_B_stop_at_entry(std::string s, int l) { sten(s, l, true,  side_B); }




// stop_at_exit
void stex(std::string s, int l, bool q, Tracking_Side side) {
    ROI_Image *tmp = checkPathway(s);
    if (tmp!=NULL) {
        tmp->type = roi_type_stop_at_exit;
        addPathway(tmp,l,q,side);
    }
}

void Trekker::pathway_stop_at_exit(std::string s)          { stex(s, 0, false, side_undefined); }
void Trekker::pathway_stop_at_exit(std::string s, int l)   { stex(s, l, true,  side_undefined); }
void Trekker::pathway_A_stop_at_exit(std::string s)        { stex(s, 0, false, side_A); }
void Trekker::pathway_A_stop_at_exit(std::string s, int l) { stex(s, l, true,  side_A); }
void Trekker::pathway_B_stop_at_exit(std::string s)        { stex(s, 0, false, side_B); }
void Trekker::pathway_B_stop_at_exit(std::string s, int l) { stex(s, l, true,  side_B); }




// discard_if_enters
void dien(std::string s, int l, bool q, Tracking_Side side) {
    ROI_Image *tmp = checkPathway(s);
    if (tmp!=NULL) {
        tmp->type = roi_type_discard_if_enters;
        addPathway(tmp,l,q,side);
    }
}

void Trekker::pathway_discard_if_enters(std::string s)          { dien(s, 0, false, side_undefined); }
void Trekker::pathway_discard_if_enters(std::string s, int l)   { dien(s, l, true,  side_undefined); }
void Trekker::pathway_A_discard_if_enters(std::string s)        { dien(s, 0, false, side_A); }
void Trekker::pathway_A_discard_if_enters(std::string s, int l) { dien(s, l, true,  side_A); }
void Trekker::pathway_B_discard_if_enters(std::string s)        { dien(s, 0, false, side_B); }
void Trekker::pathway_B_discard_if_enters(std::string s, int l) { dien(s, l, true,  side_B); }




// discard_if_exits
void diex(std::string s, int l, bool q, Tracking_Side side) {
    ROI_Image *tmp = checkPathway(s);
    if (tmp!=NULL) {
        tmp->type = roi_type_discard_if_exits;
        addPathway(tmp,l,q,side);
    }
}

void Trekker::pathway_discard_if_exits(std::string s)          { diex(s, 0, false, side_undefined); }
void Trekker::pathway_discard_if_exits(std::string s, int l)   { diex(s, l, true,  side_undefined); }
void Trekker::pathway_A_discard_if_exits(std::string s)        { diex(s, 0, false, side_A); }
void Trekker::pathway_A_discard_if_exits(std::string s, int l) { diex(s, l, true,  side_A); }
void Trekker::pathway_B_discard_if_exits(std::string s)        { diex(s, 0, false, side_B); }
void Trekker::pathway_B_discard_if_exits(std::string s, int l) { diex(s, l, true,  side_B); }





// discard_if_ends_inside
void diei(std::string s, int l, bool q, Tracking_Side side) {
    ROI_Image *tmp = checkPathway(s);
    if (tmp!=NULL) {
        tmp->type = roi_type_discard_if_ends_inside;
        addPathway(tmp,l,q,side);
    }
}

void Trekker::pathway_discard_if_ends_inside(std::string s)          { diei(s, 0, false, side_undefined); }
void Trekker::pathway_discard_if_ends_inside(std::string s, int l)   { diei(s, l, true,  side_undefined); }
void Trekker::pathway_A_discard_if_ends_inside(std::string s)        { diei(s, 0, false, side_A); }
void Trekker::pathway_A_discard_if_ends_inside(std::string s, int l) { diei(s, l, true,  side_A); }
void Trekker::pathway_B_discard_if_ends_inside(std::string s)        { diei(s, 0, false, side_B); }
void Trekker::pathway_B_discard_if_ends_inside(std::string s, int l) { diei(s, l, true,  side_B); }



// satisy_requirements_in_order

void Trekker::pathway_satisy_requirements_in_order(bool q) {
    if (q)
        PATHWAY::satisfy_requirements_in_order = IN_ORDER;
    else
        PATHWAY::satisfy_requirements_in_order = NO_ORDER;
}
