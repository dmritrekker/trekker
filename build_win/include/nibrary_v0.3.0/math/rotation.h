#pragma once

#include "base/nibr.h"
#include <vector>

namespace NIBR 
{

	// Rotations
	template<class T>
	void  rotate(T& out, float R[][4]);

	template<class T1,class T2>
	void  rotate(T1 out, T2 p, float R[][4]);

	void  rotate(float* out, float* p, float* center, float R[][4]);
	void  rotate(std::vector<float*>& out, std::vector<float*>& p, float R[][4]);
	void  rotate(std::vector<float*>& out, std::vector<float*>& p, float* center, float R[][4]);

	void  rotateWithQuaternion(float* out, float* p, float *q);
	void  rotateWithQuaternion(float* out, float* p, float* center, float *q);
	void  rotateWithQuaternion(std::vector<float*>& out, std::vector<float*>& p, float *q);
	void  rotateWithQuaternion(std::vector<float*>& out, std::vector<float*>& p, float* center, float *q);

	void  rotateWithAxisAngle(float* out, float* p, float* axis, float angle);
	void  rotateWithAxisAngle(std::vector<float*>& out, std::vector<float*>& p, float* axis, float angle);



	// Rotation transformations
	void  inverseRotation(float R[][4], float out[][4]);
	void  quaternion2Rotation(float *q, float R[][4]);
	void  rotation2Quaternion(float R[][4], float *q);
	void  axisangle2Rotation(float *v, float theta, float R[][4]);
	void  axisangle2Quaternion(float *v, float theta, float *q);


	template<class T>
	inline void rotate(T& out, float R[][4]) {
		
		float p[3] = {out[0],out[1],out[2]};

		out[0] = R[0][0]*p[0] + R[0][1]*p[1] + R[0][2]*p[2];
		out[1] = R[1][0]*p[0] + R[1][1]*p[1] + R[1][2]*p[2];
		out[2] = R[2][0]*p[0] + R[2][1]*p[1] + R[2][2]*p[2];
	}

	template<class T1,class T2>
	inline void rotate(T1 out, T2 p, float R[][4]) {
		out[0] = R[0][0]*p[0] + R[0][1]*p[1] + R[0][2]*p[2];
		out[1] = R[1][0]*p[0] + R[1][1]*p[1] + R[1][2]*p[2];
		out[2] = R[2][0]*p[0] + R[2][1]*p[1] + R[2][2]*p[2];
	}

	inline void rotate(float* out, float* p, float* center, float R[][4]) {
		out[0] = R[0][0]*(p[0]-center[0]) + R[0][1]*(p[1]-center[1]) + R[0][2]*(p[2]-center[2]) + center[0];
		out[1] = R[1][0]*(p[0]-center[0]) + R[1][1]*(p[1]-center[1]) + R[1][2]*(p[2]-center[2]) + center[1];
		out[2] = R[2][0]*(p[0]-center[0]) + R[2][1]*(p[1]-center[1]) + R[2][2]*(p[2]-center[2]) + center[2];
	}

	inline void rotate(std::vector<float*>& out, std::vector<float*>& p, float R[][4]) {

		out.reserve(p.size());

		for (size_t i=0; i<p.size(); i++) {
			float* r = new float[3];
			r[0] = R[0][0]*p[i][0] + R[0][1]*p[i][1] + R[0][2]*p[i][2];
			r[1] = R[1][0]*p[i][0] + R[1][1]*p[i][1] + R[1][2]*p[i][2];
			r[2] = R[2][0]*p[i][0] + R[2][1]*p[i][1] + R[2][2]*p[i][2];
			out[i] = r;
		}

	}

	inline void rotate(std::vector<float*>& out, std::vector<float*>& p, float* center, float R[][4]) {

		out.reserve(p.size());

		for (size_t i=0; i<p.size(); i++) {
			float* r = new float[3];
			r[0] = R[0][0]*(p[i][0]-center[0]) + R[0][1]*(p[i][1]-center[1]) + R[0][2]*(p[i][2]-center[2]) + center[0];
			r[1] = R[1][0]*(p[i][0]-center[0]) + R[1][1]*(p[i][1]-center[1]) + R[1][2]*(p[i][2]-center[2]) + center[1];
			r[2] = R[2][0]*(p[i][0]-center[0]) + R[2][1]*(p[i][1]-center[1]) + R[2][2]*(p[i][2]-center[2]) + center[2];
			out[i] = r;
		}

	}

	inline void rotateWithQuaternion(float* out, float* p, float *q) {
		float R[3][4];
		quaternion2Rotation(q,R);
		rotate(out,p,R);
	}

