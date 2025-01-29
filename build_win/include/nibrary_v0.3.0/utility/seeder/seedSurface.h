#pragma once

#include "base/nibr.h"
#include "seeder.h"

namespace NIBR {

class SeedSurface : public Seeder {

public:

	SeedSurface() {};
	~SeedSurface() {if (doRandomThings!=NULL) {delete[] doRandomThings; doRandomThings=NULL;}}

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
    virtual bool setSeed(Surface*);
    virtual bool setSeed(Surface*,float) {return false;}

    // Surface options
    virtual bool useDensity   (std::vector<float>&);
    virtual void useSurfNorm  (bool);

private:

	Surface             *seed_surf;
    std::vector<double>  faces_vec_dens;

    bool                 surfNorm;
    bool                 useDensInp;

	RandomDoer          *doRandomThings{NULL};

    std::vector<int>     nonZeroFaces;
    std::vector<double>  cdf;
    double               totalDensity;
    void                 computeCDF();
    
};

}

