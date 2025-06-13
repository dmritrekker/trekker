#include "cmd.h"

using namespace NIBR;

namespace CMDARGS_MERGE {
    std::vector<std::string>  inp_fnames;
    std::vector<std::string>  exc_fnames;
    std::string   out_fname = "";

    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;
}

using namespace CMDARGS_MERGE;

std::vector<std::string> resolveInputPatterns(const std::vector<std::string>& patterns_or_files) {

    // Create list of files to include
    std::vector<std::string> resolvedFiles;

    for (const std::string& inp : patterns_or_files) {
        std::vector<std::string> tmp = getMatchingFiles(inp);
        for (const std::string& f : tmp) {
            auto ext = getFileExtension(f);
            if ((ext == "vtk") || (ext == "tck") || (ext == "trk")){
                resolvedFiles.push_back(f);
            }
        }
    }

    // Remove duplicates
    if (!resolvedFiles.empty()) {
        std::sort(resolvedFiles.begin(), resolvedFiles.end());
        resolvedFiles.erase(std::unique(resolvedFiles.begin(), resolvedFiles.end()), resolvedFiles.end());
    }

    // Confirm existance of files
    std::vector<int> indicesToRemove;

    for (std::size_t n = 0; n < resolvedFiles.size(); n++) {
        std::ifstream testFile(resolvedFiles[n]);
        if (!testFile) {
            disp(MSG_WARN, "File not found or not accessible: %s", resolvedFiles[n].c_str());
            indicesToRemove.push_back(n);
        }
        testFile.close();
    }

    NIBR::removeIdx(resolvedFiles,indicesToRemove);

    return resolvedFiles;

}

void run_merge()
{

    parseCommon(numberOfThreads,verbose);
    if (!parseForceOutput(out_fname,force)) return;
    if (!ensureVTKorTCK(out_fname)) return;

    auto filesToInc = resolveInputPatterns(inp_fnames);
    auto filesToExc = resolveInputPatterns(exc_fnames);
    
    std::vector<int> indicesToExclude;
    for (std::size_t n = 0; n < filesToInc.size(); n++) {
        for (const auto& exc : filesToExc) {
            if (filesToInc[n] == exc) indicesToExclude.push_back(n);
        }
    }
    NIBR::removeIdx(filesToInc,indicesToExclude);

    if (filesToInc.empty()) {
        disp(MSG_INFO, "No input files found after resolving patterns.");
        return;
    }

    disp(MSG_DEBUG, "Found %zu unique tractogram files to merge:", filesToInc.size());
    for (size_t i = 0; i < filesToInc.size(); ++i) {
        disp(MSG_DEBUG, "  Input %zu: %s", i + 1, filesToInc[i].c_str());
    }

    // Merge tractograms
    TractogramWriter writer(out_fname);
    if (!writer.isValid()) return;

    if (!writer.open()) {
        disp(MSG_FATAL, "Failed to open output file for writing: %s", out_fname.c_str());
        return;
    }

    disp(MSG_INFO, "Starting to merge %zu tractogram(s) into %s...", filesToInc.size(), out_fname.c_str());

    for (size_t i = 0; i < filesToInc.size(); ++i) {

        const std::string& current_inp_fname = filesToInc[i];

        disp(MSG_INFO, "Processing input file (%zu/%zu): %s", i + 1, filesToInc.size(), current_inp_fname.c_str());

        TractogramReader reader(current_inp_fname);
        if (!reader.isReady()) {
            disp(MSG_ERROR, "Failed to initialize reader for %s. Skipping this file.", current_inp_fname.c_str());
            writer.close();
            return;
        }

        if (reader.numberOfStreamlines == 0) {
            disp(MSG_WARN, "Input file %s contains 0 streamlines.", current_inp_fname.c_str());
            continue;
        }

        reader.reset();

        int  batch_size                 = 50000;
        int  streamlines_in_batch       = 0;
        long streamlines_from_this_file = 0;
        int  batch_count_this_file      = 0;

        while (true) {

            NIBR::StreamlineBatch batch = reader.getNextStreamlineBatch(batch_size);
            streamlines_in_batch  = batch.size();

            if (streamlines_in_batch == 0) break;

            if (!writer.writeBatch(std::move(batch))) {
                disp(MSG_ERROR, "Failed to write batch from %s to output. Aborting merge.", current_inp_fname.c_str());
                writer.close();
                return;
            }
            streamlines_from_this_file += streamlines_in_batch;
            batch_count_this_file++;
            if (batch_count_this_file % 50 == 0 && streamlines_from_this_file > 0) { // Progress for large files
                 disp(MSG_DEBUG, "  ... %ld streamlines merged from %s ...", streamlines_from_this_file, current_inp_fname.c_str());
            }
        }
        disp(MSG_DEBUG, "Finished processing %s. Added %ld streamlines to output.", current_inp_fname.c_str(), streamlines_from_this_file);
    }

    if (!writer.close()) {
        disp(MSG_FATAL, "Failed to finalize and close output tractogram: %s", out_fname.c_str());
    } else {
        disp(MSG_INFO, "Finished merging. Total streamlines written: %ld", writer.getFinalStreamlineCount());
    }

    return;

}


void merge(CLI::App* app)
{

    app->description("merges tractograms");

    app->add_option("<output>",              out_fname,          "Output tractogram (.vtk, .tck)")
        ->required();

    app->add_option("--include,-i",          inp_fnames,         "List of tractograms or wildcards (.vtk, .tck, .trk) to include in the output, e.g.1. inp1.vtk inp2.tck, e.g.2. inp*.vtk, e.g.3. inp1*.tck inp2*.vtk")
        ->required()
        ->delimiter(' ');
    
    app->add_option("--exclude,-e",          exc_fnames,         "List of tractograms or wildcards to exclude. These files are excluded even if they match the input wildcards")
        ->delimiter(' ');

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");

    app->callback(run_merge);
    
}

