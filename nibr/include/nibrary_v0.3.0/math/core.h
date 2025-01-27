#pragma once
#ifdef _WIN32
#include <math.h>
#endif

#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <complex>
#include <cmath>
#include <cfloat>
#include <functional>
#include <algorithm>

#include "base/nibr.h"

namespace NIBR 
{

    #define SQRT2	   1.41421356237309504880168872420969807856967187537694807
    #define PI 		   3.14159265358979323846264338327950288419716939937510582
    #define TWOPI	   6.28318530717958647692528676655900576839433879875021164
    #define PIOVERTWO  1.57079632679489661923132169163975144209858469968755291
    #define N180OVERPI 57.2957795130823208767981548141051703324054724665643215 // 180/PI
    #define PIOVERN180 0.01745329251994329576923690768488612713442871888541725 // PI/180

    #define EPS16      0.0000000000000001
    #define EPS15      0.000000000000001
    #define EPS14      0.00000000000001
    #define EPS13      0.0000000000001
    #define EPS12      0.000000000001
    #define EPS11      0.00000000001
    #define EPS10      0.0000000001
    #define EPS9       0.000000001
    #define EPS8       0.00000001
    #define EPS7       0.0000001
    #define EPS6       0.000001
    #define EPS5       0.00001
    #define EPS4       0.0001
    #define EPS3       0.001
    #define EPS2       0.01

    struct Point{
        float x;
        float y;
        float z;
    };

    struct LineSegment {
        int    id{INT_MIN};
        float* beg{NULL};
        float* end{NULL};
        float  len{0};
        float  dir[3]{0,0,0};
    };

    // Implementation of a generalized signof
    template <typename T> inline constexpr
    int getSign(const T& x, std::false_type) {
        return T(0) < x;
    }

    template <typename T> inline constexpr
    int getSign(const T& x, std::true_type) {
        return (T(0) < x) - (x < T(0));
    }

    template <typename T> inline constexpr
    int getSign(const T& x) {
        return getSign(x, std::is_signed<T>());
    }
    //-----


    template<typename T> inline constexpr
    double norm(const T& v) 
    { 
        return std::sqrt(double(v[0])*double(v[0])+double(v[1])*double(v[1])+double(v[2])*double(v[2]));
    }

    template<typename T> inline constexpr
    double norm(std::vector<T>& v) 
    {
        return std::sqrt(double(v[0])*double(v[0])+double(v[1])*double(v[1])+double(v[2])*double(v[2]));
    }

    template<typename T> inline constexpr
    void normalize(T& v) 
    {
        double scale  = 1.0/std::sqrt(double(v[0])*double(v[0])+double(v[1])*double(v[1])+double(v[2])*double(v[2]));
        v[0] *= scale;
        v[1] *= scale;
        v[2] *= scale;
    }

    template<typename T> inline constexpr
    void normalize(std::vector<T>& v) 
    {
        double scale  = 1.0/std::sqrt(double(v[0])*double(v[0])+double(v[1])*double(v[1])+double(v[2])*double(v[2]));
        v[0] *= scale;
        v[1] *= scale;
        v[2] *= scale;
    }


    template<typename T> inline constexpr
    void verifyUnitRange(T& v) {
        for (int i=0;i<3;i++){
            if (v[i] >  1) v[i]= 0.99999999999999999999999999999999999999;
            if (v[i] < -1) v[i]=-0.99999999999999999999999999999999999999;
        }
    }

    template<typename T> inline constexpr
    void verifyUnitRange(std::vector<T>& v) 
    {
        for (int i=0;i<3;i++){
            if (v[i] >  1) v[i]= 0.99999999999999999999999999999999999999;
            if (v[i] < -1) v[i]=-0.99999999999999999999999999999999999999;
        }
    }

    template<typename T1,typename T2> inline constexpr
    double dot(const T1& v1, const T2& v2) 
    {
        return double(v1[0])*double(v2[0])+double(v1[1])*double(v2[1])+double(v1[2])*double(v2[2]);
    }

    template<typename T1,typename T2> inline constexpr
    double distS2(const T1& v1, const T2& v2) 
    {
        return std::acos(std::clamp(dot(v1,v2),-1.0,1.0));
    }

    template <typename TOUT, typename T1, typename T2> inline constexpr
    void cross(TOUT& out, const T1& v1, const T2& v2)
    {
        out[0] = v1[1]*v2[2] - v1[2]*v2[1];
        out[1] = v1[2]*v2[0] - v1[0]*v2[2];
        out[2] = v1[0]*v2[1] - v1[1]*v2[0];
    }

    template<typename T1,typename T2> inline constexpr
    double dist(const T1& p1,const T2& p2)
    {
        return std::sqrt( (p1[0]-p2[0])*(p1[0]-p2[0]) + (p1[1]-p2[1])*(p1[1]-p2[1]) + (p1[2]-p2[2])*(p1[2]-p2[2]) );
    }

