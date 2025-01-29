#pragma once

#include <cfloat>
#include <random>

#include "base/nibr.h"
#include "math/core.h"
#include "math/rotation.h"

namespace NIBR
{

	class RandomDoer {

	public:
		RandomDoer();
		~RandomDoer();

		float 		 uniform_01();					// random number between  0   and 1    uniformly distributed
		float 		 uniform_m05_p05(); 			// random number between -0.5 and 0.5  uniformly distributed
		float 		 uniform_m1_p1();   			// random number between -1   and 1    uniformly distributed
		float        uniform_a_b(float a, float b); // random number between  a   and b    uniformly distributed
		float 		 normal_m0_s1();    			// normal distribution with mean=0 and standard deviation=1
		float 		 normal_m0_s1_double();    		// normal distribution with mean=0 and standard deviation=1 with double precision

		void         init_uniform_int(int limit);
		int          uniform_int();

		void 		 getAUnitRandomVector(float* out);
		void 		 getAUnitRandomPerpVector(float* out, float* inp);
		void 		 getAUnitRandomQuaternion(float* q);
		void         getARandomRotationMatrix(float R[][4]);
		void 		 getARandomMovingFrame(float** F, float* _dir);
		void         getARandomPointWithinDisk(float* x, float *y, float r);
		void         getARandomPointWithinSphere(float* x, float *y, float *z, float r);
		void         getARandomPointWithinSphere(float* p, float r);
		void 		 getARandomPointWithinTriangle(float* out, float* a, float* b, float* c);

		void 		 getARandomPointWithinBoundingBox(float* p, float* bb); // bb must have values in order, e.g., [x_min x_max y_min y_max z_min z_max] 
		void 		 getNRandomPointsWithinBoundingBox(std::vector<float*>& p, float* bb, int N); // bb must have values in order, e.g., [x_min x_max y_min y_max z_min z_max] 

		void 		 randomizeWithinVoxel(float* p, float* pixdim);

		std::vector<std::vector<float>>	getA3DRandomWalk(float* origin, float D, float dt, float N); // D: diffusivity, dt: delta t, N: number of steps

		std::mt19937 getGen() { return gen; }

	private:
		std::mt19937 gen;
		std::uniform_real_distribution<float> *unidis_01;
		std::uniform_real_distribution<float> *unidis_m05_p05;
		std::uniform_real_distribution<float> *unidis_m1_p1;
		std::uniform_int_distribution<int>    *unidis_int;
		std::normal_distribution<float>       *normdis_m0_s1;
		std::normal_distribution<double>      *normdis_m0_s1_double;

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

	inline float RandomDoer::uniform_a_b(float a, float b) {
		return (*unidis_01)(gen)*(b-a) + a;
	}

	inline float RandomDoer::normal_m0_s1() {
		return (*normdis_m0_s1)(gen);
	}

	inline float RandomDoer::normal_m0_s1_double() {
		return (*normdis_m0_s1_double)(gen);
	}

	inline int RandomDoer::uniform_int() {
		return (*unidis_int)(gen);
	}

	inline void RandomDoer::getAUnitRandomVector(float* out) {

		do {
			out[0] = (*unidis_m1_p1)(gen);
			out[1] = (*unidis_m1_p1)(gen);
			out[2] = (*unidis_m1_p1)(gen);
		} while ((out[0]==0) && (out[1]==0) && (out[2]==0));
		normalize(out);

	}

	inline void RandomDoer::getARandomPointWithinDisk(float* x, float *y, float r) {

		do {
			*x = (*unidis_m1_p1)(gen);
			*y = (*unidis_m1_p1)(gen);
		} while ((*x**x+*y**y)>1);
		*x *= r;  
		*y *= r;
	}

	inline void RandomDoer::getARandomPointWithinSphere(float* x, float *y, float *z, float r) {

		do {
			*x = (*unidis_m1_p1)(gen);
			*y = (*unidis_m1_p1)(gen);
			*z = (*unidis_m1_p1)(gen);
		} while ((*x**x+*y**y+*z**z)>1);
		*x *= r;  
		*y *= r;
		*z *= r;
	}

	inline void RandomDoer::getARandomPointWithinSphere(float* p, float r) {

		if (r <= 0.0f) {
			p[0] = 0.0f;
			p[1] = 0.0f;
			p[2] = 0.0f;
			return;
		}

		// Guarantees the (float32) distance to be under < r
		do {
			p[0] = (*unidis_m1_p1)(gen) * r;
			p[1] = (*unidis_m1_p1)(gen) * r;
			p[2] = (*unidis_m1_p1)(gen) * r;
		} while ( std::sqrt(p[0]*p[0] + p[1]*p[1] + p[2]*p[2]) >= r);

	}

