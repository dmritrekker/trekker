#pragma once

#include "base/nibr.h"
#include "seeder.h"

namespace NIBR {

class SeedImage : public Seeder {

public:

	SeedImage() {};
	~SeedImage() {if (doRandomThings!=NULL) {delete[] doRandomThings; doRandomThings=NULL;}}

	virtual SeederOutputState getSeed(float* p, int threadNo);
    virtual SeederOutputState getSeed(float* p, float* dir, int threadNo);
    virtual void computeSeedCountAndDensity();
    virtual void computeMaxPossibleSeedCount();
    virtual void setNumberOfThreads(int n);

    // Image
    virtual bool setSeed(Image<int8_t>*);
    virtual bool setSeed(Image<int>*,int);
    virtual bool setSeed(Image<float>*);
    virtual bool setSeed(Image<float>*,int);

    // Sphere
    virtual bool setSeed(float,float,float,float) {return false;}
	virtual bool setSeed(Point,float) {return false;}
	virtual bool setSeed(float*,float) {return false;}

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

    Image<float>      *seed_img_pvf;
	Image<int8_t>      seed_img_mask; // Data in image is not used. Only the seed_indices are used. Image is created for coordinate conversions.
    std::vector<int>   seed_indices;
    float              max4rs;
    int                volInd{-1};

	RandomDoer        *doRandomThings{NULL};
    
};
}