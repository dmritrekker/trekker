#ifndef SRC_MULTITHREADER_H_
#define SRC_MULTITHREADER_H_

#ifdef BUILD_FOR_WINDOWS
#include <windows.h>
#include <io.h>
#undef max
#else
#include <unistd.h>
#endif


#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <thread>
#include <functional> 
#include <mutex>
#include <condition_variable>

struct MTTASK {
    size_t   no;
    uint16_t threadId;
};

namespace MT {
    
extern std::condition_variable      exit_cv;
extern std::mutex                   exit_mx;
extern std::mutex                   proc_mx;
extern float                        progress;
extern int                          maxNumberOfThreads;
extern uint16_t                     finishedThreadId;

extern void MTINIT();

extern void MTRUN(size_t range, int numberOfThreads, void (*f)(size_t begin_ind, size_t end_ind, uint16_t threadNo));
extern void MTRUN(size_t range, int numberOfThreads, std::string message, void (*f)(size_t begin_ind, size_t end_ind, uint16_t threadNo));

extern void MTRUN(size_t range, int numberOfThreads, std::function<void(size_t begin_ind, size_t end_ind, uint16_t threadNo)> f);
extern void MTRUN(size_t range, int numberOfThreads, std::string message, std::function<void(size_t begin_ind, size_t end_ind, uint16_t threadNo)> f);

extern void MTRUN(size_t range, int numberOfThreads, std::function<void(MTTASK mttask)> f);
extern void MTRUN(size_t range, int chunkSize, int numberOfThreads, std::function<void(MTTASK mttask)> f);

extern void MTRUN(size_t range, int numberOfThreads, std::string message, std::function<void(MTTASK mttask)> f);
extern void MTRUN(size_t range, int chunkSize, int numberOfThreads, std::string message, std::function<void(MTTASK mttask)> f);


}

#endif
