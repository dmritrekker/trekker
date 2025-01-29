#pragma once

#include "base/nibr.h"
#include "seeder.h"

namespace NIBR {

class SeedInsideSurface : public Seeder {

public:

	SeedInsideSurface() {};
	~SeedInsideSurface();

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
    virtual bool setSeed(float,float,float,float) {return false;}
	virtual bool setSeed(Point,float) {return false;}
	virtual bool setSeed(float*,float) {return false;}

    // List
    virtual bool setSeed(std::vector<Point>&) {return false;}
	virtual bool setSeed(std::vector<Point>&,std::vector<Point>&) {return false;}

    // Surface
    virtual bool setSeed(Surface*) {return false;}
    virtual bool setSeed(Surface*,float);

    // Surface options
    virtual bool useDensity   (std::vector<float>&) {return false;}
    virtual void useSurfNorm  (bool) {return;}

private:

    Surface         *seed_surf;
    std::vector<int> seed_indices;
	RandomDoer      *doRandomThings{NULL};
    
};

}