#ifndef SRC_MATH_DORANDOMTHINGS_H_
#define SRC_MATH_DORANDOMTHINGS_H_

#include <cfloat>
#include <random>

#include "math_aux.h"
#include "coordinate.h"

#ifdef _MSC_VER
# include <intrin.h>
#else
# include <x86intrin.h>
#endif

class RandomDoer {

public:
	RandomDoer();
	~RandomDoer();

	float 		 uniform_01();
	float 		 uniform_m05_p05();
	float 		 uniform_m1_p1();

	void         init_uniform_int(int limit);
	int          uniform_int();

	void 		 randomize(Coordinate* obj);
	void 		 randomizeWithinVoxel(Coordinate* obj, float *pixDim);

	Coordinate 	 getAUnitRandomVector();
	void 		 getAUnitRandomVector(float* out);
	void 		 getAUnitRandomPerpVector(float* out, float* inp);

private:
	std::mt19937 gen;
	std::uniform_real_distribution<float> *unidis_01;
	std::uniform_real_distribution<float> *unidis_m05_p05;
	std::uniform_real_distribution<float> *unidis_m1_p1;
	std::uniform_int_distribution<int> *unidis_int;

};

inline float RandomDoer::uniform_01() {
	return (*unidis_01)(gen);
}

inline float RandomDoer::uniform_m05_p05() {
	return (*unidis_m05_p05)(gen);
}

inline float RandomDoer::uniform_m1_p1() {
	return (*unidis_m1_p1)(gen);
}

inline int RandomDoer::uniform_int() {
	return (*unidis_int)(gen);
}

inline void RandomDoer::getAUnitRandomVector(float* out) {

	do {
		out[0] = uniform_m1_p1();
		out[1] = uniform_m1_p1();
		out[2] = uniform_m1_p1();
	} while ((out[0]==0) && (out[1]==0) && (out[2]==0));
	normalize(out);

}

inline void RandomDoer::getAUnitRandomPerpVector(float* out, float* inp) {
	float tmp[3];
	getAUnitRandomVector(tmp);
	cross(out,inp,tmp);
	normalize(out);
}

#endif
