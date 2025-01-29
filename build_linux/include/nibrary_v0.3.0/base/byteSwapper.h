#pragma once

#include "base/nibr.h"
#include <algorithm>

namespace NIBR 
{

    template<typename T>
    inline void swapByteOrder(T& a) 
    {
        char* byteArray = reinterpret_cast<char*>(&a);
        for(std::size_t i = 0; i < (sizeof(a)/2); i++)
            std::swap(byteArray[sizeof(a) - 1 - i],byteArray[i]);
    }

}
