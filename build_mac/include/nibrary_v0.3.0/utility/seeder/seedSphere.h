#pragma once

#include "base/nibr.h"
#include "seeder.h"

namespace NIBR {

class SeedSphere : public Seeder {

public:

	SeedSphere() {};
	~SeedSphere() {if (doRandomThings!=NULL) {delete[] doRandomThings; doRandomThings=NULL;}}

	virtual SeederOutputState getSeed(float* p, int threadNo);
    virtual SeederOutputState getSeed(float* p, float* dir, int threadNo);
	virtual void computeSeedCountAndDensity();
	virtual void computeMaxPossibleSeedCount();
    virtual void setNumberOfThreads(int n);

    // Image
    virtual bool setSeed(Image<int8_t>*) {return false;}
    virtual bool setSeed(Image<int>*,int)  {return false;}
    virtual bool setSeed(Image<float>*) {return false;}
    virtual bool setSeed(Image<float>*,int) {return false;}

    // Sphere
    virtual bool setSeed(float,float,float,float);
	virtual bool setSeed(Point,float);
	virtual bool setSeed(float*,float);

    // List
    virtual bool setSeed(std::vector<Point>&) {return false;}
	virtual bool setSeed(std::vector<Point>&,std::vector<Point>&) {return false;}

    // Surface
    virtual bool setSeed(Surface*) {return false;}
    virtual bool setSeed(Surface*,float) {return false;}

    // Surface options
    virtual bool useDensity   (std::vector<float>&) {return false;}
    virtual void useSurfNorm  (bool) {return;}

private:

	float seed_center[3]{0,0,0};
    float seed_radius{0};

	RandomDoer *doRandomThings{NULL};
    
};

}

