#pragma once 

#include "conf/config.h"
#include "conf/auxiliary.h"

void info(CLI::App* app);
void track(CLI::App* app);
void filter(CLI::App* app);
void track2img(CLI::App* app);
void track2surf(CLI::App* app);
void img2track(CLI::App* app);
void select(CLI::App* app);
void resample(CLI::App* app);
void convert(CLI::App* app);
void transform(CLI::App* app);
void diff(CLI::App* app);
void merge(CLI::App* app);
void addColor(CLI::App* app);
void fieldExport(CLI::App* app);
void fieldImport(CLI::App* app);
void fieldRemove(CLI::App* app);
void dMRI_cmd(CLI::App* app);
void dMRI_recon(CLI::App* app);
void dMRI_recon_transhi2015(CLI::App* app);
