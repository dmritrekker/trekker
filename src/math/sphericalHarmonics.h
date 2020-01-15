#ifndef SRC_MATH_SPHERICALHARMONICS_H_
#define SRC_MATH_SPHERICALHARMONICS_H_

#include "../config/config_tracker.h"
#include "math_aux.h"

using namespace TRACKER;

namespace SH {

extern int 		sphericalHarmonicOrder;
extern int 		numberOfSphericalHarmonicCoefficients;

extern size_t   numberOfSamples_phi;
extern size_t   numberOfSamples_theta;

extern float    scalingFactor_phi;
extern float    scalingFactor_theta;

extern float   *precomputedPhiComponent;
extern float   *precomputedThetaComponent;

void  precompute(size_t num);
void  clean();
float SH_amplitude(float *values, float *unit_dir);

extern float   **Ylm; // Coefficients for SH expansion
void  precomputeExpansionCoefficients();

}

#endif
