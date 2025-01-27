#pragma once

#include "base/nibr.h"
#include "math/core.h"

namespace NIBR
{

    template<class T>
    T areaOfTriangle(T* a, T* b, T* c) 
    {
        T v1[3], v2[3], v3[3];
        
        vec3sub(v1, b, a);
        vec3sub(v2, c, a);
        cross(v3, v1, v2);
        T res = norm(v3)*0.5f;

        return res;
    }

    // p is the point to interpolate the value
    // a,b,c are the corners of input triangle
    // va,vb,vc are the values at the corresponding corners
    template<class T1, class T2>
    T2* barycentricInterp(T1* p, T1* a, T1* b, T1* c, T2* va, T2* vb, T2* vc, int ndim) 
    {
        T2* res = new T2[ndim];

        T1 A_abp = areaOfTriangle(a, b, p);
        T1 A_bcp = areaOfTriangle(b, c, p);
        T1 A_cap = areaOfTriangle(c, a, p);
        T1 A_tot = A_abp + A_bcp + A_cap;

        for (int i=0; i<ndim; i++) {
            res[i] = (va[i]*A_bcp + vb[i]*A_cap + vc[i]*A_abp) / A_tot;
        }

        return res;
    }

    // p is the point to interpolate the value
    // a,b,c are the corners of input triangle
    // va,vb,vc are the values at the corresponding corners
    template<class T1, class T2>
    void barycentricInterp(T2* res, T1* p, T1* a, T1* b, T1* c, T2* va, T2* vb, T2* vc) 
    {

        T1 A_abp = areaOfTriangle(a, b, p);
        T1 A_bcp = areaOfTriangle(b, c, p);
        T1 A_cap = areaOfTriangle(c, a, p);
        T1 A_tot = A_abp + A_bcp + A_cap;

        for (int i=0; i<3; i++) {
            res[i] = (va[i]*A_bcp + vb[i]*A_cap + vc[i]*A_abp) / A_tot;
        }

    }
}