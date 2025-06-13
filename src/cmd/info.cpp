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

        bool isTractogram = false;

        NIBR::disableTerminalOutput();
        NIBR::TractogramReader tractogram(inp_fname);
        NIBR::enableTerminalOutput();
        
        if (tractogram.isReady()) {
            
            if ((ext=="tck") || (ext == "trk")) {
                 isTractogram = true;
            } else {
                if (tractogram.numberOfStreamlines  > 0) {
                    isTractogram = true;
                }
            }
            
        }

        if (isTractogram) {
            disp(MSG_DETAIL, "Tractogram");
            tractogram.printInfo();

            disp(MSG_DETAIL,"Computing and plotting streamline length histogram.");

            int N = 13;
            std::vector<float> bins(N);
            for (int n = 0; n < N; n++) {
                bins[n] = 20.0f * float(n);
            }

            std::cout << std::endl;
            auto [isCorrupted,streamlineLengths,numberOfPoints,aveStepSize,stdStepSize] = getTractogramStats(&tractogram);

            if (isCorrupted) {
                disp(MSG_INFO,"Corrupted streamline detected.");
            } else {
                disp(MSG_INFO,"No corruption detected.");
                disp(MSG_INFO,"Number of points: %lu",numberOfPoints);
                disp(MSG_INFO,"Mean±Std of step size: %.4f±%.4f",aveStepSize,stdStepSize);
                auto binCounts = create_histogram(streamlineLengths,bins);
                NIBR::plot_histogram("Streamline length histogram",bins,binCounts);
            }

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

    if (ext == "vtk") {
        disp(MSG_INFO,"Unknown or corrupt .vtk format.");
        return;
    }

    disp(MSG_INFO,"Unknown file format.");
    
    return;

}


void info(CLI::App* app)
{

    app->description("displays information about input file");

    app->add_option("<input>",              inp_fname,           "Input file")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    
    app->callback(run_info);
    
}

