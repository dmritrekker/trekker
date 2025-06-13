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

    if ((out_diff_fname!="")&&(!parseForceOutput(out_diff_fname,force)&&(!ensureVTKorTCK(out_diff_fname)))) return;
    if ((out_same_fname!="")&&(!parseForceOutput(out_same_fname,force)&&(!ensureVTKorTCK(out_same_fname)))) return;


    // Initialize readers
    NIBR::disp(MSG_DEBUG,"Initializing input reader");
    NIBR::TractogramReader inp_reader(inp_fname);
    if (!inp_reader.isReady()) return;

    NIBR::disp(MSG_DEBUG,"Initializing reference reader");
    NIBR::TractogramReader ref_reader(ref_fname);
    if (!ref_reader.isReady()) return;   


    // Search streamlines
    std::vector<bool> diff;
    diff.reserve(inp_reader.numberOfStreamlines);

    std::mutex modifier;

    int batch_count         = 0;
    int batch_size          = 1000;
    int tot_batch_count     = std::ceil(inp_reader.numberOfStreamlines / float(batch_size));
    
    inp_reader.reset();

    while (true) {

        NIBR::StreamlineBatch inp_batch = inp_reader.getNextStreamlineBatch(batch_size);

        if (inp_batch.empty()) break;

        batch_count++;
        if (batch_count>1) std::cout << "\033[A\r\033[K" << std::flush; // Clear previous line
        disp(MSG_INFO, "Processing batch %d / %d", batch_count, tot_batch_count);

        std::vector<bool> local_diff(inp_batch.size(),true); // Initialize all as different

        if (ref_reader.numberOfStreamlines > 0) {

            ref_reader.reset();

            NIBR::StreamlineBatch ref_batch;

            auto compare = [&](const NIBR::MT::TASK& task)->void {

                if (!local_diff[task.no]) return;

                const auto& inpStreamline = inp_batch[task.no];

                for (const auto& refStreamline : ref_batch) {
                    if (NIBR::areStreamlinesIdentical(inpStreamline, refStreamline)) {
                        std::lock_guard lock(modifier);
                        local_diff[task.no] = false;
                        return;
                    }
                }
                
            };

            while (true) {
                ref_batch = ref_reader.getNextStreamlineBatch(batch_size);
                if (ref_batch.size() == 0) break;
                NIBR::MT::MTRUN(inp_batch.size(), compare);
            }

        }

        diff.insert(diff.end(),local_diff.begin(),local_diff.end());
    }


    // Write output
    if (out_diff_fname!="") {
        std::vector<size_t> select;
        select.reserve(inp_reader.numberOfStreamlines);
        for (std::size_t n = 0; n < diff.size(); n++) {
            if (diff[n]) select.push_back(n);
        }
        disp(MSG_INFO,"Found %zu different streamlines.", select.size());
        disp(MSG_INFO,"Writing... %s", out_diff_fname.c_str());
        NIBR::writeTractogram(out_diff_fname,&inp_reader, select);
        disp(MSG_INFO,"Done");
    }

    if (out_same_fname!="") {
        std::vector<size_t> select;
        select.reserve(inp_reader.numberOfStreamlines);
        for (std::size_t n = 0; n < diff.size(); n++) {
            if (!diff[n]) select.push_back(n);
        }
        disp(MSG_INFO,"Found %zu same streamlines.", select.size());
        disp(MSG_INFO,"Writing... %s", out_same_fname.c_str());
        NIBR::writeTractogram(out_same_fname,&inp_reader, select);
        disp(MSG_INFO,"Done");
    }

    return;

}


void diff(CLI::App* app)
{

    app->description("finds different streamlines between two tractograms");

    app->add_option("<input_tractogram>",     inp_fname,          "Input tractogram (.vtk, .tck, .trk)")
        ->required()
        ->check(CLI::ExistingFile);
    
    app->add_option("<reference_tractogram>", ref_fname,          "Reference tractogram (.vtk, .tck, .trk)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("--out_diff, -d",         out_diff_fname,     "Output streamlines of input tractogram that does not exist in the reference (.vtk, .tck)");
    app->add_option("--out_same, -s",         out_same_fname,     "Output streamlines of input tractogram that exists in the reference (.vtk, .tck)");

    app->add_option("--numberOfThreads, -n",  numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",          verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",              force,              "Force overwriting of existing file");

    app->callback(run_diff);
    
}

