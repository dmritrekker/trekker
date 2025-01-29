#pragma once

#include "base/nibr.h"
#include "math/core.h"
#include <sstream>

namespace NIBR 
{

    double isNumber(const std::string &s);

    std::vector<std::string> splitString (const std::string &s, char delim);
    
    std::tuple<bool,Point,float> getCenterAndRadius(std::string inp);

    template <typename T>
    std::string to_string_with_precision(const T val, const int n = 4)
    {
        std::ostringstream out;
        out.precision(n);
        out << std::fixed << val;
        return std::move(out).str();
    }
    
}