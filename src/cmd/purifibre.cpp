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

    if (!ensureVTKorTCK(out_tractogram_fname)) return;
    if ((out_fico!="") && !ensureVTK(out_fico)) return;

    
    // Load tractogram
    NIBR::TractogramReader tractogram(inp_tractogram_fname);
    if (!tractogram.isReady()) {
        disp(MSG_FATAL, "Failed to open input tractogram: %s", inp_tractogram_fname.c_str());
        return;
    }

    // Compute FICO values
    std::vector<float> fico = NIBR::getFico(&tractogram,trimFactor,voxDim,anisotropicSmoothing,sphericalSmoothing);

    if (fico.size()!=tractogram.numberOfStreamlines) {
        disp(MSG_FATAL, "FICO computation failed.");
        return;
    }

    // Purify tractogram
    std::vector<size_t> idx = NIBR::purify(fico, puriFactor);

    // Write output tractogram
    writeTractogram(out_tractogram_fname,inp_tractogram_fname,idx);

    // Write a tractogram with FICO values as a field
    if (out_fico!="") {

        TractogramField ficoField = NIBR::makeTractogramFieldFromVector(tractogram, "FICO", fico);
        std::vector<TractogramField> fields;
        fields.emplace_back(ficoField);

        tractogram.reset();
        Tractogram allStreamlines = tractogram.getTractogram();
        writeTractogram(out_fico, allStreamlines, fields);
    }
    
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

    app->add_option("<output_tractogram>", out_tractogram_fname, "Output tractogram (.vtk, .tck, .trx)")->required()->type_name("FILE");
    
    app->add_option("--trim,-t", trimFactor, "Trim excludes ends of streamlines from being analyzed. E.g., when trim is 10, 90%% of the streamline is analyzed. 5%% of the streamline length from each end is excluded from the computation. Default: 10.");
    
    app->add_option("--purify, -p", puriFactor, "Percentage of streamlines to remove from the tractogram. Default: 5.");

    app->add_option("--voxDim", voxDim, "Isotropic voxel dimension for sTODI computation. Default: 4.");

    app->add_option("--anisotropicSmoothing", anisotropicSmoothing, "Standard deviation of the Gaussian kernel (in mm), and computation density for anisotropic smoothing (number of streamlines). E.g. when set to 2 100, smoothing is done using 100 streamlines randomly distributed around each streamline using a Gaussion distribution with standard deviation of 2 mm. Default=0 0.");

    app->add_option("--sphericalSmoothing", sphericalSmoothing, "Amount of sTODI spherical smoothing. Default: 15.");

    app->add_option("--save_fico", out_fico, "Saves a .vtk formatted copy of input tractogram with FICO values written as a field.");

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.")->check(CLI::Range(0, INT32_MAX));
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quite\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");

    app->callback(run_purifibre);

}