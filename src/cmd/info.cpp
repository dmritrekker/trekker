#include "cmd.h"

using namespace NIBR;

namespace CMDARGS_INFO {
    std::string  inp_fname;

    int numberOfThreads     =  0;
    std::string verbose     = "info";
}

using namespace CMDARGS_INFO;


void run_info()
{

    parseCommon(numberOfThreads,verbose);

    std::string ext = getFileExtension(inp_fname);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    // Image
    if ((ext == "nii")    || 
        (ext == "nii.gz") || 
        (ext == "mgh")    || 
        (ext == "mgz")    ||
        (ext == "dcm") ) {
        
        NIBR::Image<float> img(inp_fname);
        img.read();
        img.printInfo();
        return;
    }

    // Tractogram
    if ((ext == "vtk") ||
        (ext == "tck") || 
        (ext == "trk")) {

        NIBR::TractogramReader tractogram;
        if (tractogram.initReader(inp_fname)) {
            if (((tractogram.numberOfPoints == 0) && (tractogram.numberOfStreamlines == 0)) ||
                ((tractogram.numberOfPoints  > 0) && (tractogram.numberOfStreamlines  > 0))) {
                    tractogram.printInfo();
                    return;       
            }
        }

    }


    // Surface
    if ((ext == "vtk")      ||
        (ext == "gii")      || 
        (ext == "orig")     || 
        (ext == "pial")     || 
        (ext == "white")    || 
        (ext == "inflated") || 
        (ext == "sphere")   || 
        (ext == "smoothwm")) {
        
        NIBR::Surface surf(inp_fname);
        surf.readMesh();     
        surf.printInfo();
        return;
    }

    std::cout << "Unknown file format" << std::endl << std::flush;
    return;

}


void info(CLI::App* app)
{

    app->description("displays information about input file");

    app->add_option("<input>", inp_fname, "Input file")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quite\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    
    app->callback(run_info);
    
}

