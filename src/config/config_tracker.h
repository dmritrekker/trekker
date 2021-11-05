#ifndef SRC_CONFIG_CONFIG_TRACKER_H_
#define SRC_CONFIG_CONFIG_TRACKER_H_

#include "../image/fod_image.h"
#include "../image/scalar_image.h"
#include "config_general.h"

#define SHPRECOMPUTEDIM 1024
#define PTFCOEFFMATRIXSIZE 501

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
	ATMAXLENGTH_NOTSET = -1,
	ATMAXLENGTH_STOP,
	ATMAXLENGTH_DISCARD
} AtMaxLength;

typedef enum {
	ATINIT_NOTSET = -1,
	ATINIT_USEBEST,
	ATINIT_REJECTIONSAMPLE
} AtInit;

typedef enum {
	ORDEROFDIRECTIONS_NOTSET = -1,
    XYZ,XYz,XyZ,Xyz,xYZ,xYz,xyZ,xyz,
    XZY,XZy,XzY,Xzy,xZY,xZy,xzY,xzy,
    YXZ,YXz,YxZ,Yxz,yXZ,yXz,yxZ,yxz,
    YZX,YZx,YzX,Yzx,yZX,yZx,yzX,yzx,
    ZYX,ZYx,ZyX,Zyx,zYX,zYx,zyX,zyx,
    ZXY,ZXy,ZxY,Zxy,zXY,zXy,zxY,zxy
} OrderOfDirections;

typedef enum {
	FODDISC_NOTSET = -1,
	FODDISC_OFF,
	FODDISC_ON
} fodDiscretizationMode;

typedef enum {
	CHECKWEAKLINKS_NOTSET = -1,
	CHECKWEAKLINKS_OFF,
	CHECKWEAKLINKS_ON
} checkWeakLinksMode;

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
extern bool            useMinFODampImage;
extern int             maxEstInterval;
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
extern float           probeStepSize;
extern float           probeNormalizer;
extern float           angularSeparation;


extern SCALAR_Image   *img_minFODamp;

// Derived parameters
extern float		   maxCurvature;
extern float		   smallestPixDim;
extern int 		       posteriorMaxEstimationInterval;
extern int 		       triesPerRejectionSampling;
extern float           writeStepSize;
extern float           maxEstStepSize;
extern float           posteriorMaxEstimationStepSize;
extern float           modMinFodAmp;
extern AtInit 		   atInit;

extern OrderOfDirections     orderOfDirections;
extern std::string           orderOfDirectionsTextInput;
extern fodDiscretizationMode fodDiscretization;
extern checkWeakLinksMode    checkWeakLinks;
extern float                 weakLinkThresh;

// Functions
void cleanConfigTracker();
void readFODImage();
void readMinFODampImage();
void setDefaultParametersWhenNecessary();
void setMethodsDefaultParametersWhenNecessary();
void print();



}

#endif
