#pragma once

#include "base/nibr.h"
#include "base/vectorOperations.h"
#include "../tractographyAlgorithm.h"

namespace NIBR {

class PTF;

class TrackWith_PTT : public TractographyAlgorithm {

public:

	TrackWith_PTT();
	~TrackWith_PTT();

	virtual Propagation_Decision initialize();
	virtual Propagation_Decision propagate();
	virtual Propagation_Decision flip();

	virtual void  setSeed();
	virtual void  reset();
	virtual void  append();
	virtual float writeStepSize();
	virtual int   appendInterval();

	float                       outputStep{NAN};
	float                       stepSize{NAN};

	float                       minRadiusOfCurvature{NAN};
	float                       minDataSupport{NAN};
	float                       dataSupportExponent{NAN};

	int                      	maxEstInterval{-1};
	int                       	initMaxEstTrials{-1};
	int                       	propMaxEstTrials{-1};
	int                       	triesPerRejectionSampling{-1};

	float               		probeCount{NAN};
    float                		probeRadius{NAN};
    float               		probeLength{NAN};
	float                		probeQuality{NAN};

	float                		maxCurvature{NAN};
	float 				 		modMinDataSupport{NAN};

	std::vector<std::array<int,3>>*		 cdfFace;
	std::vector<std::pair<float,float>>* cdfk1k2;

private:

	PTF *curve;
	PTF *initial_curve;

	void 					fetchParams(float *p);
	void 	 				estimatePosteriorMax();
	Propagation_Decision 	rejectionSample();
	Propagation_Decision 	sampleFromCDF();
	void                    setCDF();

	float 	 posteriorMax;
	int      stepCounter;

	std::vector<float> cdfVertVal; // Data support for each vertex of the k1-k2 disc
	std::vector<float> cdf;        // The cumulative distribution function (evaluated on faces)	

	// This tracker's parameters
	bool                        parametersAreReady{false};

};

}