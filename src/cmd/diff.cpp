#include "cmd.h"

using namespace NIBR;

namespace CMDARGS_DIFF {
    std::string  inp_fname;
    std::string  ref_fname;
    std::string  out_diff_fname = "";
    std::string  out_same_fname = "";

    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;
}

using namespace CMDARGS_DIFF;


void run_diff()
{

    parseCommon(numberOfThreads,verbose);

    if (out_diff_fname=="" && out_same_fname=="") {
        std::cout << "Either out_diff or out_same has to be defined. "<< std::endl << std::flush;
        return;
    }
    if ((out_diff_fname!="")&&(!parseForceOutput(out_diff_fname,force))) return;
    if ((out_same_fname!="")&&(!parseForceOutput(out_same_fname,force))) return;

    if(!ensureVTKorTCK(inp_fname))      return;
    if(!ensureVTKorTCK(ref_fname))      return;
    if( (out_diff_fname!="") && !ensureVTKorTCK(out_diff_fname)) return;
    if( (out_same_fname!="") && !ensureVTKorTCK(out_same_fname)) return;



    NIBR::disp(MSG_DEBUG,"Initializing input reader");
    NIBR::TractogramReader inp_tractogram;
    if (!inp_tractogram.initReader(inp_fname)) {
        std::cout << "Can't read " << inp_fname << std::endl << std::flush;
        return;
    }

    NIBR::disp(MSG_DEBUG,"Initializing reference reader");
    NIBR::TractogramReader ref_tractogram;
    if (!ref_tractogram.initReader(ref_fname)) {
        std::cout << "Can't read " << ref_fname << std::endl << std::flush;
        return;
    }

    NIBR::disp(MSG_DEBUG,"Running diff");
    auto diff = NIBR::tractogramDiff(&inp_tractogram,&ref_tractogram);

    NIBR::disp(MSG_DEBUG,"Writing tractogram");
    if (out_diff_fname!="") NIBR::writeTractogram(out_diff_fname, &inp_tractogram, std::get<0>(diff));
    if (out_same_fname!="") NIBR::writeTractogram(out_same_fname, &inp_tractogram, std::get<1>(diff));

}


void diff(CLI::App* app)
{

    app->description("finds different streamlines between two tractograms");

    app->add_option("<input_tractogram>", inp_fname, "Input tractogram (.vtk, .tck)")
        ->required()
        ->check(CLI::ExistingFile);
    
    app->add_option("<reference_tractogram>", ref_fname, "Reference tractogram (.vtk, .tck)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("--out_diff, -d", out_diff_fname, "Output streamlines of input tractogram that does not exist in the reference (.vtk, .tck)");
    app->add_option("--out_same, -s", out_same_fname, "Output streamlines of input tractogram that exists in the reference (.vtk, .tck)");

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");

    app->callback(run_diff);
    
}