    inline double dist(const Point* p1,const Point* p2)
    {
        return std::sqrt( (p1->x-p2->x)*(p1->x-p2->x) + (p1->y-p2->y)*(p1->y-p2->y) + (p1->z-p2->z)*(p1->z-p2->z) );
    }

    inline double dist(const Point& p1,const Point& p2)
    {
        return std::sqrt( (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) + (p1.z-p2.z)*(p1.z-p2.z) );
    }

    template<typename T1,typename T2> inline constexpr
    double squared_dist(const T1& p1,const T2& p2)
    {
        return (p1[0]-p2[0])*(p1[0]-p2[0]) + (p1[1]-p2[1])*(p1[1]-p2[1]) + (p1[2]-p2[2])*(p1[2]-p2[2]);
    }

    inline double squared_dist(const Point* p1,const Point* p2)
    {
        return (p1->x-p2->x)*(p1->x-p2->x) + (p1->y-p2->y)*(p1->y-p2->y) + (p1->z-p2->z)*(p1->z-p2->z);
    }

    inline double squared_dist(const Point& p1,const Point& p2)
    {
        return (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) + (p1.z-p2.z)*(p1.z-p2.z);
    }



    template<typename TOUT,typename T1,typename T2> inline constexpr
    void vec3sub(TOUT& out, const T1& v1,const T2& v2)
    {
        out[0] = v1[0] - v2[0];
        out[1] = v1[1] - v2[1];
        out[2] = v1[2] - v2[2];
    }

    template<typename TOUT,typename T1,typename T2> inline constexpr
    void vec3add(TOUT& out, const T1& v1,const T2& v2)
    {
        out[0] = v1[0] + v2[0];
        out[1] = v1[1] + v2[1];
        out[2] = v1[2] + v2[2];
    }

    template<typename TOUT,typename T1,typename T2, typename T3> inline constexpr
    void vec3add(TOUT& out, const T1& v1,const T2& v2, const T3& s)
    {
        out[0] = v1[0] + s*v2[0];
        out[1] = v1[1] + s*v2[1];
        out[2] = v1[2] + s*v2[2];
    }

    template<typename T1,typename T2> inline constexpr
    void vec3scale(T1& v, const T2& s)
    {
        v[0] *= s;
        v[1] *= s;
        v[2] *= s;
    }

    template<typename T> inline constexpr
    void vec3abs(T& v)
    {
        v[0] = std::fabs(v[0]);
        v[1] = std::fabs(v[1]);
        v[2] = std::fabs(v[2]);
    }

    template<typename TOUT> inline constexpr
    void point2vec3(TOUT& out, const Point& p, const Point& ref)
    {
        out[0] = p.x - ref.x;
        out[1] = p.y - ref.y;
        out[2] = p.z - ref.z;
    }


    template<typename T> inline constexpr
    void applyTransform(Point& p, const T& M)
    {
        float x = p.x*M[0][0] + p.y*M[0][1] + p.z*M[0][2] + M[0][3];
        float y = p.x*M[1][0] + p.y*M[1][1] + p.z*M[1][2] + M[1][3];
        float z = p.x*M[2][0] + p.y*M[2][1] + p.z*M[2][2] + M[2][3];
        p.x = x;
        p.y = y;
        p.z = z;
    }

    template<typename T1,typename T2> inline constexpr
    void applyTransform(T1& v, const T2& M)
    {
        double x = v[0]*M[0][0] + v[1]*M[0][1] + v[2]*M[0][2] + M[0][3];
        double y = v[0]*M[1][0] + v[1]*M[1][1] + v[2]*M[1][2] + M[1][3];
        double z = v[0]*M[2][0] + v[1]*M[2][1] + v[2]*M[2][2] + M[2][3];
        v[0] = x;
        v[1] = y;
        v[2] = z;
    }

    template<typename T1,typename T2> inline constexpr
    void applyTransform(std::vector<T1>& v_arr, const T2& M)
    {

        for (auto v : v_arr) {
            double x = v[0]*M[0][0] + v[1]*M[0][1] + v[2]*M[0][2] + M[0][3];
            double y = v[0]*M[1][0] + v[1]*M[1][1] + v[2]*M[1][2] + M[1][3];
            double z = v[0]*M[2][0] + v[1]*M[2][1] + v[2]*M[2][2] + M[2][3];
            v[0] = x;
            v[1] = y;
            v[2] = z;
        }
        
    }

    template<typename T1, typename T2, typename T3> inline constexpr
    void applyTransform(T1& vout, const T2& vinp, const T3& M)
    {
        vout[0] = vinp[0]*M[0][0] + vinp[1]*M[0][1] + vinp[2]*M[0][2] + M[0][3];
        vout[1] = vinp[0]*M[1][0] + vinp[1]*M[1][1] + vinp[2]*M[1][2] + M[1][3];
        vout[2] = vinp[0]*M[2][0] + vinp[1]*M[2][1] + vinp[2]*M[2][2] + M[2][3];
    }


