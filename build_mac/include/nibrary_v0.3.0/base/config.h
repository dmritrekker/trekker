#pragma once

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <complex>
#include <set>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#undef max
#undef min
#else
#include <unistd.h>
#endif

#include "base/nibr.h"
#include "verbose.h"
#include "byteSwapper.h"
#include "multithreader.h"
#include "dataTypeHandler.h"

namespace NIBR 
{

    VERBOSE_LEVEL& VERBOSE();

    void INITIALIZE();
    void TERMINATE();
    
    int  RUNTIME();
    int  MSECRUNTIME();
    void TIC();
    void TOC();
    void TOC(MESSAGE msg);

    typedef enum {
        UNKNOWNSPACEUNIT,
        METER,
        MM,
        MICRON
    } SPACEUNIT;

    typedef enum {
        UNKNOWNTIMEUNIT,
        SEC,
        MSEC,
        USEC,
        HZ,
        PPM,
        RADS
    } TIMEUNIT;

}
