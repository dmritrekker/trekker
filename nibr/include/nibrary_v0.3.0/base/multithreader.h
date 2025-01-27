#pragma once

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#undef max
#else
#include <unistd.h>
#endif

#include <iostream>
#include <string>
#include <iomanip>
#include <thread>
#include <vector>
#include <memory>
#include <functional> 
#include <mutex>
#include <condition_variable>

#include "base/nibr.h"
#include "math/randomThings.h"

using namespace std;

namespace NIBR 
{

    namespace MT 
    {

        struct TASK {
            size_t   no;
            uint16_t threadId;
        };

        std::mutex&                                      PROC_MX();
        int&                                             MAXNUMBEROFTHREADS();
        void                                             SETMAXNUMBEROFTHREADS(int n);
        std::atomic<size_t>&                             FINISHEDTASKCOUNT();
        std::atomic<size_t>&                             FINISHEDTASKCOUNTTOSTOP();
        uint16_t&                                        FINISHEDTHREADID();
        std::vector<std::unique_ptr<NIBR::RandomDoer>>&  RNDM();

        void MTINIT();

        void MTRUN(size_t range, int numberOfThreads, std::function<void(TASK mttask)> f);
        void MTRUN(size_t range, int numberOfThreads, std::function<bool(TASK mttask)> f, size_t stopLim);

        void MTRUN(size_t range, int numberOfThreads, std::string message, std::function<void(TASK mttask)> f);
        void MTRUN(size_t range, int numberOfThreads, std::string message, std::function<bool(TASK mttask)> f, size_t stopLim);
        
        
        void MTRUN(size_t range, std::function<void(TASK mttask)> f);
        void MTRUN(size_t range, std::function<bool(TASK mttask)> f, size_t stopLim);

        void MTRUN(size_t range, std::string message, std::function<void(TASK mttask)> f);
        void MTRUN(size_t range, std::string message, std::function<bool(TASK mttask)> f, size_t stopLim);

        std::vector<std::pair<int,int>> createTaskRange(int taskCount, int workerCount);

    }

}
