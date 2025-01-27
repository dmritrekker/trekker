#include "config.h"

#include <iostream>
#include <cstdio>
#ifdef BUILD_FOR_WINDOWS
#include <windows.h>
#define popen _popen
#define pclose _pclose
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

void displayHelp(const std::string& text) {

    std::cout << "Print test display help\n" << std::flush;

    if (isatty(fileno(stdout))) {
        
        std::cout << "Print test display isatty\n" << std::flush;

        #ifdef BUILD_FOR_WINDOWS
        FILE* f = popen("more", "w");
        #else
        std::cout << "Print test display popen start\n" << std::flush;
        FILE* f = popen("less", "w");
        std::cout << "Print test display popen end\n" << std::flush;
        #endif

        if (f) {
            std::cout << "Print test display pclose start\n" << std::flush;
            fwrite(text.c_str(), 1, text.size(), f);
            pclose(f);
            std::cout << "Print test display pclose end\n" << std::flush;
        } else {
            std::cout << text << std::endl;
        }
    } else {
        std::cout << text << std::endl;
    }
    std::cout << "Help done\n" << std::flush;
}


void parseCommon(int& numberOfThreads, std::string verbose)
{

    NIBR::MT::SETMAXNUMBEROFTHREADS((numberOfThreads == 0) ? NIBR::MT::MAXNUMBEROFTHREADS() : numberOfThreads);

    numberOfThreads = NIBR::MT::MAXNUMBEROFTHREADS();

    if (verbose=="quite") {
        NIBR::VERBOSE() = VERBOSE_QUITE;
    } else if (verbose=="fatal") {
        NIBR::VERBOSE() = VERBOSE_FATAL;
    } else if (verbose=="error") {
        NIBR::VERBOSE() = VERBOSE_ERROR;
    } else if (verbose=="warn") {
        NIBR::VERBOSE() = VERBOSE_WARN;
    } else if (verbose=="info") {
        NIBR::VERBOSE() = VERBOSE_INFO;
    } else if (verbose=="detail") {
        NIBR::VERBOSE() = VERBOSE_DETAIL;
    } else if (verbose=="debug") {
        numberOfThreads              = 1;
        NIBR::MT::SETMAXNUMBEROFTHREADS(1);
        NIBR::VERBOSE()                = VERBOSE_DEBUG;
    } else {
        std::cout << "Unknown verbose level " << verbose << ". Using \"info\"" << std::endl << std::flush;
    }

}

bool parseForceOutput(std::string out_fname, bool force)
{
    if ((force==false) && (existsFile(out_fname))) {
        disp(MSG_ERROR,"Output file, %s, already exists. Use --force or -f to overwrite it.", out_fname.c_str());   
        return false;
    }
    return true;
}

void setInfo(CLI::App* app, const std::string& info) {
    std::string preamble = "\n------------------------------------------------------------\n\nDescription:\n\n";    
    app->footer(preamble + info + "\n\n" + "----------Trekker v1.0.0-rc3---------\n© Copyright 2025, Dogu Baran Aydogan, baran.aydogan@uef.fi\n");
}
