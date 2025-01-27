#pragma once

#include "base/nibr.h"
#include "../algorithms/tractographyAlgorithm.h"
#include "../algorithms/ptt/algorithm_ptt_params.h"
#include "../seed/seed.h"
#include "../pathway/pathway.h"
#include "../io/tractogramField.h"

#define DEFAULT_IDLETIMELIMIT 120 // If not streamline is computed in the last 120 sec idleTimeLimitReached will be set to true

using namespace std;

namespace NIBR {

namespace TRACKER 
{

// User parameters
extern Algorithm_Type  algorithm;
extern int             nThreads;
extern int             runTimeLimit;
extern int             idleTimeLimit;
extern Seed            seed;
extern bool            saveSeedIndex;
extern Pathway 		   pw;
extern Params_PTT      params_ptt;

// Derived parameters
extern std::vector<std::vector<std::vector<float>>> tractogram;
extern std::vector<int>                             seedIndex;
extern TractogramField                              seedIndexField;
extern std::chrono::steady_clock::time_point        initTime;
extern std::chrono::steady_clock::time_point        lastTime;
extern bool                                         runtimeLimitReached;  // time passed since the TRACKER was initialized or reset
extern bool                                         idletimeLimitReached; // time passed since the last successful streamline was computed and appended on the tractogram
extern bool                                         countIsReached;
extern int                                          ready_thread_id;

// Loggers
extern std::atomic<size_t> log_success_REACHED_MAXLENGTH_LIMIT;
extern std::atomic<size_t> log_success_REACHED_MINDATASUPPORT_LIMIT;
extern std::atomic<size_t> log_success_SATISFIED_PATHWAY_RULES;

extern std::atomic<size_t> log_discard_TOO_SHORT;
extern std::atomic<size_t> log_discard_TOO_LONG;
extern std::atomic<size_t> log_discard_DISCARD_ROI_REACHED;
extern std::atomic<size_t> log_discard_REQUIRED_ROI_NOT_MET;
extern std::atomic<size_t> log_discard_REQUIRED_ROI_ORDER_NOT_MET;
extern std::atomic<size_t> log_discard_CANT_MEET_STOP_CONDITION;
extern std::atomic<size_t> log_discard_ENDED_INSIDE_DISCARD_ROI;
extern std::atomic<size_t> log_discard_REACHED_TIME_LIMIT;

extern std::atomic<size_t> log_failed_UNKNOWN_REASON;
extern std::atomic<size_t> log_failed_BY_THE_ALGORITHM_AT_INITIALIZATION;
extern std::atomic<size_t> log_failed_BY_THE_ALGORITHM;

extern std::atomic<size_t> log_unexpected_TRACKING_STATUS;

std::vector<std::vector<std::vector<float>>>& getTractogram();
TractogramField& getSeedIndexField();

int  runTime();
int  idleTime();
bool isWithinTimeLimits();

void reset();
void print();

}

}