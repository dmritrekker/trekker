#include "cmd.h"

using namespace NIBR;

namespace CMDARGS_CONVERT {
    std::string  inp_fname;
    std::string  out_fname;
    std::string  reference = "";
    std::string  trx_dtype = "float16";

    bool ascii              = false;

    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;
}

using namespace CMDARGS_CONVERT; 

 
void run_convert()
{ 

    parseCommon(numberOfThreads,verbose);
    if (!parseForceOutput(out_fname,force)) return;

    std::string inp_ext = getFileExtension(inp_fname);
    std::string out_ext = getFileExtension(out_fname);

    if ( (out_ext == "trk") && ( (inp_ext == "tck") || (inp_ext == "vtk") ) && (reference == "") ) {
        std::cout << "Conversion from .vtk/.tck to .trk requires a reference image to be specified using the --reference option." << std::endl << std::flush;
        return;
    }
    
    if ( (inp_ext == "trk") && ( (out_ext == "tck") || (out_ext == "vtk") ) && (reference == "") ) {
        std::cout << "Conversion from .trk to .vtk/.tck requires a reference image to be specified using the --reference option." << std::endl << std::flush;
        return;
    }

    // Trk to trk conversion is ambigous if the user gives a reference image. For now let's not allow this. 
    // One can always first convert to vtk/tck and then to trk.
    if ( (inp_ext == "trk") && (out_ext == "trk") ) {
        std::cout << "Nothing done. (.trk -> .trk conversion not supported.)" << std::endl << std::flush;
        return;
    }


    // === Initialize TractogramReader ===
    NIBR::TractogramReader reader(inp_fname);
    if (!reader.isReady()) {
        disp(MSG_FATAL, "Failed to initialize TractogramReader for input file: %s", inp_fname.c_str());
        return;
    }
    disp(MSG_DEBUG, "Successfully initialized reader for %s. Streamlines found: %lu", inp_fname.c_str(), reader.numberOfStreamlines);

    // === Validate TRX dtype option ===
    if (out_ext == "trx") {
        if (trx_dtype != "float16" && trx_dtype != "float32") {
            std::cout << "Invalid --dtype value: \"" << trx_dtype << "\". Must be float16 or float32." << std::endl << std::flush;
            return;
        }
    }

    // === Create and Configure TractogramWriter ===
    NIBR::TractogramWriter writer(out_fname);
    if (ascii) writer.setVTKIsAscii(true);
    if (out_ext == "trx") writer.setTRXDtype(trx_dtype);
    if (!writer.isValid()) return;

    // === Handle reference image ===
    if (out_ext == "trk") {
        if (!reference.empty()) {
            NIBR::Image<bool> refImgForTrkHeader(reference);
            writer.setTRKReference(refImgForTrkHeader);
        } else {
            disp(MSG_FATAL, "trk output requires a reference image when input is not .trk, but reference is empty.");
            return;
        }
    }
    if (out_ext == "trx" && !reference.empty()) {
        NIBR::Image<bool> refImg(reference);
        writer.setTRXReference(refImg);
    }

    // === Perform Conversion ===
    disp(MSG_DEBUG, "Starting tractogram conversion from .%s to .%s...", inp_ext.c_str(), out_ext.c_str());
    
    if (!writer.open()) {
        disp(MSG_FATAL, "Failed to open output file for writing: %s", out_fname.c_str());
        return;
    }

    long totalStreamlinesProcessed  = 0;
    int  batch_size                 = 50000;
    int  batch_count                = 0;
    int  streamlines_in_batch       = 0;

    reader.reset();

    while (true) {

        NIBR::StreamlineBatch batch = reader.getNextStreamlineBatch(batch_size);

        streamlines_in_batch  = batch.size();

        if (streamlines_in_batch == 0) break;

        if (!writer.writeBatch(std::move(batch))) {
            disp(MSG_ERROR, "Failed to write batch %d to %s.", batch_count, out_fname.c_str());
            writer.close(); // Attempt to close to salvage what's written
            return; 
        }
        totalStreamlinesProcessed += streamlines_in_batch;
        batch_count++;
        if (batch_count > 0 && batch_count % 20 == 0) { // Progress update every 20 batches
            disp(MSG_DEBUG, "Processed %d batches, %ld streamlines written...", batch_count, totalStreamlinesProcessed);
        }
    }
    disp(MSG_DEBUG, "Finished reading input. Total batches processed: %d.", batch_count);

    if (!writer.close()) {
        disp(MSG_DEBUG, "Failed to finalize and close output tractogram: %s", out_fname.c_str());
    } else {
        disp(MSG_INFO, "Tractogram conversion successful!");
        disp(MSG_INFO, "Output written to: %s", out_fname.c_str());
        disp(MSG_INFO, "Total streamlines written: %ld", writer.getFinalStreamlineCount());
        disp(MSG_INFO, "Total points written: %ld", writer.getFinalPointCount());
    }
       
}          
    
     
void convert(CLI::App* app)   
{ 


    app->formatter(std::make_shared<CustomHelpFormatter>());

    const std::string info = "\033[1mWARNING\033[0m: The .trk format has ambiguous definitions, leading to different interpretations by various software tools. For example, DSI Studio and TrackVis interpret this format differently. We follow the nibabel convention. Please ensure that conversions involving the .trk format are done accurately and are compatible with the software you have previously used.";

    setInfo(app,info);

    app->description("converts tractogram file formats");

    app->add_option("<input_tractogram>",    inp_fname,          "Input tractogram (.vtk, .tck, .trk, .trx)")
        ->required()
        ->check(CLI::ExistingFile);
     
    app->add_option("<output_tractogram>",   out_fname,          "Output tractogram (.vtk, .tck, .trk, .trx)")
        ->required( );

    app->add_option("--reference,-r",        reference,          "Reference image. Required for .vtk/.tck → .trk. Optional for .trx output (sets VOXEL_TO_RASMM and DIMENSIONS).")
        ->check(CLI::ExistingFile);
    
    app->add_flag("--ascii,-a",              ascii,              "Write ASCII output. Only available when the output is .vtk.");

    app->add_option("--dtype,-d",            trx_dtype,          "Coordinate dtype for .trx output (float16, float32). Default=float16")
        ->default_val("float16");

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");

    app->callback(run_convert);  
     
}

                       