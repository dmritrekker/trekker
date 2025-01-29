#pragma once

// This class handles the seeding for tractography.
// The seed definion is actually done as a part of pathway rules with a seed typed rule.
// With the setSeed function, the seed is defined, i.e., used as input in this class.
// This class provides some additional functionality over the seed definition of the pathway rules.
// For example, seed count, density, density files, etc. can additionally be provided.
// After all the members are set/updated, getSeed functions can be used to get seeds for tractography.

#include "base/nibr.h"
#include "../../../utility/seeder/seedingMethods.h"
#include "../pathway/pathwayRule.h"
#include <cstdint>

#define SEEDMAXSCALE 1

namespace NIBR {

class Seed {

public:

    Seed() {};
    ~Seed() {
        clear();
    }


    bool setSeed(PathwayRule rule);

    SeedingMode getMode() {return seeder->mode;}

    SeederOutputState getSeed(float* p, int tID);
    SeederOutputState getSeed(float* p,float *dir, int tID);
    long getCount() {return seeder->count;};

    bool update();
    void clear();
    void reset() {seeder->reset();};
    void print();

    void setSeedCount  (long    _count) {sCount  =_count;   sHasDensity=false;}
    void setSeedDensity(float _density) {sDensity=_density; sHasDensity=true; }
    long getMaxTrackerCount()           {return seeder->maxPossibleSeedCount;}

    long  sCount        = INT64_MAX;
    float sDensity      = 0;
    bool  sHasDensity   = false;
    int   trials        = 1;

    // Used for setting the seed.
    std::string surf_faceDensity_filename{""};
    std::string surf_vertDensity_filename{""};
    std::string surf_densityFile_dataType{""};
    std::string surf_density_fieldname{""};

    bool        useSurfNorm{false};

private:

    bool    isReady{false};
    Seeder *seeder{NULL};

};

}
