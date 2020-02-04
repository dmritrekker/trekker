#include "config_tracker.h"
#include "../base/tractogram.h"

#include "../math/sphericalHarmonics.h"
#include "../base/algorithm.h"

#include "../tracker/algorithms/ptt/algorithm_ptt.h"
#include "../tracker/algorithms/ptt/tractogram_ptt.h"
#include "../tracker/algorithms/local_probabilistic/algorithm_local_probabilistic.h"
#include "../tracker/algorithms/local_probabilistic/tractogram_local_probabilistic.h"

using namespace GENERAL;
using namespace SH;

namespace TRACKER {

FOD_Image* img_FOD            		    = new FOD_Image;
Algorithm      		   algorithm        = ALGORITHM_NOTSET;
bool 				   defaultsSet 		= false;
TractographyAlgorithm *method 			= NULL;
Tractogram            *tractogram 		= NULL;

// Common tracking parameters
float stepSize       					= NOTSET;
float minRadiusOfCurvature   			= NOTSET;
float minRadiusOfTorsion   			    = NOTSET;
float minFODamp      					= NOTSET;
int   maxEstInterval                    = NOTSET;
float dataSupportExponent      			= NOTSET;
float minLength      					= NOTSET;
float maxLength      					= NOTSET;
AtMaxLength atMaxLength 				= ATMAXLENGTH_NOTSET;
Directionality directionality 			= DIRECTIONALITY_NOTSET;
int   writeInterval  					= NOTSET;
int	  initMaxEstTrials 					= NOTSET;
int   propMaxEstTrials    				= NOTSET;

// PTT parameters
float probeLength    					= NOTSET;
float probeRadius    					= NOTSET;
int   probeQuality   					= NOTSET;
int   probeCount 						= NOTSET;
float probeStepSize                     = NOTSET;
float probeNormalizer                   = NOTSET;
float angularSeparation                 = NOTSET;


// Derived parameters
float maxCurvature   					= NOTSET;
float maxTorsion   					    = NOTSET;
float writeStepSize                  	= NOTSET;
float maxEstStepSize                    = NOTSET;
float smallestPixDim 					= 0.0;
float varCurvature   					= NOTSET;
float varStraighten   					= NOTSET;
bool priorComputationFlag 				= true;
bool straighteningComputationFlag 		= true;
float gaussianScalingFactor             = NOTSET;
float gaussianPeakLocationFactor        = NOTSET;
int posteriorMaxEstimationInterval 		= NOTSET;
int triesPerRejectionSampling      		= NOTSET;
float posteriorMaxEstimationStepSize 	= NOTSET;
AtInit atInit 							= ATINIT_NOTSET;

OrderOfDirections       orderOfDirections           = ORDEROFDIRECTIONS_NOTSET;
std::string             orderOfDirectionsTextInput;
fodDiscretizationMode   fodDiscretization           = FODDISC_NOTSET;
checkWeakLinksMode      checkWeakLinks              = CHECKWEAKLINKS_NOTSET;

void cleanConfigTracker() {

    img_FOD->cleanFODImage();
    delete img_FOD;
    
	SH::clean();
	delete method;
	delete tractogram;
    
}

void setDefaultParametersWhenNecessary() {

	smallestPixDim     = img_FOD->getSmallestPixdim();

    // FOD fodDiscretization
    if (fodDiscretization==FODDISC_NOTSET) {
        fodDiscretization = FODDISC_ON;
        img_FOD->discretizationFlag = true;
    }
    
    // Check weak checkWeakLinks
    if (checkWeakLinks==CHECKWEAKLINKS_NOTSET) {
        checkWeakLinks = CHECKWEAKLINKS_OFF;
    }
    
    // Handle OrderOfDirections
    if (orderOfDirections==ORDEROFDIRECTIONS_NOTSET) {
        orderOfDirections=XYZ;
        if (GENERAL::verboseLevel>MINIMAL) std::cout << "Using order of directions    : XYZ " << std::endl;
    }

	// Handle algorithm
	if (algorithm==ALGORITHM_NOTSET) {
		algorithm = PTT_C2;
		if (GENERAL::verboseLevel>MINIMAL) std::cout << "Using default algorithm      : PTT " << std::endl;
	}

	switch (TRACKER::algorithm) {
	case PTT_C1:
    case PTT_C2:
    case PTT_C3:
		tractogram 								= new Tractogram_PTT();
		method 									= new  TrackWith_PTT();
		break;
	case LOCAL_PROBABILISTIC:
		tractogram 								= new Tractogram_Local_Probabilistic();
		method 									= new  TrackWith_Local_Probabilistic();
		break;
	default :
		break;
	}
    
    setMethodsDefaultParametersWhenNecessary();
    
    SH::precompute(SHPRECOMPUTEDIM);
    if (img_FOD->isspheresliced) {
        SH::precomputeExpansionCoefficients();
    }
    
	defaultsSet = true;
}

void setMethodsDefaultParametersWhenNecessary() {
    method->setDefaultParametersWhenNecessary();
}

void readFODImage() {
	if (GENERAL::verboseLevel!=QUITE) std::cout << "Reading FOD image                  : " << img_FOD->getFilePath() << std::endl;
	if(!img_FOD->readImage()) exit(EXIT_FAILURE);
}


void print() {
	std::cout << std::endl;
    if (GENERAL::usingAPI==false) {
        std::cout << "TRACKER OPTIONS"<< std::endl;
    }

	method->print();

	if (GENERAL::verboseLevel>ON) std::cout << std::endl << "-----------------" << std::endl;
	std::cout << "fod                  : "  << img_FOD->getFilePath()  << std::endl;
	if (GENERAL::verboseLevel>ON) img_FOD->printInfo();
    
    if (TRACKER::fodDiscretization==FODDISC_ON)
        std::cout << "fodDiscretization    : ON"  << std::endl;
    else {
        std::cout << "fodDiscretization    : OFF"  << std::endl;
    }
    
    if (TRACKER::orderOfDirectionsTextInput=="")
        std::cout << "orderOfDirections    : XYZ"  << std::endl;
    else {
        std::cout << "orderOfDirections    : "  << TRACKER::orderOfDirectionsTextInput  << std::endl;
    }
    
	if (GENERAL::verboseLevel>ON) std::cout << "-----------------" << std::endl << std::endl;
}

}
