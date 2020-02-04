#ifndef SRC_TRACKER_ALGORITHMS_PTT_FAST_PTF_DISCRETIZER_H_
#define SRC_TRACKER_ALGORITHMS_PTT_FAST_PTF_DISCRETIZER_H_

#include "../../../config/config_tracker.h"
#include <stddef.h>
#include <vector>

namespace PTF_CONSTS {

extern size_t  k_resolution;
extern float   k_step;

extern float** curve_consts;
extern float** probe_step_consts;

extern float   probe_cum_length;
extern float   probeNormalizer;
extern float   angularSeparation;
extern size_t  numelk;
extern size_t  numelkq;

extern std::vector<size_t> valid_indices;
extern size_t  validIndexCount;

extern bool    isCleaned;
extern bool    isReady;

void precomputePTFCoefficients(size_t _k_resolution);
void cleanPTFCoefficients();

}

#endif
