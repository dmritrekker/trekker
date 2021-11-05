#include "./algorithm_local_probabilistic.h"
#include "./algorithm_local_probabilistic_defaults.h"

void TrackWith_Local_Probabilistic::setDefaultParametersWhenNecessary() {

	// Handle stepSize
	if (TRACKER::stepSize<=0.0) {
		TRACKER::stepSize = TRACKER::smallestPixDim*DEFAULT_LOCAL_PROBABILISTIC_STEPSIZE_IN_PIXELDIM;
		if (GENERAL::verboseLevel>MINIMAL) std::cout << "Using default stepSize      : " << TRACKER::stepSize << img_FOD->getUnit() << std::endl;
	}

	// Handle minRadiusOfCurvature
	if (TRACKER::minRadiusOfCurvature<=0.0) {
		TRACKER::minRadiusOfCurvature = TRACKER::smallestPixDim*DEFAULT_LOCAL_PROBABILISTIC_MINRADIUSOFCURVATURE_IN_PIXELDIM;
		if (GENERAL::verboseLevel>MINIMAL) std::cout << "Using default minRadiusOfCurvature      : " << TRACKER::minRadiusOfCurvature << img_FOD->getUnit() << std::endl;
	}

	TRACKER::maxCurvature = 1/TRACKER::minRadiusOfCurvature;
	if (TRACKER::maxCurvature<1e-4) {
		TRACKER::maxCurvature = 1e-4;
		if (GENERAL::verboseLevel!=QUITE) std::cout << "minRadiusOfCurvature is very small" << std::endl;
	}

	// Handle minFODamp
	if ((TRACKER::minFODamp<0.0) && (GENERAL::verboseLevel>MINIMAL)) std::cout << "Using default minFODamp     : 0" << std::endl;
	if (TRACKER::minFODamp<=0.0) TRACKER::minFODamp = DEFAULT_LOCAL_PROBABILISTIC_MINFODAMP; // so comparisons are faster

	currMinFODamp = TRACKER::minFODamp;

	// Handle writeStepSize
	if (TRACKER::writeInterval<=0.0) {
		TRACKER::writeStepSize = TRACKER::smallestPixDim*DEFAULT_LOCAL_PROBABILISTIC_WRITEINTERVAL_IN_PIXELDIM;
		TRACKER::writeInterval = ceil(TRACKER::writeStepSize/TRACKER::stepSize);
		if (GENERAL::verboseLevel>MINIMAL) std::cout << "Using default writeInterval : " << TRACKER::writeInterval << std::endl;
	}

	TRACKER::writeStepSize = TRACKER::writeInterval*TRACKER::stepSize;
	if (GENERAL::verboseLevel>ON) std::cout << "   effective written step size is : " << TRACKER::writeStepSize << img_FOD->getUnit() << std::endl;

	// Handle minLength
	if ((minLength<0.0) && (GENERAL::verboseLevel>MINIMAL)) std::cout << "Using default minLength     : 0 " << img_FOD->getUnit() << std::endl;
	if (minLength<=0.0) minLength = DEFAULT_LOCAL_PROBABILISTIC_MINLENGTH; // so comparisons are faster

	// Handle maxLength
	if ((TRACKER::maxLength<=minLength) || (TRACKER::maxLength>DEFAULT_LOCAL_PROBABILISTIC_MAXLENGTH)){
		TRACKER::maxLength = DEFAULT_LOCAL_PROBABILISTIC_MAXLENGTH;
		if (GENERAL::verboseLevel>MINIMAL) std::cout << "Using default maxLength     : infinite " << img_FOD->getUnit() << std::endl;
	}

	// Handle atMaxLength
	if (atMaxLength==ATMAXLENGTH_NOTSET) {
		atMaxLength = ATMAXLENGTH_DISCARD;
		if (GENERAL::verboseLevel>MINIMAL) std::cout << "Using default atMaxLength    : discard " << std::endl;
	}

	// Handle directionality
	if (TRACKER::directionality==DIRECTIONALITY_NOTSET) {
		TRACKER::directionality = TWO_SIDED;
		if (GENERAL::verboseLevel>MINIMAL)  std::cout << "Using default directionality: two_sided" << std::endl;
	}

	// Handle triesPerRejectionSampling
	if (TRACKER::triesPerRejectionSampling==NOTSET) {
		TRACKER::triesPerRejectionSampling = DEFAULT_LOCAL_PROBABILISTIC_TRIESPERREJECTIONSAMPLING;
		if (GENERAL::verboseLevel>ON) std::cout << "triesPerRejectionSampling : " << TRACKER::triesPerRejectionSampling << std::endl;
	}

	// Handle atInit
	if (TRACKER::atInit==ATINIT_NOTSET) {
		TRACKER::atInit = ATINIT_REJECTIONSAMPLE;
		if (GENERAL::verboseLevel>ON) std::cout << "useBestATinit             : OFF" << std::endl;
	}

}

void TrackWith_Local_Probabilistic::print() {

	std::cout << "algorithm            : local_probabilistic"   << std::endl;

	std::cout << "stepSize             : "  << TRACKER::stepSize 			 << TRACKER::img_FOD->getUnit() << std::endl;
	std::cout << "minRadiusOfCurvature : "  << TRACKER::minRadiusOfCurvature << TRACKER::img_FOD->getUnit() << std::endl;

	if (TRACKER::useMinFODampImage) {
		std::cout << "minFODamp            : "  << TRACKER::img_minFODamp->getFilePath() << std::endl;
	} else {
		std::cout << "minFODamp            : "  << TRACKER::minFODamp << std::endl;
	}

	std::cout << "minLength            : ";
	if (TRACKER::minLength<(DEFAULT_LOCAL_PROBABILISTIC_MINLENGTH*1.01))	std::cout 		<< "0" 							<< std::endl;
	else 								std::cout << TRACKER::minLength 					<< TRACKER::img_FOD->getUnit() 	<< std::endl;

	std::cout << "maxLength            : ";
	if (TRACKER::maxLength>(DEFAULT_LOCAL_PROBABILISTIC_MAXLENGTH*0.99))	std::cout 		<< "infinite " << img_FOD->getUnit() 	<< std::endl;
	else 								std::cout << TRACKER::maxLength 					<< TRACKER::img_FOD->getUnit() 			<< std::endl;

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

	std::cout << "writeInterval        : "  << TRACKER::writeInterval 						<< std::endl;
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
