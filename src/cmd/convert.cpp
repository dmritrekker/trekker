#include "cmd.h"

using namespace NIBR;

namespace CMDARGS_CONVERT {
    std::string  inp_fname;
    std::string  out_fname;
    std::string  reference = "";

    bool ascii = false;
    
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


    NIBR::TractogramReader tractogram(inp_fname);

    if (reference != "") {
        NIBR::Image<bool> img(reference);
        tractogram.setReferenceImage(&img);
    }
    
    if (ascii && (out_ext=="vtk"))
        NIBR::writeTractogram_VTK_ascii(out_fname,&tractogram);
    else
        NIBR::writeTractogram(out_fname,&tractogram);
       
}          
    
     
void convert(CLI::App* app)   
{ 


    app->formatter(std::make_shared<CustomHelpFormatter>());

    const std::string info = "\033[1mWARNING\033[0m: The .trk format has ambiguous definitions, leading to different interpretations by various software tools. For example, DSI Studio and TrackVis interpret this format differently. We follow the nibabel convention. Please ensure that conversions involving the .trk format are done accurately and are compatible with the software you have previously used.";

    setInfo(app,info);

    app->description("converts tractogram file formats");

    app->add_option("<input_tractogram>", inp_fname, "Input tractogram (.vtk, .tck,.trk)")
        ->required()
        ->check(CLI::ExistingFile);
     
    app->add_option("<output_tractogram>", out_fname, "Output tractogram (.vtk, .tck)")
        ->required( );

    app->add_option("--reference,-r", reference, "Image to use as reference space (needed for converting .vtk/.tck to .trk, and vice versa)")
        ->check(CLI::ExistingFile);
    
    app->add_flag("--ascii,-a", ascii, "Write ASCII output. Only available when the output is .vtk.");

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quite\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");

    app->callback(run_convert);  
     
}

                       