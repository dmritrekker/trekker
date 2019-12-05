#ifndef SRC_CONFIG_CONFIG_TRACKER_H_
#define SRC_CONFIG_CONFIG_TRACKER_H_

#include "../image/fod_image.h"
#include "config_general.h"

class TractographyAlgorithm;
class Tractogram;

typedef enum {
	ALGORITHM_NOTSET = -1,
	PTT,
	LOCAL_PROBABILISTIC
} Algorithm;

typedef enum {
	DIRECTIONALITY_NOTSET = -1,
	ONE_SIDED,          			// one_sided
	TWO_SIDED               		// two_sided
} Directionality;

typedef enum {
	NEIGHBORHOODMODE_NOTSET = 0,
	SPHERE,            				// use points in sphere
	WALK_ZERO_RADIUS,       		// walk along a single 1D line
	WALK_NONZERO_RADIUS     		// walk along 4 different 1D lines
} neighborhoodMode;

typedef enum {
	ATMAXLENGTH_NOTSET = -1,
	ATMAXLENGTH_STOP,
	ATMAXLENGTH_DISCARD
} AtMaxLength;

typedef enum {
	ATINIT_NOTSET = -1,
	ATINIT_USEBEST,
	ATINIT_REJECTIONSAMPLE
} AtInit;

namespace TRACKER {

extern Algorithm       			algorithm;
extern bool 		   			defaultsSet;
extern TractographyAlgorithm   *method;
extern Tractogram   		   *tractogram;


// User inputs
extern FOD_Image 	  *img_FOD;
extern float 		   stepSize;
extern float 		   minRadiusOfCurvature;
extern float		   minFODamp;
extern float		   dataSupportExponent;
extern float		   minLength;
extern float		   maxLength;
extern AtMaxLength 	   atMaxLength;
extern int 		       writeInterval;
extern Directionality  directionality;
extern int			   initMaxEstTrials;
extern int			   propMaxEstTrials;

extern float 		   probeLength;
extern float 		   probeRadius;
extern int 			   probeCount;
extern int 		       probeQuality;

// Derived parameters
extern float		   maxCurvature;
extern float		   smallestPixDim;
extern float		   varCurvature;
extern bool 		   priorComputationFlag;
extern float		   varStraighten;
extern bool 		   straighteningComputationFlag;
extern float 		   gaussianScalingFactor;
extern float 		   gaussianPeakLocationFactor;
extern int 		       posteriorMaxEstimationInterval;
extern int 		       triesPerRejectionSampling;
extern float           writeStepSize;
extern float           posteriorMaxEstimationStepSize;
extern AtInit 		   atInit;

extern neighborhoodMode neighborhoodSamplingMode;



// Functions
void cleanConfigTracker();
void readFODImage();
void setDefaultParametersWhenNecessary();
void print();

}

#endif
