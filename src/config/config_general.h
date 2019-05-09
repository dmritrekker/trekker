#ifndef SRC_CONFIG_CONFIG_GENERAL_H_
#define SRC_CONFIG_CONFIG_GENERAL_H_

#ifdef BUILD_FOR_WINDOWS
#include <windows.h>
#endif

#ifdef ENABLE_MULTITHREADING
#include <pthread.h>
#include <semaphore.h>
#endif

#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <unistd.h>
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

#ifdef ENABLE_MULTITHREADING
extern sem_t 			exit_sem;
extern pthread_mutex_t  lock;
#endif

int    runTime();
void   setDefaultParametersWhenNecessary();
void   print();

}

#endif
