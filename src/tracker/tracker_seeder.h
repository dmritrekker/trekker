#ifndef SRC_TRACKER_SEEDER_H_
#define SRC_TRACKER_SEEDER_H_

#include "tracker_thread.h"

void TrackingThread::getSeed(Coordinate* point) {
	seed_coordinates 			= *point;
	seed_init_direction 		= tracker_randomThings->getAUnitRandomVector();
}

void TrackingThread::getSeed() {
	switch (SEED::seedingMode) {

	case SEED_IMAGE: {
		if (SEED::countPerVoxel != NOTSET) {
			seed_coordinates 	= tracker_SEED->ind2phy(SEED::seed_indices.at(seedNo%(SEED::seed_indices.size())));
			tracker_randomThings->randomizeWithinVoxel(&seed_coordinates,tracker_SEED->getNim()->pixdim);
		}
		else {
			bool seedNotSet = true;
			while (seedNotSet) {
				seed_coordinates 	= tracker_SEED->ind2phy(SEED::seed_indices.at(tracker_randomThings->uniform_int()));
				tracker_randomThings->randomizeWithinVoxel(&seed_coordinates,tracker_SEED->getNim()->pixdim);
				if (tracker_SEED->getVal(seed_coordinates)>=0.5)
					seedNotSet = false;
			}
		}

		seed_init_direction 		= tracker_randomThings->getAUnitRandomVector();
		break;
	}

	case SEED_COORDINATES: {
		seed_coordinates 			= SEED::seed_coordinates.at(seedNo);
		seed_init_direction 		= tracker_randomThings->getAUnitRandomVector();
        if (seed_radius>0) {
            float x,y,z;
            tracker_randomThings->getARandomPointWithinSphere(&x,&y,&z,seed_radius);
            seed_coordinates.x += x;
            seed_coordinates.y += y;
            seed_coordinates.z += z;
        }
		break;
	}

	case SEED_COORDINATES_WITH_DIRECTIONS: {
		seed_coordinates 			= SEED::seed_coordinates.at(seedNo);
		seed_init_direction 		= SEED::seed_init_directions.at(seedNo);
		break;
	}

	default: break;

	}

}



#endif
