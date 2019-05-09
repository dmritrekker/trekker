#ifndef SRC_CONFIG_CONFIG_SEEDING_H_
#define SRC_CONFIG_CONFIG_SEEDING_H_

#include <vector>
#include <iostream>

#include "../image/roi_image.h"
#include "config_general.h"

#define MAXNUMBEROFSEEDS				1e9
#define MAXTRIALSPERSEEDS				1e6

typedef enum {
	SEED_NOTSET = -1,
	SEED_IMAGE,
	SEED_COORDINATES,
	SEED_COORDINATES_WITH_DIRECTIONS
} SeedingMode;

namespace SEED {

extern ROI_Image                  *img_SEED;
extern std::vector<size_t>   	   seed_indices;

extern std::vector<Coordinate>     seed_coordinates;
extern std::vector<Coordinate>     seed_init_directions;

extern std::string     			   seed_coordinate_fname;
extern std::string     			   seed_init_directions_fname;

extern SeedingMode                 seedingMode;
extern int                         count;
extern int                         countPerVoxel;
extern int 			   			   maxTrialsPerSeed;

void cleanConfigSeeding();
void setDefaultParametersWhenNecessary();
void print();
void readSeedImage();

}

#endif
