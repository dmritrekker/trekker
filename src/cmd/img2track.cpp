#include "cmd.h"

using namespace NIBR;

namespace CMDARGS_IMAGE_MAP2TRACTOGRAM {
    std::string inp_image;
    std::string inp_tractogram;
    std::string out_fname;
    bool        isSH = false;
    bool        isSF = false;
    float       smoothing = 0;

    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;
}

using namespace CMDARGS_IMAGE_MAP2TRACTOGRAM;

void run_map2track()
{

    parseCommon(numberOfThreads,verbose);
    if (!parseForceOutput(out_fname,force)) return;
    
    // Initialize tractogram and make copies for multithreader
    NIBR::TractogramReader tractogram(inp_tractogram);

    int N = tractogram.numberOfStreamlines;

    if (N<1) {
        std::cout << "Empty tractogram" << std::endl;
        return;
    }
    

    // Output from SH image
    if (isSH == true) {

        // Read image
        FOD_Image img(inp_image);
        img.read();

        // Reserve mapping
        std::vector<std::vector<float>> mapping;
        mapping.resize(N);

        tractogram.reset();

        auto run = [&]()->void {

            auto [success,streamline,streamlineId] = tractogram.getNextStreamline();

            int len = streamline.size();

            mapping[streamlineId].reserve(len);

            float T[3];
            
            for (int l=0; l<(len-1); l++) {
                vec3sub(T,streamline[l+1],streamline[l]);
                normalize(T);
                mapping[streamlineId].push_back(img.getFODamp(streamline[l].data(),T));
            }

            mapping[streamlineId].push_back(img.getFODamp(streamline[len-1].data(),T));

        };
        NIBR::MT::MTRUN(N, "Mapping from spherical harmonic expansion", run);
        
        std::ofstream out;
        out.open(out_fname,std::ios::binary);

        for (int n=0; n<N; n++)
            for (auto val : mapping[n])
                out.write(reinterpret_cast<char*>(&val), sizeof(float));

        out.close();

        return;
        
    }



    // Output from SF image
    if (isSF == true) {

        // Read image
        SF_Image img(inp_image,true);
        img.read();
        img.setOutsideVal(0.0f);

        if (smoothing>0) 
            img.smooth(smoothing);

        // Reserve mapping
        std::vector<std::vector<float>> mapping;
        mapping.resize(N);

        tractogram.reset();

        auto run = [&]()->void {

            auto [success,streamline,streamlineId] = tractogram.getNextStreamline();

            int len = streamline.size();

            mapping[streamlineId].reserve(len);

            float T[3];
            
            for (int l=0; l<(len-1); l++) {
                vec3sub(T,streamline[l+1],streamline[l]);
                normalize(T);
                mapping[streamlineId].push_back(img.getSFval(streamline[l].data(),T));
            }

            mapping[streamlineId].push_back(img.getSFval(streamline[len-1].data(),T));

        };
        NIBR::MT::MTRUN(N, "Mapping from spherical function", run);

        std::ofstream out;
        out.open(out_fname,std::ios::binary);

        for (int n=0; n<N; n++)
            for (auto val : mapping[n])
                out.write(reinterpret_cast<char*>(&val), sizeof(float));

        out.close();

        return;
        
    }




    // Read image
    NIBR::Image<float> img(inp_image);
    img.read();

    const int dim = img.valCnt;

    // Reserve mapping
    std::vector<std::vector<std::vector<float>>> mapping;
    mapping.resize(N);

    tractogram.reset();

    auto run = [&]()->void {

        auto [success,streamline,streamlineId] = tractogram.getNextStreamline();

        int len = streamline.size();

        mapping[streamlineId].reserve(len);
        
        for (int l=0; l<len; l++) {
            std::vector<float> tmp;
            tmp.reserve(dim);

            for (int t=0; t<dim; t++) {
                tmp.push_back(img(streamline[l].data(),t));
            }

            mapping[streamlineId].push_back(tmp);
        }

    };
    NIBR::MT::MTRUN(N, "Mapping values", run);
    
    std::ofstream out;
    out.open(out_fname,std::ios::binary);

    for (int n=0; n<N; n++)
        for (auto val : mapping[n])
            for (auto d : val) {
                out.write((char*)(&d), sizeof(float));
            }

    out.close();    

}

void img2track(CLI::App* app)
{

    app->description("maps image values on a tractogram");
    
    app->add_option("<input_image>",         inp_image,          "Input image (.nii, .nii.gz)")
        ->required()
        ->check(CLI::ExistingFile);
        
    app->add_option("<input_tractogram>",    inp_tractogram,     "Input tractogram (.vtk, .tck, .trk)")
        ->required()
        ->check(CLI::ExistingFile);
    
    app->add_option("<output_file_name>",    out_fname,          "Output file name. Output is a float32 type binary file.")
        ->required();

    app->add_flag("--SH",                    isSH,               "Treat the image as a 4D spherical harmonics image.");

    app->add_flag("--SF",                    isSF,               "Treat the image as a 4D spherical function image.");

    app->add_option("--SF_smoothing",        smoothing,          "Apply spherical smoothing before using the spherical function.");

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");
    
    app->callback(run_map2track);
    
}