    template<typename T>
    T rad2deg(const T& rad) {
        return rad*N180OVERPI;
    }

    template<typename T>
    T deg2rad(const T& deg) {
        return deg*PIOVERN180;
    }

    // abc triangle.
    // angle is computed for a
    template<typename T1,typename T2> inline constexpr
    double angle(const T1& a, const T2& b, const T2& c) 
    {
        float ab[3]={0.0f,0.0f,0.0f}; vec3sub(ab,b,a);
        float ac[3]={0.0f,0.0f,0.0f}; vec3sub(ac,c,a);

        double dot_product  = dot(ac,ab);
        double norm_ab      = norm(ab);
        double norm_ac      = norm(ac);
        double cos_angle    = dot_product / (norm_ab * norm_ac);

        cos_angle    = std::max(-1.0, std::min(1.0, cos_angle));

        return acos(cos_angle);
    }


    // A: voxel indices, i.e. center of voxel in image space
    // p0: origin of ray
    // dir: direction of ray
    // 
    // Returns true if ray intersects the voxel, and t is the distance from the ray origin to the intersection point.
    // Returns false if ray does not intersect the voxel, and t is set to NAN.
    //
    //
    // Note: This function handles the edge cases. 
    //    In image space if A=[0,0,0] the -0.5 is outside the voxel
    //    For other voxels x-0.5 is inside the voxel, and x+0.5 is outside the voxel, i.e. x+0.5 is inside voxel index x+1, not x
    bool rayTraceVoxel(int* A, double* p0, double*  dir, double  &t);

    // boxcenter is voxel indices in integers. boxcenter is the voxel center
    // box is then between boxcenter-0.5 and boxcenter+0.5, which is consistent with nifti convention
    // c0, c1, c2 are triangle corners
    // returns true if there is overlap
    bool triangleVoxelIntersection(int* boxcenter, float* c0, float* c1, float* c2);
    bool triangleLargerVoxelIntersection(int* boxcenter, float* c0, float* c1, float* c2);

    
    // Inverses a 4x4 affine transformation matrix. Return false if matrix is singular.
    // This function is modified from niftilib
    /*  INPUT MATRIX MUST BE OF THE FORM: */
    /* [ r11 r12 r13 v1 ] */
    /* [ r21 r22 r23 v2 ] */
    /* [ r31 r32 r33 v3 ] */
    /* [  0   0   0   1 ] */
    template<typename T1,typename T2>
    bool inverseAffine(const T1& R, T2& Q)
    {
        double r11,r12,r13,r21,r22,r23,r31,r32,r33,v1,v2,v3 , deti ;
                                                    
        r11 = R[0][0]; r12 = R[0][1]; r13 = R[0][2];
        r21 = R[1][0]; r22 = R[1][1]; r23 = R[1][2];
        r31 = R[2][0]; r32 = R[2][1]; r33 = R[2][2];
        v1  = R[0][3]; v2  = R[1][3]; v3  = R[2][3];

        deti = r11*r22*r33-r11*r32*r23-r21*r12*r33+r21*r32*r13+r31*r12*r23-r31*r22*r13 ;

        if( deti != 0.0l ) deti = 1.0l / deti ;

        Q[0][0] = deti*( r22*r33-r32*r23);
        Q[0][1] = deti*(-r12*r33+r32*r13);
        Q[0][2] = deti*( r12*r23-r22*r13);
        Q[0][3] = deti*(-r12*r23*v3+r12*v2*r33+r22*r13*v3-r22*v1*r33-r32*r13*v2+r32*v1*r23);

        Q[1][0] = deti*(-r21*r33+r31*r23);
        Q[1][1] = deti*( r11*r33-r31*r13);
        Q[1][2] = deti*(-r11*r23+r21*r13);
        Q[1][3] = deti*( r11*r23*v3-r11*v2*r33-r21*r13*v3+r21*v1*r33+r31*r13*v2-r31*v1*r23);

        Q[2][0] = deti*( r21*r32-r31*r22);
        Q[2][1] = deti*(-r11*r32+r31*r12);
        Q[2][2] = deti*( r11*r22-r21*r12);
        Q[2][3] = deti*(-r11*r22*v3+r11*r32*v2+r21*r12*v3-r21*r32*v1-r31*r12*v2+r31*r22*v1);

        Q[3][0] = Q[3][1] = Q[3][2] = 0.0l;
        Q[3][3] = (deti == 0.0l) ? 0.0l : 1.0l;

        if (deti==0) 
            return false;
        else
            return true;
    }

}