	inline void RandomDoer::getARandomPointWithinTriangle(float* out, float* a, float* b, float* c) {

		float r1 = (*unidis_01)(gen);
		float r2 = (*unidis_01)(gen);

		// Generate a uniformly random point in the triangle with vertices (0,0) (0,1) (1,0).
		// If the point (r1, r2) is not inside the triangle, reflect it through the point (0.5, 0.5).
		if (r1 + r2 > 1.0f) {
			r1 = 1.0f - r1;
			r2 = 1.0f - r2;
		}

		// Map the point (r1, r2) in the unit-leg triangle to a point in the triangle ABC.
		// Intuition: the unit legs are stretched to match the sides AB and AC, and the origin
		// is moved to A.
		out[0] = a[0] + r1 * (b[0]-a[0]) + r2 * (c[0]-a[0]);
		out[1] = a[1] + r1 * (b[1]-a[1]) + r2 * (c[1]-a[1]);
		out[2] = a[2] + r1 * (b[2]-a[2]) + r2 * (c[2]-a[2]);
	}

	inline void RandomDoer::getAUnitRandomPerpVector(float* out, float* inp) {
		float tmp[3];
		getAUnitRandomVector(tmp);
		cross(out,inp,tmp);
		normalize(out);
	}

	inline void RandomDoer::getAUnitRandomQuaternion(float* q) {
		q[0] = (*normdis_m0_s1)(gen);
		q[1] = (*normdis_m0_s1)(gen);
		q[2] = (*normdis_m0_s1)(gen);
		q[3] = (*normdis_m0_s1)(gen);
		float normalizer = 1.0/std::sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]);
		q[0] *= normalizer;
		q[1] *= normalizer;
		q[2] *= normalizer;
		q[3] *= normalizer;
	}

	inline void RandomDoer::getARandomRotationMatrix(float R[][4]) {
		float q[4];
		getAUnitRandomQuaternion(&q[0]);
		quaternion2Rotation(&q[0],R);
	}

	inline void RandomDoer::getARandomMovingFrame(float** F, float* _dir) {
		
		if (_dir==NULL) {
			getAUnitRandomVector(F[0]);
			getAUnitRandomPerpVector(F[2],F[0]);
			cross(F[1],F[2],F[0]);
			return;
		}
		
		F[0][0] = _dir[0];
		F[0][1] = _dir[1];
		F[0][2] = _dir[2];
		getAUnitRandomPerpVector(F[2],F[0]);
		cross(F[1],F[2],F[0]);
	}

	// bb must have values in order, e.g., [x_min x_max y_min y_max z_min z_max] 
	inline void RandomDoer::getARandomPointWithinBoundingBox(float* p, float* bb) {
		p[0] = (*unidis_01)(gen)*(bb[1]-bb[0]) + bb[0];
		p[1] = (*unidis_01)(gen)*(bb[3]-bb[2]) + bb[2];
		p[2] = (*unidis_01)(gen)*(bb[5]-bb[4]) + bb[4];
	}

	// bb must have values in order, e.g., [x_min x_max y_min y_max z_min z_max] 
	inline void RandomDoer::getNRandomPointsWithinBoundingBox(std::vector<float*>& p, float* bb, int N) {

		p.reserve(N);

		for (size_t i=0; i<p.size(); i++) {
			float* r = new float[3];
			getARandomPointWithinBoundingBox(r,bb);
			p.push_back(r);
		}

	}

	// origin: float[3], D: diffusivity, dt: delta t, N: number of steps
	// Output walker is Nx3
	inline std::vector<std::vector<float>> RandomDoer::getA3DRandomWalk(float* origin, float D, float dt, float N) {

		std::vector<std::vector<float>> out;

		double k = std::sqrt(2*double(D)*double(dt));

		double x = origin[0];
		double y = origin[1];
		double z = origin[2];

		out.push_back({float(x),float(y),float(z)});

		for (int n=1; n<N; n++) {

			x += k*normal_m0_s1_double();
			y += k*normal_m0_s1_double();
			z += k*normal_m0_s1_double();

			out.push_back({float(x),float(y),float(z)});

		}

		return out;

	}

	inline void RandomDoer::randomizeWithinVoxel(float* p, float *pixDim) {
		p[0] += uniform_m05_p05()*pixDim[0];
		p[1] += uniform_m05_p05()*pixDim[1];
		p[2] += uniform_m05_p05()*pixDim[2];

		// Prevents points to be exactly on the voxel edges
		for (int i = 0; i < 3; i++) {
			if (p[i] == (pixDim[i]-0.5f)) p[i] = std::nextafterf(pixDim[i]-0.5f, FLT_MAX);
			if (p[i] == (pixDim[i]+0.5f)) p[i] = std::nextafterf(pixDim[i]+0.5f, FLT_MIN);
		}
	}

}