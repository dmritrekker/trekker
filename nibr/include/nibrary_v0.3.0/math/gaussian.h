#pragma once

#include "base/nibr.h"
#include "core.h"

namespace NIBR 
{

// Computations with sigma
template <typename T>
T gaussian1D(T x, T sigma) 
{
    T sigma_squared = sigma * sigma;
    T exponent = -(x * x) / (2.0 * sigma_squared); 
    return (1.0 / std::sqrt(2.0 * PI * sigma_squared)) * std::exp(exponent); 
}

template <typename T>
T gaussian2D(T r, T sigma)  
{
    T sigma_squared = sigma * sigma;
    T exponent = -(r * r) / (2.0 * sigma_squared); 
    return (1.0 / (2.0 * PI * sigma_squared)) * std::exp(exponent); 
}

template <typename T>
T gaussian3D(T r, T sigma)  
{
    T sigma_squared = sigma * sigma;
    T exponent = -(r * r) / (2.0 * sigma_squared); 
    return std::pow(sigma_squared * 2 * PI, -1.5) * std::exp(exponent); 
}



// Computations with sigma*sigma
template <typename T>
T gaussian1D_var(T x, T sigma_squared) 
{
    T exponent = -(x * x) / (2.0 * sigma_squared); 
    return (1.0 / std::sqrt(2.0 * PI * sigma_squared)) * std::exp(exponent); 
}

template <typename T>
T gaussian2D_var(T r, T sigma_squared)  
{
    T exponent = -(r * r) / (2.0 * sigma_squared); 
    return (1.0 / (2.0 * PI * sigma_squared)) * std::exp(exponent); 
}

template <typename T>
T gaussian3D_var(T r, T sigma_squared)  
{
    T exponent = -(r * r) / (2.0 * sigma_squared); 
    return std::pow(sigma_squared * 2 * PI, -1.5) * std::exp(exponent); 
}


class Gaussian3D_evaluator_4squaredDist {
private:
    std::vector<double> precomputedValues;
    int N;
    double delta;

public:

    Gaussian3D_evaluator_4squaredDist() : N(0), delta(0) {}

    Gaussian3D_evaluator_4squaredDist(double rrMax, double sigma, int numValues) {
        initialize(rrMax,sigma,numValues);
    }

    int getN() const { return N; }

    void initialize(double rrMax, double sigma, int numValues) {

        N     = numValues;
        delta = rrMax / double(numValues - 1);
        double sigma_squared = sigma*sigma;
        double c = std::pow(sigma_squared * 2 * PI, -1.5);

        precomputedValues.reserve(numValues);

        for (int i = 0; i < numValues; ++i) {
            double exponent = - (i*delta) / (2.0 * sigma_squared);
            precomputedValues.push_back(c * std::exp(exponent));
        }

    }

    double eval(double x) {
        int index = std::round(x/delta);
        index     = std::max(0, std::min(N-1, index));
        return precomputedValues[index];
    }
};


}