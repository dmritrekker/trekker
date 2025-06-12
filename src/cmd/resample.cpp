#include "cmd.h"
#include <algorithm>

using namespace NIBR;

namespace CMDARGS_RESAMPLE {
    std::string inp_fname;
    std::string out_fname;

    CLI::Option* sizeOpt   = NULL;
    CLI::Option* countOpt  = NULL;

    float stepSize;
    int   stepCount;

    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;
}

using namespace CMDARGS_RESAMPLE;


void run_resample()
{

    parseCommon(numberOfThreads, verbose);
    if (!parseForceOutput(out_fname, force)) return;
    if (!ensureVTKorTCK(out_fname)) return;

    int optCounter = 0;
    if (*sizeOpt)   optCounter++;
    if (*countOpt)  optCounter++;

    if ((optCounter == 0) || (optCounter > 1)) {
        std::cout << "Only one option allowed. Use either \"stepSize\" or \"stepCount\"." << std::endl << std::flush;
        return;
    }

    NIBR::TractogramReader reader(inp_fname);
    if (!reader.isReady()) return;

    NIBR::TractogramWriter writer(out_fname);
    if (!writer.open()) {
        disp(MSG_FATAL, "Failed to open output file: %s", out_fname.c_str());
        return;
    }
    
    int          batch_size  = 50000;
    size_t       batch_count = 0;

    while (true) {

        NIBR::StreamlineBatch input_batch = reader.getNextStreamlineBatch(batch_size);

        if (input_batch.empty()) break;

        NIBR::MT::SET_DISP_RANGE(batch_count*batch_size,reader.numberOfStreamlines);

        NIBR::StreamlineBatch output_batch = resampleBatch(input_batch, stepSize, stepCount, (*sizeOpt) ? true : false );

        if (!writer.writeBatch(std::move(output_batch))) {
            disp(MSG_ERROR, "Failed to write batch %d.", batch_count);
            break;
        }

        batch_count++;
        
    }

    if (!writer.close()) {
        disp(MSG_ERROR, "Failed to finalize and close output file.");
    } else {
        disp(MSG_INFO, "Processing finished successfully.");
    }

    return;
}
  
  
void resample(CLI::App* app) 
{  

    app->description("resamples streamlines in a tractogram");

    app->add_option("<input_tractogram>",            inp_fname,          "Input tractogram (.vtk, .tck, .trk)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("<output_tractogram>",           out_fname,          "Output tractogram (.vtk, .tck)")
        ->required();   
    
    sizeOpt   = app->add_option("--stepSize, -s",   stepSize,           "Resampling is done based on the given step size. The last segment of a streamline might have a shorter step size.");
    countOpt  = app->add_option("--stepCount, -c",  stepCount,          "Resampling is done so each streamline has the same number of points.");

    app->add_option("--numberOfThreads, -n",        numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",                verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",                    force,              "Force overwriting of existing file");

    app->callback(run_resample);
    
} 

