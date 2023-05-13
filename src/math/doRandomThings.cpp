#include "doRandomThings.h"

RandomDoer::RandomDoer() {
	

#if defined(BUILD_FOR_WINDOWS) || defined(BUILD_FOR_MACOS)
	std::random_device rd;
	unsigned long randSeed = rd();
#else
	unsigned int lo, hi;
	__asm__ __volatile__("rdtsc" : "=a" (lo), "=d" (hi));
	unsigned long randSeed = ((unsigned long long)hi << 32) | lo;
#endif

	gen.seed(randSeed);
	unidis_01  				= new std::uniform_real_distribution<float>(   0, std::nextafter(1,   FLT_MAX));
	unidis_m05_p05 			= new std::uniform_real_distribution<float>(-0.5, std::nextafter(0.5, FLT_MAX));
	unidis_m1_p1 			= new std::uniform_real_distribution<float>(  -1, std::nextafter(1,   FLT_MAX));
	unidis_int 				= NULL;

}

RandomDoer::~RandomDoer() {

	delete unidis_01;
	delete unidis_m05_p05;
	delete unidis_m1_p1;
	if (unidis_int!=NULL)
		delete unidis_int;

}

void RandomDoer::init_uniform_int(int limit) {
	unidis_int 		= new std::uniform_int_distribution<int>(0,limit);
}

void RandomDoer::randomize(Coordinate* obj) {
	do {
		obj->x = uniform_m1_p1();
		obj->y = uniform_m1_p1();
		obj->z = uniform_m1_p1();
	} while ((obj->x==0) && (obj->y==0) && (obj->z==0)) ;

	obj->normalize();
}


void RandomDoer::randomizeWithinVoxel(Coordinate* obj, float *pixDim) {
	obj->x += uniform_m05_p05()*pixDim[1];
	obj->y += uniform_m05_p05()*pixDim[2];
	obj->z += uniform_m05_p05()*pixDim[3];
}


Coordinate RandomDoer::getAUnitRandomVector() {
	Coordinate out;
	randomize(&out);
	return out;
}
