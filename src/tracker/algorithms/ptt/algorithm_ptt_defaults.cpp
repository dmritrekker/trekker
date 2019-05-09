#include "algorithm_ptt.h"

void TrackWith_PTT::precomputeCurveConstants() {
	PTF_CONSTS::precomputePTFCoefficients(501);
}

void TrackWith_PTT::setDefaultParametersWhenNecessary() {

	// Handle stepSize
	if (TRACKER::stepSize<=0.0) {
		TRACKER::stepSize = TRACKER::smallestPixDim*DEFAULT_PTT_STEPSIZE_IN_PIXELDIM;
		if (GENERAL::verboseLevel>MINIMAL) std::cout << "Using default stepSize      : " << TRACKER::stepSize << img_FOD->getUnit() << std::endl;
	}

	// Handle minRadiusOfCurvature
	if (TRACKER::minRadiusOfCurvature<=0.0) {
		TRACKER::minRadiusOfCurvature = TRACKER::smallestPixDim*DEFAULT_PTT_MINRADIUSOFCURVATURE_IN_PIXELDIM;
		if (GENERAL::verboseLevel>MINIMAL) std::cout << "Using default minRadiusOfCurvature      : " << TRACKER::minRadiusOfCurvature << img_FOD->getUnit() << std::endl;
	}

	TRACKER::maxCurvature = 1/TRACKER::minRadiusOfCurvature;
	if (TRACKER::maxCurvature<1e-4) {
		TRACKER::maxCurvature = 1e-4;
		if (GENERAL::verboseLevel!=QUITE) std::cout << "minRadiusOfCurvature is very large" << std::endl;
	}

	// Set probeCount and probeRadius
	if (TRACKER::probeCount<1)   TRACKER::probeCount =NOTSET;
	if (TRACKER::probeRadius<0)  TRACKER::probeRadius=NOTSET;

	if (TRACKER::probeCount==NOTSET && TRACKER::probeRadius==NOTSET) {
		TRACKER::probeCount = DEFAULT_PTT_PROBECOUNT;
		TRACKER::probeRadius= DEFAULT_PTT_PROBERADIUS_IN_PIXELDIM*TRACKER::smallestPixDim;
	} else if (TRACKER::probeCount==NOTSET && TRACKER::probeRadius!=NOTSET) {
		TRACKER::probeCount = DEFAULT_PTT_PROBECOUNT_WHEN_THEREIS_PROBERADIUS;
	} else if (TRACKER::probeCount!=NOTSET && TRACKER::probeRadius==NOTSET) {
		TRACKER::probeRadius= DEFAULT_PTT_PROBERADIUS_IN_PIXELDIM*TRACKER::smallestPixDim;
	}

	if (TRACKER::probeCount==1) {
		TRACKER::probeRadius=0;
	} else {
		if (TRACKER::probeRadius==0) {
			TRACKER::probeCount=1;
			if (GENERAL::verboseLevel!=QUITE) std::cout << "Setting probeCount to 1 because probeRadius is 0"<< img_FOD->getUnit() << std::endl;
		} else {
			if (TRACKER::probeRadius>TRACKER::minRadiusOfCurvature) {
				TRACKER::probeRadius = TRACKER::minRadiusOfCurvature;
				if (GENERAL::verboseLevel!=QUITE) std::cout << "Setting probeRadius to      : " << TRACKER::probeRadius << img_FOD->getUnit() << std::endl;
				if (GENERAL::verboseLevel!=QUITE) std::cout << "        probeRadius can't be larger than minRadiusOfCurvature" << std::endl;
			}
		}
	}



	// Handle probeLength
	if (TRACKER::probeLength<=0.0) {
		TRACKER::probeLength = TRACKER::smallestPixDim*DEFAULT_PTT_PROBELENGTH_IN_PIXELDIM;
		if (GENERAL::verboseLevel>MINIMAL) std::cout << "Using default probeLength      : " << TRACKER::probeLength << img_FOD->getUnit() << std::endl;
	}

	// Handle probeQuality
	if ((TRACKER::probeQuality<=0.0) || (TRACKER::probeQuality>100)) {
		TRACKER::probeQuality = DEFAULT_PTT_PROBEQUALITY;
		if (GENERAL::verboseLevel>MINIMAL) std::cout << "Using default probeQuality      : " << TRACKER::probeQuality << std::endl;
	}

	// Handle minFODamp
	if (TRACKER::minFODamp<0.0) TRACKER::minFODamp = DEFAULT_PTT_MINFODAMP;

	// Handle writeStepSize
	if (TRACKER::writeInterval<=0.0) {
		TRACKER::writeStepSize = TRACKER::smallestPixDim*DEFAULT_PTT_WRITEINTERVAL_IN_PIXELDIM;
		TRACKER::writeInterval = ceil(TRACKER::writeStepSize/TRACKER::stepSize);
		if (GENERAL::verboseLevel>MINIMAL) std::cout << "Using default writeInterval : " << TRACKER::writeInterval << std::endl;
	}

	TRACKER::writeStepSize = TRACKER::writeInterval*TRACKER::stepSize;
	if (GENERAL::verboseLevel>ON) std::cout << "   effective written step size is : " << TRACKER::writeStepSize << img_FOD->getUnit() << std::endl;

	// Handle minLength
	if ((minLength<0.0) && (GENERAL::verboseLevel>MINIMAL)) std::cout << "Using default minLength     : 0 " << img_FOD->getUnit() << std::endl;
	if (minLength<=0.0) minLength = DEFAULT_PTT_MINLENGTH; // so comparisons are faster

	// Handle maxLength
	if ((TRACKER::maxLength<=minLength) || (TRACKER::maxLength>DEFAULT_PTT_MAXLENGTH)){
		TRACKER::maxLength = DEFAULT_PTT_MAXLENGTH;
		if (GENERAL::verboseLevel>MINIMAL) std::cout << "Using default maxLength     : infinite " << img_FOD->getUnit() << std::endl;
	}

	// Handle atMaxLength
	if (atMaxLength==ATMAXLENGTH_NOTSET) {
		atMaxLength = ATMAXLENGTH_DISCARD;
		if (GENERAL::verboseLevel>MINIMAL) std::cout << "Using default atMaxLength   : discard " << std::endl;
	}

	// Handle directionality
	if (TRACKER::directionality==DIRECTIONALITY_NOTSET) {
		TRACKER::directionality = TWO_SIDED;
		if (GENERAL::verboseLevel>MINIMAL)  std::cout << "Using default directionality: two_sided" << std::endl;
	}

	// Handle triesPerRejectionSampling
	if (TRACKER::triesPerRejectionSampling==NOTSET) {
		TRACKER::triesPerRejectionSampling = DEFAULT_PTT_TRIESPERREJECTIONSAMPLING;
		if (GENERAL::verboseLevel>ON) std::cout << "triesPerRejectionSampling : " << TRACKER::triesPerRejectionSampling << std::endl;
	}

	// Handle atInit
	if (TRACKER::atInit==ATINIT_NOTSET) {
		TRACKER::atInit = ATINIT_REJECTIONSAMPLE;
		if (GENERAL::verboseLevel>ON) std::cout << "useBestATinit             : OFF" << std::endl;
	}

	precomputeCurveConstants();
}

