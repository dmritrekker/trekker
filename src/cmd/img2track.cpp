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
    if(!ensureVTKorTCK(inp_tractogram)) return;
    NIBR::TractogramReader* tractogram = new NIBR::TractogramReader[numberOfThreads]();
    tractogram[0].initReader(inp_tractogram);

    int N = tractogram[0].numberOfStreamlines;

    if (N<1) {
        std::cout << "Empty tractogram" << std::endl;
        delete[] tractogram;
        return;
    }

    for (int t = 1; t < numberOfThreads; t++)
        tractogram[t].copyFrom(tractogram[0]);
    

    // Output from SH image
    if (isSH == true) {

        // Read image
        FOD_Image img(inp_image);
        img.read();

        // Reserve mapping
        std::vector<std::vector<float>> mapping;
        mapping.resize(N);

        auto run = [&](NIBR::MT::TASK task)->void {

            float** streamline = tractogram[task.threadId].readStreamline(task.no);
            int len            = tractogram[task.threadId].len[task.no];
            mapping[task.no].reserve(len);

            float T[3];
            
            for (int l=0; l<(len-1); l++) {
                vec3sub(T,streamline[l+1],streamline[l]);
                normalize(T);
                mapping[task.no].push_back(img.getFODamp(streamline[l],T));
                delete[] streamline[l];
            }

            mapping[task.no].push_back(img.getFODamp(streamline[len-1],T));

            delete[] streamline;

        };
        NIBR::MT::MTRUN(N, "Mapping from spherical harmonic expansion", run);

        for (int t = 0; t < numberOfThreads; t++) 
            tractogram[t].destroyCopy();
        delete[] tractogram;
        
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

        auto run = [&](NIBR::MT::TASK task)->void {

            float** streamline = tractogram[task.threadId].readStreamline(task.no);
            int len            = tractogram[task.threadId].len[task.no];
            mapping[task.no].reserve(len);

            float T[3];
            
            for (int l=0; l<(len-1); l++) {
                vec3sub(T,streamline[l+1],streamline[l]);
                normalize(T);
                mapping[task.no].push_back(img.getSFval(streamline[l],T));

                /*
                float tmp = img.getSFval(streamline[l],T);

                if (tmp<0) {
                    std::cout << "val: " << tmp << std::endl;
                    std::cout << "n: " << task.no << " l: " << l << std::endl;
                    std::cout << "xyz: " << streamline[l][0] << " " << streamline[l][1] << " " << streamline[l][2] << std::endl;
                    float ijk[3];
                    img.to_ijk(streamline[l],ijk);
                    std::cout << "ijk: " << ijk[0] << " " << ijk[1] << " " << ijk[2] << std::endl;
                    std::cout << "T: " << T[0] << " " << T[1] << " " <<  T[2] << std::endl;
                    std::cout << "sfInd: " << SF::coordinate2index(T) << std::endl;
                    std::cout << "Recomp 1: " << Interpolator<float,float>::interp_linear_4D_att(&img, streamline[l], SF::coordinate2index(T)) << std::endl;
                    std::cout << "Recomp 2: " << Interpolator<float,float>::interp_linear_4D_att(&img, streamline[l], SF::coordinate2index(T)) << std::endl;
                    std::cout << "Recomp 3: " << Interpolator<float,float>::interp_linear_4D_att(&img, streamline[l], SF::coordinate2index(T)) << std::endl;
                    std::cout << "Recomp 4: " << Interpolator<float,float>::interp_linear_4D_att(&img, streamline[l], SF::coordinate2index(T)) << std::endl;
                    std::cout << "Recomp 5: " << Interpolator<float,float>::interp_linear_4D_att(&img, streamline[l], SF::coordinate2index(T)) << std::endl;

                    float   cfs[3];
                    int64_t cor_ijk[3];
                    INTERPAT checker = Interpolator<float,float>::init_interp_linear(&img, streamline[l], cfs, cor_ijk);
                    std::cout << "cfs: " << cfs[0] << " " << cfs[1] << " " <<  cfs[2] << std::endl;
                    std::cout << "cor_ijk: " << cor_ijk[0] << " " << cor_ijk[1] << " " <<  cor_ijk[2] << std::endl;

                    if (checker==INSIDE) im("INSIDE");
                    else if (checker==BOUNDARY) im("BOUNDARY");
                    else im("OUTSIDE");

                    im("");

                }

                mapping[task.no].push_back(tmp);
                */

                
                
            
                delete[] streamline[l];
            }

            mapping[task.no].push_back(img.getSFval(streamline[len-1],T));

            delete[] streamline;

        };
        NIBR::MT::MTRUN(N, "Mapping from spherical function", run);
        // NIBR::MT::MTRUN(N, run);

    
        for (int t = 0; t < numberOfThreads; t++) 
            tractogram[t].destroyCopy();
        delete[] tractogram;

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

    auto run = [&](NIBR::MT::TASK task)->void {

        float** streamline = tractogram[task.threadId].readStreamline(task.no);
        int len            = tractogram[task.threadId].len[task.no];
        mapping[task.no].reserve(len);
        
        for (int l=0; l<len; l++) {
            std::vector<float> tmp;
            tmp.reserve(dim);

            for (int t=0; t<dim; t++) {
                tmp.push_back(img(streamline[l],t));
            }

            mapping[task.no].push_back(tmp);
            delete[] streamline[l];
        }
        delete[] streamline;

    };
    NIBR::MT::MTRUN(N, "Mapping values", run);

    for (int t = 0; t < numberOfThreads; t++) 
        tractogram[t].destroyCopy();
    delete[] tractogram;
    
    std::ofstream out;
    out.open(out_fname,std::ios::binary);

    for (int n=0; n<N; n++)
        for (auto val : mapping[n])
            for (auto d : val) {
                // std::cout << d << std::endl;
                out.write((char*)(&d), sizeof(float));
            }

    out.close();    

}

void img2track(CLI::App* app)
{

    app->description("maps image values on a tractogram");
    
    app->add_option("<input_image>", inp_image, "Input image (.nii, .nii.gz)")
        ->required()
        ->check(CLI::ExistingFile);
        
    app->add_option("<input_tractogram>", inp_tractogram, "Input tractogram (.vtk, .tck)")
        ->required()
        ->check(CLI::ExistingFile);
    
    app->add_option("<output_file_name>", out_fname, "Output file name. Output is a float32 type binary file.")
        ->required();

    app->add_flag("--SH", isSH, "Treat the image as a 4D spherical harmonics image.");

    app->add_flag("--SF", isSF, "Treat the image as a 4D spherical function image.");

    app->add_option("--SF_smoothing", smoothing, "Apply spherical smoothing before using the spherical function.");

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");
    
    app->callback(run_map2track);
    
}
