#ifndef SRC_MATH_MATH_AUX_H_
#define SRC_MATH_MATH_AUX_H_

#include <iostream>
#include <cmath>

#define SQRT2	1.41421356237309504880168872420969807856967187537694807
#define PI 		3.14159265358979323846264338327950288419716939937510582
#define TWOPI	6.28318530717958647692528676655900576839433879875021164

namespace MATH_AUX {

}

void  normalize(float *vec);
float norm(float *vec);
float dot(float* A, float *B);
void  cross(float *out, float* A, float *B);
void  quaternion2Rotation(float *R, float *q);
void  rotate(float *out, float *p, float *R);

// This makes sure that SH computation never hits the boundary range of the precomputed coefficients
// which otherwise might cause a segmentation fault
// for example, when vec[0]=-1, in SH_amplitude this is used to find the precomputed index:
// (vec[0]+1)*scalingFactor -> Now, in some rare cases vec[0]+1 turns out to be -0.00000000000000000000000000001
// The (-) sign causes a segmentation fault
// For all practical purposes, below does not change the result
inline void verifyUnitRange(float *vec) {
	for (int i=0;i<3;i++){
		if (vec[i]> 1) vec[i]= 0.99999999999999999999999999999999999999;
		if (vec[i]<-1) vec[i]=-0.99999999999999999999999999999999999999;
	}
}

inline void normalize(float *vec) {
	float normi = 1.0/std::sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
	vec[0] *= normi;
	vec[1] *= normi;
	vec[2] *= normi;
}

inline float norm(float *vec) {
	return std::sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
}

inline float dot(float* A, float *B) {
	return A[0]*B[0]+A[1]*B[1]+A[2]*B[2];
}

inline void cross(float *out, float* A, float *B) {
	out[0] = A[1]*B[2] - A[2]*B[1];
	out[1] = A[2]*B[0] - A[0]*B[2];
	out[2] = A[0]*B[1] - A[1]*B[0];
}

inline void quaternion2Rotation(float *R, float *q) {
	R[0] = q[0]*q[0]+q[1]*q[1]-q[2]*q[2]-q[3]*q[3];
	R[1] = 2*(q[1]*q[2]+q[0]*q[3]);
	R[2] = 2*(q[1]*q[3]-q[0]*q[2]);
	R[3] = 2*(q[1]*q[2]-q[0]*q[3]);
	R[4] = q[0]*q[0]-q[1]*q[1]+q[2]*q[2]-q[3]*q[3];
	R[5] = 2*(q[2]*q[3]+q[0]*q[1]);
	R[6] = 2*(q[1]*q[3]+q[0]*q[2]);
	R[7] = 2*(q[2]*q[3]-q[0]*q[1]);
	R[8] = q[0]*q[0]-q[1]*q[1]-q[2]*q[2]+q[3]*q[3];
}

inline void rotate(float *out, float *p, float *R) {
	out[0] = R[0]*p[0] + R[3]*p[1] + R[6]*p[2];
	out[1] = R[1]*p[0] + R[4]*p[1] + R[7]*p[2];
	out[2] = R[2]*p[0] + R[5]*p[1] + R[8]*p[2];
}

#endif
