#pragma once

#include "base/nibr.h"
#include "image/image.h"

namespace NIBR
{
    void imgEDT(NIBR::Image<bool>*  inp, NIBR::Image<float>* out);
    void imgEDT(NIBR::Image<int>*   inp, NIBR::Image<float>* out);
    void imgEDT(NIBR::Image<float>* inp, NIBR::Image<float>* out);
    void imgEDT(NIBR::Image<float>* inp);  // Overwrites input
}