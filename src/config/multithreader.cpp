#include "multithreader.h"

namespace MT {

    std::condition_variable      exit_cv;
    std::mutex                   exit_mx;
    std::mutex                   proc_mx;
    float                        progress;
    
    int                          maxNumberOfThreads;
    uint16_t                     finishedThreadId;

}

void MT::MTINIT() {
#ifdef BUILD_FOR_WINDOWS
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    maxNumberOfThreads = sysinfo.dwNumberOfProcessors;
#else
    maxNumberOfThreads = sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

void MT::MTRUN(size_t range, int numberOfThreads, void (*f)(size_t begin_ind, size_t end_ind, uint16_t threadNo)) {
    
    if (numberOfThreads==0) {
        numberOfThreads=maxNumberOfThreads;
    }
    
    size_t chunkSize;
    if       (range<1000)  chunkSize = 20;
    else if  (range<10000) chunkSize = 50;
    else chunkSize = 128;

    size_t indRange  = range;
    size_t bind      = 0;
    size_t eind      = 0;
    size_t taskCount = 0;
    
    float  chkPrgrs  = float(chunkSize)/float(range)*100;
    progress         = 0;
    
    std::thread* task = new std::thread[numberOfThreads];
    
    std::unique_lock<std::mutex> lk(exit_mx);
    
    for (int i=0; i<numberOfThreads; i++) {
        if (eind < indRange) {
            bind = eind;
            eind = bind + chunkSize;
            if (eind>indRange) eind=indRange;
        } else {
            break;
        }
        task[i] = std::thread((*f), bind, eind, i);
        task[i].detach();
        taskCount++;
    }
    
    while(eind < indRange) {
        
        bind = eind;
        eind = bind + chunkSize;
        if (eind>indRange) eind=indRange;
        
        exit_cv.wait(lk);
        progress += chkPrgrs;
        task[finishedThreadId] = std::thread((*f), bind, eind, finishedThreadId);
        task[finishedThreadId].detach();
        proc_mx.unlock();
        
    }
    
    while (taskCount) {
        exit_cv.wait(lk);
        progress += chkPrgrs;
        taskCount--;
        proc_mx.unlock();
    }
    
}

void MT::MTRUN(size_t range, int numberOfThreads, std::string message, void (*f)(size_t begin_ind, size_t end_ind, uint16_t threadNo)) {
    
    if (numberOfThreads==0) {
        numberOfThreads=maxNumberOfThreads;
    }
    
    size_t chunkSize;
    if       (range<1000)  chunkSize = 20;
    else if  (range<10000) chunkSize = 50;
    else chunkSize = 128;

    size_t indRange  = range;
    size_t bind      = 0;
    size_t eind      = 0;
    size_t taskCount = 0;
    
    float  chkPrgrs  = float(chunkSize)/float(range)*100;
    progress         = 0;
    
    std::thread* task = new std::thread[numberOfThreads];
    
    std::unique_lock<std::mutex> lk(exit_mx);
    
    std::cout << message << ": 0%" << '\r' << std::flush;
    for (int i=0; i<numberOfThreads; i++) {
        if (eind < indRange) {
            bind = eind;
            eind = bind + chunkSize;
            if (eind>indRange) eind=indRange;
        } else {
            break;
        }
        task[i] = std::thread((*f), bind, eind, i);
        task[i].detach();
        taskCount++;
    }
    
    while(eind < indRange) {
        
        bind = eind;
        eind = bind + chunkSize;
        if (eind>indRange) eind=indRange;
        
        exit_cv.wait(lk);
        std::cout << "                                               " << '\r' << std::flush; // clean terminal
        std::cout << std::fixed << std::setprecision (2) << message << ": " << progress << "%" << '\r' << std::flush;
        progress += chkPrgrs;
        task[finishedThreadId] = std::thread((*f), bind, eind, finishedThreadId);
        task[finishedThreadId].detach();
        proc_mx.unlock();
        
    }
    
    while (taskCount) {
        exit_cv.wait(lk);
        progress += chkPrgrs;
        taskCount--;
        proc_mx.unlock();
    }
    std::cout << "                                                   " << '\r'      << std::flush; // clean terminal
    std::cout << message << ": 100%" << std::endl << std::flush;
    
}


void MT::MTRUN(size_t range, int numberOfThreads, std::function<void(size_t begin_ind, size_t end_ind, uint16_t threadNo)> f) {
    
    if (numberOfThreads==0) {
        numberOfThreads=maxNumberOfThreads;
    }
    
    size_t chunkSize;
    if       (range<1000)  chunkSize = 20;
    else if  (range<10000) chunkSize = 50;
    else chunkSize = 128;

    size_t indRange  = range;
    size_t bind      = 0;
    size_t eind      = 0;
    size_t taskCount = 0;
    
    float  chkPrgrs  = float(chunkSize)/float(range)*100;
    progress         = 0;
    
    std::thread* task = new std::thread[numberOfThreads];
    
    std::unique_lock<std::mutex> lk(exit_mx);
    
    for (int i=0; i<numberOfThreads; i++) {
        if (eind < indRange) {
            bind = eind;
            eind = bind + chunkSize;
            if (eind>indRange) eind=indRange;
        } else {
            break;
        }
        task[i] = std::thread(f, bind, eind, i);
        task[i].detach();
        taskCount++;
    }
    
    while(eind < indRange) {
        
        bind = eind;
        eind = bind + chunkSize;
        if (eind>indRange) eind=indRange;
        
        exit_cv.wait(lk);
        progress += chkPrgrs;
        task[finishedThreadId] = std::thread(f, bind, eind, finishedThreadId);
        task[finishedThreadId].detach();
        proc_mx.unlock();
        
    }
    
    while (taskCount) {
        exit_cv.wait(lk);
        progress += chkPrgrs;
        taskCount--;
        proc_mx.unlock();
    }
    
}


void MT::MTRUN(size_t range, int numberOfThreads, std::string message, std::function<void(size_t begin_ind, size_t end_ind, uint16_t threadNo)> f) {
    
    if (numberOfThreads==0) {
        numberOfThreads=maxNumberOfThreads;
    }
    
    size_t chunkSize;
    if       (range<1000)  chunkSize = 20;
    else if  (range<10000) chunkSize = 50;
    else chunkSize = 128;

    size_t indRange  = range;
    size_t bind      = 0;
    size_t eind      = 0;
    size_t taskCount = 0;
    
    float  chkPrgrs  = float(chunkSize)/float(range)*100;
    progress         = 0;
    
    std::thread* task = new std::thread[numberOfThreads];
    
    std::unique_lock<std::mutex> lk(exit_mx);
    
    std::cout << message << ": 0%" << '\r' << std::flush;
    for (int i=0; i<numberOfThreads; i++) {
        if (eind < indRange) {
            bind = eind;
            eind = bind + chunkSize;
            if (eind>indRange) eind=indRange;
        } else {
            break;
        }
        task[i] = std::thread(f, bind, eind, i);
        task[i].detach();
        taskCount++;
    }
    
    while(eind < indRange) {
        
        bind = eind;
        eind = bind + chunkSize;
        if (eind>indRange) eind=indRange;
        
        exit_cv.wait(lk);
        std::cout << "                                               " << '\r' << std::flush; // clean terminal
        std::cout << std::fixed << std::setprecision (2) << message << ": " << progress << "%" << '\r' << std::flush;
        progress += chkPrgrs;
        task[finishedThreadId] = std::thread(f, bind, eind, finishedThreadId);
        task[finishedThreadId].detach();
        proc_mx.unlock();
        
    }
    
    while (taskCount) {
        exit_cv.wait(lk);
        progress += chkPrgrs;
        taskCount--;
        proc_mx.unlock();
    }
    std::cout << "                                                   " << '\r'      << std::flush; // clean terminal
    std::cout << message << ": 100%" << std::endl << std::flush;
    
}


void MT::MTRUN(size_t range, int numberOfThreads, std::function<void(MTTASK mttask)> f) {
    
    if (numberOfThreads==0) {
        numberOfThreads=maxNumberOfThreads;
    }
    
    size_t chunkSize;
    if       (range<1000)  chunkSize = 20;
    else if  (range<10000) chunkSize = 50;
    else chunkSize = 128;

    size_t indRange  = range;
    size_t bind      = 0;
    size_t eind      = 0;
    size_t taskCount = 0;
    int    i         = 0;
    
    float  chkPrgrs  = float(chunkSize)/float(range)*100;
    progress         = 0;
    
    
    std::shared_ptr<std::thread[]> task (new std::thread[numberOfThreads]);
    
    std::unique_lock<std::mutex> lk(exit_mx);
    
    
    auto taskRunner = [&](size_t begin_ind, size_t end_ind, uint16_t threadNo)->void {
        for (size_t i=begin_ind; i<end_ind; i++) {
            f({i,threadNo});
        }

        MT::proc_mx.lock();
        MT::finishedThreadId = threadNo;
        std::lock_guard<std::mutex> lk(MT::exit_mx);
        MT::exit_cv.notify_all();
    };
        
    for (i=0; i<numberOfThreads; i++) {
        if (eind < indRange) {
            bind = eind;
            eind = bind + chunkSize;
            if (eind>indRange) eind=indRange;
        } else {
            break;
        }            
            
        task[i] = std::thread(taskRunner, bind, eind, i);
        task[i].detach();
        taskCount++;
    }
    
    while(eind < indRange) {
        
        bind = eind;
        eind = bind + chunkSize;
        if (eind>indRange) eind=indRange;
        
        exit_cv.wait(lk);
        progress += chkPrgrs;
        task[finishedThreadId] = std::thread(taskRunner, bind, eind, finishedThreadId);
        task[finishedThreadId].detach();
        proc_mx.unlock();
        
    }
    
    while (taskCount) {
        exit_cv.wait(lk);
        progress += chkPrgrs;
        taskCount--;
        proc_mx.unlock();
    }
    
}

void MT::MTRUN(size_t range, int chunkSize, int numberOfThreads, std::function<void(MTTASK mttask)> f) {
    
    if (numberOfThreads==0) {
        numberOfThreads=maxNumberOfThreads;
    }

    size_t indRange  = range;
    size_t bind      = 0;
    size_t eind      = 0;
    size_t taskCount = 0;
    int    i         = 0;
    
    float  chkPrgrs  = float(chunkSize)/float(range)*100;
    progress         = 0;
    
    
    std::shared_ptr<std::thread[]> task (new std::thread[numberOfThreads]);
    
    std::unique_lock<std::mutex> lk(exit_mx);
    
    
    auto taskRunner = [&](size_t begin_ind, size_t end_ind, uint16_t threadNo)->void {
        for (size_t i=begin_ind; i<end_ind; i++) {
            f({i,threadNo});
        }

        MT::proc_mx.lock();
        MT::finishedThreadId = threadNo;
        std::lock_guard<std::mutex> lk(MT::exit_mx);
        MT::exit_cv.notify_all();
    };
        
    for (i=0; i<numberOfThreads; i++) {
        if (eind < indRange) {
            bind = eind;
            eind = bind + chunkSize;
            if (eind>indRange) eind=indRange;
        } else {
            break;
        }            
            
        task[i] = std::thread(taskRunner, bind, eind, i);
        task[i].detach();
        taskCount++;
    }
    
    while(eind < indRange) {
        
        bind = eind;
        eind = bind + chunkSize;
        if (eind>indRange) eind=indRange;
        
        exit_cv.wait(lk);
        progress += chkPrgrs;
        task[finishedThreadId] = std::thread(taskRunner, bind, eind, finishedThreadId);
        task[finishedThreadId].detach();
        proc_mx.unlock();
        
    }
    
    while (taskCount) {
        exit_cv.wait(lk);
        progress += chkPrgrs;
        taskCount--;
        proc_mx.unlock();
    }
    
}


void MT::MTRUN(size_t range, int numberOfThreads, std::string message, std::function<void(MTTASK mttask)> f) {
    
    if (numberOfThreads==0) {
        numberOfThreads=maxNumberOfThreads;
    }
    
    size_t chunkSize;
    if       (range<1000)  chunkSize = 20;
    else if  (range<10000) chunkSize = 50;
    else chunkSize = 128;

    size_t indRange  = range;
    size_t bind      = 0;
    size_t eind      = 0;
    size_t taskCount = 0;
    int    i         = 0;
    
    float  chkPrgrs  = float(chunkSize)/float(range)*100;
    progress         = 0;
    
    std::shared_ptr<std::thread[]> task (new std::thread[numberOfThreads]);
    
    std::unique_lock<std::mutex> lk(exit_mx);
    
    
    auto taskRunner = [&](size_t begin_ind, size_t end_ind, uint16_t threadNo)->void {
        for (size_t i=begin_ind; i<end_ind; i++) {
            f({i,threadNo});
        }

        MT::proc_mx.lock();
        MT::finishedThreadId = threadNo;
        std::lock_guard<std::mutex> lk(MT::exit_mx);
        MT::exit_cv.notify_all();
    };
        
    std::cout << message << ": 0%" << '\r' << std::flush;
    for (i=0; i<numberOfThreads; i++) {
        if (eind < indRange) {
            bind = eind;
            eind = bind + chunkSize;
            if (eind>indRange) eind=indRange;
        } else {
            break;
        }            
            
        task[i] = std::thread(taskRunner, bind, eind, i);
        task[i].detach();
        taskCount++;
    }
    
    while(eind < indRange) {
        
        bind = eind;
        eind = bind + chunkSize;
        if (eind>indRange) eind=indRange;
        
        exit_cv.wait(lk);
        std::cout << "                                               " << '\r' << std::flush; // clean terminal
        std::cout << std::fixed << std::setprecision (2) << message << ": " << progress << "%" << '\r' << std::flush;
        progress += chkPrgrs;
        task[finishedThreadId] = std::thread(taskRunner, bind, eind, finishedThreadId);
        task[finishedThreadId].detach();
        proc_mx.unlock();
        
    }
    
    while (taskCount) {
        exit_cv.wait(lk);
        progress += chkPrgrs;
        taskCount--;
        proc_mx.unlock();
    }
    std::cout << "                                                   " << '\r'      << std::flush; // clean terminal
    std::cout << message << ": 100%" << std::endl << std::flush;
    
}

void MT::MTRUN(size_t range, int chunkSize, int numberOfThreads, std::string message, std::function<void(MTTASK mttask)> f) {
    
    if (numberOfThreads==0) {
        numberOfThreads=maxNumberOfThreads;
    }

    size_t indRange  = range;
    size_t bind      = 0;
    size_t eind      = 0;
    size_t taskCount = 0;
    int    i         = 0;
    
    float  chkPrgrs  = float(chunkSize)/float(range)*100;
    progress         = 0;
    
    std::shared_ptr<std::thread[]> task (new std::thread[numberOfThreads]);
    
    std::unique_lock<std::mutex> lk(exit_mx);
    
    
    auto taskRunner = [&](size_t begin_ind, size_t end_ind, uint16_t threadNo)->void {
        for (size_t i=begin_ind; i<end_ind; i++) {
            f({i,threadNo});
        }

        MT::proc_mx.lock();
        MT::finishedThreadId = threadNo;
        std::lock_guard<std::mutex> lk(MT::exit_mx);
        MT::exit_cv.notify_all();
    };
        
    std::cout << message << ": 0%" << '\r' << std::flush;
    for (i=0; i<numberOfThreads; i++) {
        if (eind < indRange) {
            bind = eind;
            eind = bind + chunkSize;
            if (eind>indRange) eind=indRange;
        } else {
            break;
        }            
            
        task[i] = std::thread(taskRunner, bind, eind, i);
        task[i].detach();
        taskCount++;
    }
    
    while(eind < indRange) {
        
        bind = eind;
        eind = bind + chunkSize;
        if (eind>indRange) eind=indRange;
        
        exit_cv.wait(lk);
        std::cout << "                                               " << '\r' << std::flush; // clean terminal
        std::cout << std::fixed << std::setprecision (2) << message << ": " << progress << "%" << '\r' << std::flush;
        progress += chkPrgrs;
        task[finishedThreadId] = std::thread(taskRunner, bind, eind, finishedThreadId);
        task[finishedThreadId].detach();
        proc_mx.unlock();
        
    }
    
    while (taskCount) {
        exit_cv.wait(lk);
        progress += chkPrgrs;
        taskCount--;
        proc_mx.unlock();
    }
    std::cout << "                                                   " << '\r'      << std::flush; // clean terminal
    std::cout << message << ": 100%" << std::endl << std::flush;
    
    
    
}
