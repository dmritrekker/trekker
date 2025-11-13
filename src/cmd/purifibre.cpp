#include "cmd.h"

using namespace NIBR;

namespace CMDARGS_PURIFIBRE {

    std::string             inp_tractogram_fname;
    std::string             out_tractogram_fname;
    std::string             out_fico = "";

    float                   trimFactor  = 10;
    float                   puriFactor  = 5;

    float                   voxDim = 0;
    std::tuple<float,int>   anisotropicSmoothing (std::make_tuple(0.0f,0));
    float                   sphericalSmoothing = 15;

    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;

}

using namespace CMDARGS_PURIFIBRE;

using namespace NIBR;

void run_purifibre()
{

    parseCommon(numberOfThreads,verbose);
    if (!parseForceOutput(out_tractogram_fname,force)) return;
    if (!parseForceOutput(out_fico,force)) return;

    if (voxDim<=0) voxDim=4.0f;

    // Initialize tractogram and make copies for multithreader
    NIBR::TractogramReader tractogram(inp_tractogram_fname);

    int N = tractogram.numberOfStreamlines;

    if (N<1) {
        NIBR::disp(MSG_WARN,"Empty tractogram");
        return;
    }

    // Save FICO
    if ((out_fico!="") && !ensureVTK(out_fico)) return;

    // Compute sTODI
    NIBR::SF::init(true,17);

    NIBR::SF_Image img;
    std::vector<float> bb = getTractogramBBox(&tractogram);
    bb.push_back(-0.5);
    bb.push_back(int64_t(SF::getSFCoords().size())-0.5);
    img.createFromBoundingBox(4,bb,voxDim,false);

    NIBR::Tractogram2ImageMapper<float> gridder(&tractogram,&img);
    gridder.anisotropicSmoothing(anisotropicSmoothing);
    allocateGrid_4segmentLength_sf(&gridder);
    gridder.run(processor_4segmentLength_sf<float>, outputCompiler_4segmentLength_sf<float>);
    deallocateGrid_4segmentLength_sf(&gridder);

    img.smooth(sphericalSmoothing);

    
    // Compute SECO and FICO
    std::vector<std::tuple<uint64_t,float>> fico;
    fico.resize(N);

    tractogram.reset();

    auto run = [&]()->void {

        auto [success,streamline,streamlineId] = tractogram.getNextStreamline();

        int len = streamline.size();
        if (len<2) {
            fico[streamlineId] = std::make_tuple(streamlineId,0.0f);
            return;
        }

        float T[3];

        int trim = int(len*trimFactor*0.5*0.01);
        if (trim>=int(len/2-1)) trim = int(len/2)-1;
        if (trim<=0) trim  = 0;

        float minSeco   = std::numeric_limits<float>::infinity();

        for (int l=trim; l<(len-trim-1); l++) {
            vec3sub(T,streamline[l+1],streamline[l]);
            normalize(T);

            float seco = img.getSFval(streamline[l].data(),T); // segment-to-bundle coupling (SECO)
            if (seco < minSeco)
                minSeco = seco;
            
        }
        float seco = img.getSFval(streamline[len-trim-1].data(),T);
        if (seco < minSeco)
            minSeco = seco;

        fico[streamlineId] = std::make_tuple(streamlineId,std::log(minSeco+1));

    };
    NIBR::MT::MTRUN(N, "Computing FICO", run);

    NIBR::SF::clean();
    
    // Save FICO
    if (out_fico!="") {
        writeTractogram(out_fico,&tractogram);

        FILE *out;
	    out = fopen(out_fico.c_str(),"ab+");

        char buffer[256];

        sprintf(buffer,"CELL_DATA %lu\n",size_t(N)); 	
        fwrite(buffer, sizeof(char), strlen(buffer), out);

        sprintf(buffer,"SCALARS FICO float 1\n");
        fwrite(buffer, sizeof(char), strlen(buffer), out);
        
        sprintf(buffer,"LOOKUP_TABLE default\n"); 				
        fwrite(buffer, sizeof(char), strlen(buffer), out);

        for (int n=0; n<N; n++) {
            float tmp = std::get<1>(fico[n]);
            swapByteOrder(tmp);
            fwrite(&tmp, sizeof(float), 1, out);
        }

        fclose(out);
    }

    // Remove remN amount of smallest ones
    std::sort(fico.begin(), fico.end(), [](auto a, auto b) {return std::get<1>(a) < std::get<1>(b);} );

    std::vector<size_t> idx;
    int remN = std::floor(float(fico.size())*puriFactor*0.01);

    for (int n=remN; n<N; n++)
        idx.push_back(std::get<0>(fico[n]));

    // Write feature values
    writeTractogram(out_tractogram_fname,inp_tractogram_fname,idx);
    
}

void purifibre(CLI::App* app) 
{  
    app->formatter(std::make_shared<CustomHelpFormatter>());

    const std::string info = "Purifibre computes a smoothed Track Orientation Density Image (TODI) to measure how well each streamline aligns with the collective organization of its neighbors, assigning a Fiber Coupling (FICO) score based on the streamline's least aligned segment (minimum SECO). It then filters the tractogram by removing a user-defined percentage of streamlines with the lowest FICO scores, thereby eliminating spatially incoherent \"spurious\" fibers."
        "\n\n\033[1mREFERENCES\033[0m:"
        "\n\n[Aydogan2022] Aydogan D.B., “Fiber coupling (FICO) measure using anisotropic smoothing of track orientation density images for tractogram filtering”, ISMRM 2022";

    setInfo(app,info);
    
    app->description("purifibre removes spurious streamlines from tractograms");
    
    app->add_option("<input_tractogram>", inp_tractogram_fname, "Input tractogram (.vtk, .tck, .trk)")->required()->check(CLI::ExistingFile)->type_name("");

    app->add_option("<output_tractogram>", out_tractogram_fname, "Output tractogram (.vtk, .tck)")->required()->type_name("FILE");
    
    app->add_option("--trim,-t", trimFactor, "Trim excludes ends of streamlines from being analyzed. E.g., when trim is 10, 90%% of the streamline is analyzed. 5%% of the streamline length from each end is excluded from the computation. Default: 10.");
    
    app->add_option("--purify, -p", puriFactor, "Percentage of streamlines to remove from the tractogram. Default: 5.");

    app->add_option("--voxDim", voxDim, "Isotropic voxel dimension for sTODI computation. Default: 4.");

    app->add_option("--anisotropicSmoothing", anisotropicSmoothing, "Standard deviation of the Gaussian kernel (in mm), and computation density for anisotropic smoothing (number of streamlines). E.g. when set to 2 100, smoothing is done using 100 streamlines randomly distributed around each streamline using a Gaussion distribution with standard deviation of 2 mm. Default=0 0.");

    app->add_option("--sphericalSmoothing", sphericalSmoothing, "Amount of sTODI spherical smoothing. Default: 30.");

    app->add_option("--save_fico", out_fico, "Saves a .vtk formatted copy of input tractogram with FICO values written as a field.");

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.")->check(CLI::Range(0, INT32_MAX));
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quite\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");

    app->callback(run_purifibre);

}