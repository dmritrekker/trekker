#include "config_general.h"
#include <limits>

namespace GENERAL {

VerboseLevel  							verboseLevel 		= VERBOSELEVEL_NOTSET;
int           							numberOfThreads 	= NOTSET;
time_t  								startDate;
std::chrono::steady_clock::time_point 	startTime 			= std::chrono::steady_clock::now();
int 									timeLimit           = NOTSET;
bool 									usingAPI 			= false;


std::condition_variable     exit_cv;
std::mutex                  exit_mx;
std::mutex                  tracker_lock;
size_t                      ready_thread_id;
int                         lineCountToFlush;


int runTime() {
	return int(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()-startTime).count());
}


void setDefaultParametersWhenNecessary() {

	// Handle verboseLevel
	if (verboseLevel==VERBOSELEVEL_NOTSET) 	verboseLevel 	= MINIMAL;

	// Handle numberOfThreads
	if ( (verboseLevel>ON) && (numberOfThreads != 1) ) {
		if (verboseLevel!=QUITE) std::cout << std::endl << "-numberOfThreads option is ignored and forced to be 1 since verbose level is bigger than 1." << std::endl;
		numberOfThreads = 1;
	} else {
		if (numberOfThreads==NOTSET) {
#ifdef BUILD_FOR_WINDOWS
			SYSTEM_INFO sysinfo;
			GetSystemInfo(&sysinfo);
			numberOfThreads = sysinfo.dwNumberOfProcessors;
#else
			numberOfThreads = sysconf(_SC_NPROCESSORS_ONLN);
#endif
		}
	}

	if (timeLimit<=0) {
		if (verboseLevel!=QUITE) std::cout << "Setting time limit to infinite" << std::endl;
		timeLimit = MAXTIMELIMIT; // no time limit by default
	}

}


void print() {
	std::cout << std::endl;
	std::cout << "GENERAL OPTIONS"<< std::endl;
	std::cout << "Verbose level         : "  << verboseLevel 		<< std::endl;
	std::cout << "Number of threads     : "  << numberOfThreads 	<< std::endl;
	std::cout << "Time limit            : ";
	if (timeLimit==MAXTIMELIMIT)
		std::cout << "infinite minutes";
	else {
		float tmp = float(timeLimit)/60.0;
		std::cout << tmp;
		if (tmp<=1)
			std::cout << " minute" << std::endl;
		else
			std::cout << " minutes" << std::endl;
	}
	std::cout << std::endl;

}

}

