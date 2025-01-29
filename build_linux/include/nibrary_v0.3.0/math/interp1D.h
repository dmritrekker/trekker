#pragma once

#include "base/nibr.h"
#include "base/verbose.h"
#include "core.h"

namespace NIBR 
{

    template<class TOUT,class TP, class T>
    void linearInterp(TOUT out, TP p0, TP p1, T s) {

        if (s<=0) {
            out[0] = p0[0];
            out[1] = p0[1];
            out[2] = p0[2];
            return;
        }

        if (s>=1) {
            out[0] = p1[0];
            out[1] = p1[1];
            out[2] = p1[2];
            return;
        }

        for (int i=0; i<3; i++) {
            out[i] = (1.0f-s)*p0[i] + s*p1[i];
        }
        
        return;

    }

    template<class TOUT,class TP, class T>
    void cosineInterp(TOUT out, TP p0, TP p1, T s) {

        if (s<=0) {
            out[0] = p0[0];
            out[1] = p0[1];
            out[2] = p0[2];
            return;
        }

        if (s>=1) {
            out[0] = p1[0];
            out[1] = p1[1];
            out[2] = p1[2];
            return;
        }

        s = 1.0f-cos(s*PIOVERTWO);

        for (int i=0; i<3; i++) {
            out[i] = p0[i]*s + p1[i]*(1.0f-s);
        }
        
        return;

    }

    template<class TOUT,class TP,class TT, class T>
    void hermiteInterp(TOUT out, TP p0, TT T0, TP p1, TT T1, T s) {

        if (s<=0) {
            out[0] = p0[0];
            out[1] = p0[1];
            out[2] = p0[2];
            return;
        }

        if (s>=1) {
            out[0] = p1[0];
            out[1] = p1[1];
            out[2] = p1[2];
            return;
        }

        T s2 = s*s;
        T s3 = s2*s;

        T h1 =  2*s3 - 3*s2 + 1;
        T h2 = -2*s3 + 3*s2;
        T h3 =    s3 - 2*s2 + s;
        T h4 =    s3 -   s2;

        for (int i=0; i<3; i++) {
            out[i] = h1*p0[i] + h2*p1[i] + h3*T0[i] + h4*T1[i];
        }
        
        return;

    }

    template<class T,class TT>
    float hermiteLength(T p0, TT T0, T p1, TT T1, int div) {

        float a[3], b[3];
        for (int i=0; i<3; i++) {
            a[i] =  2*p0[i] +  T0[i] - 2*p1[i] + T1[i];
            b[i] = -3*p0[i] -2*T0[i] + 3*p1[i] - T1[i];
        }

        float step   = 1.0f/float(div);
        float s      = step;

        float prev[3] = {p0[0],p0[1],p0[2]};
        float next[3];
        float length  = 0;

        while (s<(1.0f-EPS8)) {

            next[0] = a[0]*s*s*s + b[0]*s*s + T0[0]*s + p0[0];
            next[1] = a[1]*s*s*s + b[1]*s*s + T0[1]*s + p0[1];
            next[2] = a[2]*s*s*s + b[2]*s*s + T0[2]*s + p0[2];

            length += dist(next,prev);

            prev[0] = next[0];
            prev[1] = next[1];
            prev[2] = next[2];

            s      += step;
        }
        length += dist(p1,prev);        

        return length;

    }

    template<class T,class TT>
    std::vector<float> hermiteLengthWithSpeed(T p0, TT T0, T p1, TT T1, int div) {

        double a[3], b[3];
        for (int i=0; i<3; i++) {
            a[i] =  2*p0[i] +  T0[i] - 2*p1[i] + T1[i];
            b[i] = -3*p0[i] -2*T0[i] + 3*p1[i] - T1[i];
        }

        double step   = 1.0f/double(div);
        double s      = step;

        float prev[3] = {p0[0],p0[1],p0[2]};
        float next[3];
        float length  = 0;
        std::vector<float> out;

        for (int i=1; i<div; i++) {

            next[0] = a[0]*s*s*s + b[0]*s*s + T0[0]*s + p0[0];
            next[1] = a[1]*s*s*s + b[1]*s*s + T0[1]*s + p0[1];
            next[2] = a[2]*s*s*s + b[2]*s*s + T0[2]*s + p0[2];

            length += dist(next,prev);
            out.push_back(length);

            prev[0] = next[0];
            prev[1] = next[1];
            prev[2] = next[2];

            s += step;

        }

        length += dist(p1,prev);
        out.push_back(length);      

        return out;

    }
    
}