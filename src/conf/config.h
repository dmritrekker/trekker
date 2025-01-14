#pragma once

#include "nibrary.h"

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif

#include "CLI11.hpp"

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif


void parseCommon(int& numberOfThreads, std::string verbose);
bool parseForceOutput(std::string out_fname, bool force);
void setInfo(CLI::App* app, const std::string& information);
void displayHelp(const std::string& text);