	inline void rotateWithQuaternion(float* out, float* p, float* center, float *q) {
		float R[3][4];
		quaternion2Rotation(q,R);
		rotate(out,p,center,R);
	}

	inline void rotateWithQuaternion(std::vector<float*>& out, std::vector<float*>& p, float *q) {
		float R[3][4];
		quaternion2Rotation(q,R);
		rotate(out,p,R);
	}

	inline void rotateWithQuaternion(std::vector<float*>& out, std::vector<float*>& p, float* center, float *q) {
		float R[3][4];
		quaternion2Rotation(q,R);
		rotate(out,p,center,R);
	}

	// 3D rotation using quaternions
	// Simplified expression from:
	// http://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
	inline void rotateWithAxisAngle(float* out, float* p, float* axis, float angle) {
		if (angle > 1e-6) {
			float cos_ha           = cos(angle/(2.0f));
			float sin_ha           = sin(angle/(2.0f));

			float ux               = axis[0]*sin_ha;
			float uy               = axis[1]*sin_ha;
			float uz               = axis[2]*sin_ha;

			float c1               = 2*(ux*p[0]+uy*p[1]+uz*p[2]);
			float term1x           = ux*c1;
			float term1y           = uy*c1;
			float term1z           = uz*c1;

			float c2               = cos_ha*cos_ha-ux*ux-uy*uy-uz*uz;
			float term2x           = p[0]*c2;
			float term2y           = p[1]*c2;
			float term2z           = p[2]*c2;

			float c3               = 2*cos_ha;
			float term3x           = (uy*p[2]-uz*p[1])*c3;
			float term3y           = (uz*p[0]-ux*p[2])*c3;
			float term3z           = (ux*p[1]-uy*p[0])*c3;

			out[0]                  = term1x + term2x + term3x;
			out[1]                  = term1y + term2y + term3y;
			out[2]                  = term1z + term2z + term3z;
		} else
			for (int i=0; i<3; i++)
				out[i] = p[i];

	}


	// 3D rotation using quaternions
	// Simplified expression from:
	// http://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
	inline void rotateWithAxisAngle(std::vector<float*>& out, std::vector<float*>& p, float* axis, float angle) {
		
		out.reserve(p.size());

		if (angle < 1e-6) {

			for (size_t i=0; i<p.size(); i++) {
				float* r = new float[3];
				r[0]     = p[i][0];
				r[1]     = p[i][1];
				r[2]     = p[i][2];
				out[i]   = r;
			}

			return;

		}


		float cos_ha = cos(angle/(2.0f));
		float sin_ha = sin(angle/(2.0f));

		float ux     = axis[0]*sin_ha;
		float uy     = axis[1]*sin_ha;
		float uz     = axis[2]*sin_ha;

		float c1,term1x,term1y,term1z;
		float c2,term2x,term2y,term2z;
		float c3,term3x,term3y,term3z;

		for (size_t i=0; i<p.size(); i++) {

			float* r  = new float[3];

			c1        = 2*(ux*p[i][0]+uy*p[i][1]+uz*p[i][2]);
			term1x    = ux*c1;
			term1y    = uy*c1;
			term1z    = uz*c1;

			c2        = cos_ha*cos_ha-ux*ux-uy*uy-uz*uz;
			term2x    = p[i][0]*c2;
			term2y    = p[i][1]*c2;
			term2z    = p[i][2]*c2;

			c3        = 2*cos_ha;
			term3x    = (uy*p[i][2]-uz*p[i][1])*c3;
			term3y    = (uz*p[i][0]-ux*p[i][2])*c3;
			term3z    = (ux*p[i][1]-uy*p[i][0])*c3;

			r[0]      = term1x + term2x + term3x;
			r[1]      = term1y + term2y + term3y;
			r[2]      = term1z + term2z + term3z;

			out[i]    = r;


		}

	}