void TrackWith_PTT::print() {

	std::cout << "algorithm            : parallel transport tracker (PTT)"   	<< std::endl;

	std::cout << "stepSize             : "  << TRACKER::stepSize 			 	<< TRACKER::img_FOD->getUnit() << std::endl;
	std::cout << "minRadiusOfCurvature : "  << TRACKER::minRadiusOfCurvature 	<< TRACKER::img_FOD->getUnit() << std::endl;

	std::cout << "probeLength          : "  << TRACKER::probeLength 		    << TRACKER::img_FOD->getUnit() << std::endl;
	std::cout << "probeRadius          : "  << TRACKER::probeRadius 		    << TRACKER::img_FOD->getUnit() << std::endl;
	std::cout << "probeCount           : "  << TRACKER::probeCount	        	<< std::endl;
	std::cout << "probeQuality         : "  << TRACKER::probeQuality	        << std::endl;

	std::cout << "minFODamp            : "  << TRACKER::minFODamp 				<< std::endl;

	std::cout << "minLength            : ";
	if (TRACKER::minLength<(DEFAULT_PTT_MINLENGTH*1.01))	std::cout 			<< "0" 							<< std::endl;
	else 								std::cout << TRACKER::minLength 		<< TRACKER::img_FOD->getUnit() 	<< std::endl;

	std::cout << "maxLength            : ";
	if (TRACKER::maxLength>(DEFAULT_PTT_MAXLENGTH*0.99))	std::cout 			<< "infinite " << img_FOD->getUnit() 	<< std::endl;
	else 								std::cout << TRACKER::maxLength 		<< TRACKER::img_FOD->getUnit() 			<< std::endl;

	switch(TRACKER::atMaxLength) {
	case(ATMAXLENGTH_STOP):
			std::cout << "atMaxLength          : stop"   	<< std::endl;
	break;
	case(ATMAXLENGTH_DISCARD):
			std::cout << "atMaxLength          : discard"   << std::endl;
	break;
	case(ATMAXLENGTH_NOTSET):
		break;
	default:
		break;
	}

	std::cout << "writeInterval        : "  << TRACKER::writeInterval 			<< std::endl;
	std::cout << "directionality       : ";
	switch (TRACKER::directionality) {
	case ONE_SIDED:              std::cout << "one_sided"              << std::endl; break;
	case TWO_SIDED:              std::cout << "two_sided"              << std::endl; break;
	default: break;
	}

	std::cout << "maxSamplingPerStep   : "  << TRACKER::triesPerRejectionSampling 			<< std::endl;

	if (TRACKER::initMaxEstTrials==NOTSET)
		std::cout << "initMaxEstTrials     : adaptive " << std::endl;
	else
		std::cout << "initMaxEstTrials     : "  << TRACKER::initMaxEstTrials 				<< std::endl;

	if (TRACKER::propMaxEstTrials==NOTSET)
		std::cout << "propMaxEstTrials     : adaptive " << std::endl;
	else
		std::cout << "propMaxEstTrials     : "  << TRACKER::propMaxEstTrials 				<< std::endl;

	if (TRACKER::atInit==ATINIT_USEBEST)
		std::cout << "useBestATinit        : ON "  << std::endl;
	else
		std::cout << "useBestATinit        : OFF " << std::endl;

}
