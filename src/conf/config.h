#pragma once

#include "nibrary.h"

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#include "CLI11.hpp"
#pragma GCC diagnostic pop

#else

#include "CLI11.hpp"

#endif

void parseCommon(int& numberOfThreads, std::string verbose);
bool parseForceOutput(std::string out_fname, bool force);
void setInfo(CLI::App* app, const std::string& information);
void displayHelp(const std::string& text);