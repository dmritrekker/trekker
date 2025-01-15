#include "cmd.h"
#include <climits>

using namespace NIBR;

namespace CMDARGS_DMRI_RECON_TRANSHI2015 {
    
    std::string inp_dMRI_img;
    std::string inp_mask_img;
    std::tuple<std::string, std::string> grad_fnames;
    
    int     shOrder                     = TRANSHI2015_SH_ORDER;
    double  deltaStep                   = TRANSHI2015_DELTASTEP;
    double  init_D_inAx                 = TRANSHI2015_INIT_D_INAX;
    double  init_D_trapped              = TRANSHI2015_INIT_D_TRAPPED;
    double  init_D_exAx_iso             = TRANSHI2015_INIT_D_EXAX;
    int     init_minNumConstraint       = TRANSHI2015_INIT_MINNUMCONSTRAINTS;
    int     constraintUpdateCount       = TRANSHI2015_CONSTRAINT_UPDATE_COUNT;
    bool    fastOptimization            = false;
    double  bValLow                     = TRANSHI2015_BVAL_LOW;
    double  bValHigh                    = TRANSHI2015_BVAL_HIGH;
    int     maxIter                     = TRANSHI2015_MAXITER; 
    double  xi_init                     = TRANSHI2015_XI_INIT; 
    double  xi_step                     = TRANSHI2015_XI_STEP; 
    int     xi_stepCount                = TRANSHI2015_XI_STEPCOUNT;
    int     maxCrossings                = TRANSHI2015_MAXCROSSINGS;
    double  noiseFloor                  = TRANSHI2015_NOISEFLOOR;
    

    std::string out_FOD_img;
    std::string out_TM_img;

    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;
}

using namespace CMDARGS_DMRI_RECON_TRANSHI2015;

void run_dMRI_recon_transhi2015()
{
    parseCommon(numberOfThreads,verbose);
    if (!parseForceOutput(out_FOD_img,force)) return;
    if (!parseForceOutput(out_TM_img,force))  return;

    // Check bval and bvec
    auto grad = NIBR::readGradTable(std::get<0>(grad_fnames), std::get<1>(grad_fnames));

    if (!std::get<0>(grad)) {
        disp(MSG_ERROR,"Incorrect bvals/bvecs definition.");
        return;
    }
    
    // Read input image headers
    
    // The {3,0,1,2,4,5,6} part below tells how to arrange the data that will be read from the dMRI image.
    // This order basically arranges the data array so that the forth dimension runs first. 
    // This means data[0][0][0][0] and data[1][0][0][0] are not next to each other in memory. 
    // Instead we are allocating data such that data[0][0][0][0] and data[0][0][0][1] are next to each other.
    // This makes it faster to read the diffusion signal for each voxel separately when we do the reconstruction.
    // Then why do have 4,5,6 at the end? This is because the maximum image dimension supported with NIBR is 7, and an order needs to be set for all dimensions.
    int indexOrder[7] = {3,0,1,2,4,5,6};
    
    NIBR::Image<float> dMRI(inp_dMRI_img,indexOrder);
    NIBR::Image<bool>  mask(inp_mask_img);

    // Read input images
    disp(MSG_INFO,"Reading dMRI image: %s",inp_dMRI_img.c_str());
    dMRI.read();
    mask.read();

    // Define output images
    // We will also define the output images to be ordered so that the forth dimension runs first
    NIBR::Image<float> FOD(indexOrder);
    NIBR::Image<float> TM(indexOrder);

    // Do the reconstruction
    NIBR::recon_transhi2015(
        FOD,
        TM,
        dMRI,
        std::get<1>(grad),
        mask,
        shOrder,
        deltaStep,
        init_D_inAx,
        init_D_trapped,
        init_D_exAx_iso,
        init_minNumConstraint,
        constraintUpdateCount,
        !fastOptimization,
        bValLow,
        bValHigh,
        maxIter,
        xi_init,
        xi_step,
        xi_stepCount,
        maxCrossings,
        noiseFloor);

    // Write output images
    FOD.write(out_FOD_img);
    TM.write(out_TM_img);

}

void dMRI_recon_transhi2015(CLI::App* app)
{

    app->formatter(std::make_shared<CustomHelpFormatter>());

    const std::string info = "This method is based on incorporating a compartment model into a spherical deconvolution framework, directly optimizing spherical harmonics coefficients, for fiber orientation distribution (FOD) reconstruction, using an adaptively constrained energy minimization approach for efficient computation. This allows for sharper and cleaner FODs, better modeling of fiber crossings, and reliable estimation of compartment parameters."
        "\n\n\033[1mREFERENCES\033[0m:"
        "\n\n[Tran2015] Tran G., Shi Y., “Fiber orientation and compartment parameter estimation from multi-shell diffusion imaging”, in IEEE Transactions on Medical Imaging, vol. 34, no. 11, pp. 2320-2332, Nov. 2015, doi: 10.1109/TMI.2015.2430850.";

    setInfo(app,info);

    app->description("dMRI signal reconstruction based on Tran & Shi IEEE TMI 2015 model");
    
    app->add_option("<input dMRI image>", inp_dMRI_img, "Input diffusion MRI image (.nii, .nii.gz)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("--mask,-m", inp_mask_img, "Brain mask (.nii/.nii.gz)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("--gradient,-g",grad_fnames,"bvals and bvecs text files given in FSL format, e.g., -g bvals bvecs.")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("--order,-o",                   shOrder,                    "Spherical harmonic order. Default: 8");
    app->add_option("--deltaStep,-s",               deltaStep,                  "Gradient descent step size. Default: 0.0000001");
    app->add_option("--D_inAx",                     init_D_inAx,                "Intra-axonal diffusivity. Default: 0.0017 mm^2/s");
    app->add_option("--D_trapped",                  init_D_trapped,             "Trapped water diffusivity. Default: 0 mm^2/s");
    app->add_option("--init_D_exAx_iso",            init_D_exAx_iso,            "Initial value for extra-axonal diffusivity. Default: 0.001 mm^2/s");
    app->add_option("--init_minNumConstraint",      init_minNumConstraint,      "Initial number of constraints. Default: 100");
    app->add_option("--constraintUpdateCount",      constraintUpdateCount,      "Update count for constraints. Default: 1");
    app->add_flag  ("--disableFastOptimization",    fastOptimization,           "Disable fast optimization by fully solving the system at each iteration.");
    app->add_option("--bValLow",                    bValLow,                    "Lowest b-value to include. Default: 10 s/mm^2");
    app->add_option("--bValHigh",                   bValHigh,                   "Highest b-value to include. Default: 1000000000000 s/mm2");
    app->add_option("--maxIter",                    maxIter,                    "Number of optimization steps. Default: 1000");
    app->add_option("--xi_init",                    xi_init,                    "Initial regularization value. Default: 0.04");
    app->add_option("--xi_step",                    xi_step,                    "Regularization step size. Default: 0.02");
    app->add_option("--xi_stepCount",               xi_stepCount,               "Number of regularization steps. Default: 100");
    app->add_option("--maxCrossings,-c",            maxCrossings,               "Maximum number of fiber maxCrossings. Default: 4");
    app->add_option("--noiseFloor",                 noiseFloor,                 "Noise floor. Default: 0");

    app->add_option("<output FOD>", out_FOD_img, "Output FOD image (.nii, .nii.gz)")
        ->required();

    app->add_option("<output tissue map>", out_TM_img, "Output tissue map image (.nii, .nii.gz)")
        ->required();

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quite\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");
    
    app->callback(run_dMRI_recon_transhi2015);
    
}
