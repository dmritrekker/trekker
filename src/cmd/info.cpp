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

    disp(MSG_DETAIL, "File extension is: %s", ext.c_str());

    // Image
    if ((ext == "nii")    || 
        (ext == "nii.gz") || 
        (ext == "mgh")    || 
        (ext == "mgz")    ||
        (ext == "dcm")    ||
        (ext == "")) {
        
        disp(MSG_DETAIL, "Image");
        NIBR::Image<float> img;
        img.setFilePath(inp_fname);
        if (img.readHeader()) {
            if (img.read()) {
                img.printInfo();
            } else {
                disp(MSG_ERROR, "Error reading image");
            }
        } else {
            disp(MSG_ERROR, "Error reading file header");
        }
        return;
    }
    disp(MSG_DETAIL, "Not an image");

    // Tractogram
    if ((ext == "vtk") ||
        (ext == "tck") || 
        (ext == "trk")) {

        NIBR::TractogramReader tractogram;
        bool isTractogram = false;
        
        if (tractogram.initReader(inp_fname)) {
            
            if ((ext=="tck") || (ext == "trk")) {
                 isTractogram = true;
            } else {
                if (((tractogram.numberOfPoints == 0) && (tractogram.numberOfStreamlines == 0)) ||
                    ((tractogram.numberOfPoints  > 0) && (tractogram.numberOfStreamlines  > 0))) {
                        isTractogram = true;
                }
            }

            
        }

        if (isTractogram) {
            disp(MSG_DETAIL, "Tractogram");
            tractogram.printInfo();
            return;
        }

    }
    disp(MSG_DETAIL, "Not a tractogram");

    // Surface
    if ((ext == "vtk")      ||
        (ext == "gii")      || 
        (ext == "orig")     || 
        (ext == "pial")     || 
        (ext == "white")    || 
        (ext == "inflated") || 
        (ext == "sphere")   || 
        (ext == "smoothwm")) {
        
        disp(MSG_DETAIL, "Surface");
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

    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    
    app->callback(run_info);
    
}

