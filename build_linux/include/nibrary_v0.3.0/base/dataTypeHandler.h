#pragma once

#include "base/nibr.h"
#include <iostream>
#include <typeinfo>
#include <unordered_map>
#include <string>
#include <memory>
#include <complex>

namespace NIBR 
{

    typedef enum {
        UNKNOWN_DT,
        BOOL_DT,
        INT8_DT,
        UINT8_DT,
        INT16_DT,
        UINT16_DT,
        INT32_DT,
        UINT32_DT,
        INT64_DT,
        UINT64_DT,
        FLOAT32_DT,
        FLOAT64_DT,
        FLOAT128_DT,
        COMPLEX64_DT,
        COMPLEX128_DT,
        COMPLEX256_DT
    } DATATYPE;

    std::string getTypeName(const std::type_info& T);
    DATATYPE    getTypeId(std::string type);
    DATATYPE    getTypeId(const std::type_info& T);

    

}
