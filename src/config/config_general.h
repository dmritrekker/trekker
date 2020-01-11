#ifndef SRC_CONFIG_CONFIG_GENERAL_H_
#define SRC_CONFIG_CONFIG_GENERAL_H_

#ifdef BUILD_FOR_WINDOWS
#include <windows.h>
#include <io.h>
#undef max
#else
#include <unistd.h>
#endif

#include <thread>
#include <mutex>
#include <condition_variable>

#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include "notset.h"

#define MAXTIMELIMIT std::numeric_limits<int>::max()

typedef enum {
	VERBOSELEVEL_NOTSET = -1,
	QUITE,
	MINIMAL,
	ON,
	DETAILED,
	ALL
} VerboseLevel;

namespace GENERAL {

extern VerboseLevel  		 					verboseLevel;
extern int          							numberOfThreads;
extern time_t  									startDate;
extern std::chrono::steady_clock::time_point  	startTime;
extern int 									    timeLimit;
extern bool 									usingAPI;

extern std::condition_variable 	exit_cv;
extern std::mutex               exit_mx;
extern std::mutex               tracker_lock;
extern size_t                   ready_thread_id;
extern int                      lineCountToFlush;

extern int                      genCnt;

int    runTime();
void   setDefaultParametersWhenNecessary();
void   print();

}

#endif
