#include "cmd.h"

using namespace NIBR;

namespace CMDARGS_MERGE {
    std::string  inp1_fname;
    std::string  inp2_fname;
    std::string  out_fname = "";
    
    bool checkDuplicates = false;

    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;
}

using namespace CMDARGS_MERGE;


void run_merge()
{

    parseCommon(numberOfThreads,verbose);
    if (!parseForceOutput(out_fname,force)) return;

    if(!ensureVTKorTCK(inp1_fname))      return;
    if(!ensureVTKorTCK(inp2_fname))      return;


    NIBR::disp(MSG_DEBUG,"Initializing tractogram 1");
    NIBR::TractogramReader inp1;
    if (!inp1.initReader(inp1_fname)) {
        std::cout << "Can't read " << inp1_fname << std::endl << std::flush;
        return;
    }

    NIBR::disp(MSG_DEBUG,"Initializing tractogram 2");
    NIBR::TractogramReader inp2;
    if (!inp2.initReader(inp2_fname)) {
        std::cout << "Can't read " << inp2_fname << std::endl << std::flush;
        return;
    }

    NIBR::disp(MSG_DEBUG,"Running merge");
    auto merged = NIBR::tractogramMerge(&inp1,&inp2,checkDuplicates);

    NIBR::disp(MSG_DEBUG,"Writing tractogram");
    NIBR::writeTractogram(out_fname, merged);

}


void merge(CLI::App* app)
{

    app->description("merges two tractograms, optionally ignoring duplicates");

    app->add_option("<tractogram_1>", inp1_fname, "Input tractogram (.vtk, .tck)")
        ->required()
        ->check(CLI::ExistingFile);
    
    app->add_option("<tractogram_2>", inp2_fname, "Input tractogram (.vtk, .tck)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("<output>", out_fname, "Output streamlines of input tractogram that does not exist in the reference (.vtk, .tck)")
        ->required();

    app->add_flag("--checkDuplicates, -c",   checkDuplicates,    "Checks for duplicates and ignores them when merging");

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quite\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");

    app->callback(run_merge);
    
}