	inline void rotation2Quaternion(float R[][4], float *q){

		if ((R[1][1] > -R[2][2]) && (R[0][0] > -R[1][1]) && (R[0][0] > -R[2][2])) {
			q[0] = 0.5 * sqrt(1 + R[0][0] + R[1][1] + R[2][2]);
			q[1] = - 0.5 * (R[1][2] - R[2][1]) / sqrt(1 + R[0][0] + R[1][1] + R[2][2]);
			q[2] = - 0.5 * (R[2][0] - R[0][2]) / sqrt(1 + R[0][0] + R[1][1] + R[2][2]);
			q[3] = - 0.5 * (R[0][1] - R[1][0]) / sqrt(1 + R[0][0] + R[1][1] + R[2][2]);
		}

		else if ((R[1][1] < -R[2][2]) && (R[0][0] > R[1][1]) && (R[0][0] > R[2][2])) {
			q[0] = - 0.5 * (R[1][2] - R[2][1]) / sqrt(1 + R[0][0] - R[1][1] - R[2][2]);
			q[1] = 0.5 * sqrt(1 + R[0][0] - R[1][1] - R[2][2]);
			q[2] = 0.5 * (R[0][1] + R[1][0]) / sqrt(1 + R[0][0] - R[1][1] - R[2][2]);
			q[3] = 0.5 * (R[2][0] + R[0][2]) / sqrt(1 + R[0][0] - R[1][1] - R[2][2]);
		}

		else if ((R[1][1] > R[2][2]) && (R[0][0] < R[1][1]) && (R[0][0] < -R[2][2])) {
			q[0] = - 0.5 * (R[2][0] - R[0][2]) / sqrt(1 - R[0][0] + R[1][1] - R[2][2]);
			q[1] = 0.5 * (R[0][1] + R[1][0]) / sqrt(1 - R[0][0] + R[1][1] - R[2][2]);
			q[2] = 0.5 * sqrt(1 - R[0][0] + R[1][1] - R[2][2]);
			q[3] = 0.5 * (R[1][2] + R[2][1]) / sqrt(1 - R[0][0] + R[1][1] - R[2][2]);
		}

		else if ((R[1][1] < R[2][2]) && (R[0][0] < -R[1][1]) && (R[0][0] < R[2][2])) {
			q[0] = - 0.5 * (R[0][1] - R[1][0]) / sqrt(1 - R[0][0] - R[1][1] + R[2][2]);
			q[1] = 0.5 * (R[2][0] + R[0][2]) / sqrt(1 - R[0][0] - R[1][1] + R[2][2]);
			q[2] = 0.5 * (R[1][2] + R[2][1]) / sqrt(1 - R[0][0] - R[1][1] + R[2][2]);
			q[3] = 0.5 * sqrt(1 - R[0][0] - R[1][1] + R[2][2]);
		}

		q[0] = -q[0];
		q[1] = -q[1];
		q[2] = -q[2];
		q[3] = -q[3];

	}

	inline void quaternion2Rotation(float *q, float R[][4]) {
		R[0][0] = q[0]*q[0]+q[1]*q[1]-q[2]*q[2]-q[3]*q[3];
		R[1][0] = 2*(q[1]*q[2]+q[0]*q[3]);
		R[2][0] = 2*(q[1]*q[3]-q[0]*q[2]);
		R[0][1] = 2*(q[1]*q[2]-q[0]*q[3]);
		R[1][1] = q[0]*q[0]-q[1]*q[1]+q[2]*q[2]-q[3]*q[3];
		R[2][1] = 2*(q[2]*q[3]+q[0]*q[1]);
		R[0][2] = 2*(q[1]*q[3]+q[0]*q[2]);
		R[1][2] = 2*(q[2]*q[3]-q[0]*q[1]);
		R[2][2] = q[0]*q[0]-q[1]*q[1]-q[2]*q[2]+q[3]*q[3];
	}

	inline void inverseRotation(float R[][4], float out[][4]) {
		out[0][0] = R[1][1] * R[2][2] - R[2][1] * R[1][2] ;
		out[1][0] = R[0][2] * R[2][1] - R[0][1] * R[2][2] ;
		out[2][0] = R[0][1] * R[1][2] - R[0][2] * R[1][1] ;
		out[0][1] = R[1][2] * R[2][0] - R[1][0] * R[2][2] ;
		out[1][1] = R[0][0] * R[2][2] - R[0][2] * R[2][0] ;
		out[2][1] = R[1][0] * R[0][2] - R[0][0] * R[1][2] ;
		out[0][2] = R[1][0] * R[2][1] - R[2][0] * R[1][1] ;
		out[1][2] = R[2][0] * R[0][1] - R[0][0] * R[2][1] ;
		out[2][2] = R[0][0] * R[1][1] - R[1][0] * R[0][1] ;
	}

	inline void axisangle2Quaternion(float *v, float theta, float *q) {
		if (std::fabs(theta)<1e-6) {
			q[0] = 1;
			q[1] = 0;
			q[2] = 0;
			q[3] = 0;
			return;
		}

		float w = std::sin(theta/2.0f);
		q[0] 	= std::cos(theta/2.0f);
		q[1] 	= w*v[0];
		q[2] 	= w*v[1];
		q[3] 	= w*v[2];
	}

	inline void axisangle2Rotation(float *v, float theta, float R[][4]) {
		float q[4];
		axisangle2Quaternion(v, theta, &q[0]);
		quaternion2Rotation(&q[0], R);
	}

}
