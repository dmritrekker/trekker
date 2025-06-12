#include "cmd.h"


using namespace NIBR;

namespace CMDARGS_TRACK
{
    // Tracking options
    std::string              alg                     = "";
    std::vector<std::string> fod;
    std::string              orderOfDirections       = "XYZ";
    bool                     dontDiscretizeFod       = false;
    float                    stepSize                = 0;
    float                    minRadiusOfCurvature    = 0;
    float                    minDataSupport          = NAN;
    float                    dataSupportExponent     = 1;
    int                      runTimeLimit            = 0;
    int                      idleTimeLimit           = 0;

    // Sampling options
    int         maxEstInterval          = 0;
    int         initMaxEstTrials        = 0;
    int         propMaxEstTrials        = 0;
    int         maxSamplingPerStep      = 0;
    bool        useBestAtInit           = false;
    bool        useLegacySampling       = false;
    int         samplingQuality         = 0;

    // Probe options
    float       probeLength             = 0;
    float       probeRadius             = 0;
    int         probeCount              = 0;
    int         probeQuality            = 0;
    float       ignoreWeakLinks         = 0;

    // Pathway options
    std::vector<std::string> pathway; 
    float       minlength               = 0;
    float       maxlength               = FLT_MAX;
    bool        oneSided                = false;
    bool        stopAtMax               = false;
    bool        inOrder                 = false;
    bool        skipSeed                = false;
    // bool        allowEdgeSeeds          = false;

    // Seeding options
    std::vector<std::string> seedInp;
    std::vector<std::string> discardSeedInp;
    CLI::Option* seedTrials_option;
    int          seedTrials             = 1;
    CLI::Option* seedCount_option;
    int          seedCount              = 0;
    CLI::Option* seedDensity_option;
    float        seedDensity            = 0;

    std::vector<std::string> seed_surf_faceDensity;
    std::vector<std::string> seed_surf_vertDensity;
    std::string              seed_surf_fieldDensity = "";

    CLI::Option* seed_surf_useSurfNorm_option;
    bool         seed_surf_useSurfNorm      = false;
    CLI::Option* seed_surf_dontSeedInside_option;


    // Output options
    std::string out_fname;
    float       writeStepSize       =  0;
    bool        ascii               = false;
    bool        saveSeedIndexField  = false;
    
    // General options
    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;
    
}

using namespace CMDARGS_TRACK;

void run_track()
{

    parseCommon(numberOfThreads,verbose);
    if (!parseForceOutput(out_fname,force)) return;

    if(!ensureVTKorTCK(out_fname)) return;
    std::string out_ext = getFileExtension(out_fname);

    if (saveSeedIndexField) {
        if (ascii)                  {disp(MSG_ERROR, "saveSeedIndexField is available only for binary vtk output"); return;}
        if (!ensureVTK(out_fname))  {disp(MSG_ERROR, "saveSeedIndexField is available only for binary vtk output"); return;}
    }

    // =======================
    // TREKKER
    NIBR::Trekker* trekker;
    // =======================

    // =======================
    // ALGORITHM
    if ((alg == "") || (alg == "ptt")) {
        trekker = new NIBR::Trekker(alg);
        trekker->numberOfThreads(numberOfThreads);
        trekker->runTimeLimit(runTimeLimit*60);
        trekker->idleTimeLimit(idleTimeLimit*60);
    } else {
        disp(MSG_ERROR, "Unknown algoritm");
        return;
    }

    if (fod.size()==1) {
        trekker->fod(fod[0]);
    } else if (fod.size()==3) {
        trekker->fod(fod[0]);
        trekker->fodSphere(fod[1]);
        if (fod[2]=="sym")        trekker->fodIsSym(true);
        else if (fod[2]=="asym")  trekker->fodIsSym(false);
        else {
            disp(MSG_ERROR, "Unknown option %s", fod[2].c_str());
            return;
        }
    } else {
        disp(MSG_ERROR, "FOD definition is incorrect");
        return;
    }

    NIBR::convertOrderOfDirections(orderOfDirections);
    trekker->orderOfDirections(orderOfDirections);
    trekker->fodDiscretization(!dontDiscretizeFod);

    trekker->stepSize(stepSize);
    trekker->minRadiusOfCurvature(minRadiusOfCurvature);
    trekker->minDataSupport(minDataSupport);
    trekker->dataSupportExponent(dataSupportExponent);

    trekker->maxEstInterval(maxEstInterval);
    trekker->initMaxEstTrials(initMaxEstTrials);
    trekker->propMaxEstTrials(propMaxEstTrials);
    trekker->maxSamplingPerStep(maxSamplingPerStep);
    trekker->useBestAtInit(useBestAtInit);
    trekker->useLegacySampling(useLegacySampling);
    trekker->samplingQuality(samplingQuality);

    trekker->probeLength(probeLength);
    trekker->probeRadius(probeRadius);
    trekker->probeCount(probeCount);
    trekker->probeQuality(probeQuality);
    trekker->ignoreWeakLinks(ignoreWeakLinks);

    trekker->writeStepSize(writeStepSize);
    // =======================



    // =======================
    // SEED
    if (!seed_surf_faceDensity.empty()) {

        if ((seed_surf_faceDensity.size()!=2)){
            disp(MSG_ERROR,"seed_surf_faceDensity expects 2 arguments: file name and data type. Acceptable types are \"int\", \"float\" or \"ascii\". E.g. \"--seed_surf_faceDensity density int\"");
            return;
        }
        trekker->seed_surface_faceDensity(seed_surf_faceDensity[0]);
        trekker->seed_surface_density_fileDataType(seed_surf_faceDensity[1]);
    }

    if (!seed_surf_vertDensity.empty()) {
        if ((seed_surf_vertDensity.size()!=2)){
            disp(MSG_ERROR,"seed_surf_vertDensity expects 2 arguments: file name and data type. Acceptable types are \"int\", \"float\" or \"ascii\". E.g. \"--seed_surf_vertDensity density int\"");
            return;
        }
        trekker->seed_surface_vertDensity(seed_surf_vertDensity[0]);
        trekker->seed_surface_density_fileDataType(seed_surf_vertDensity[1]);
    }

    trekker->seed_surface_fieldDensity(seed_surf_fieldDensity);
    if (*seed_surf_useSurfNorm_option)    trekker->seed_surface_useNormForDir(seed_surf_useSurfNorm);

    if (*seedCount_option)   trekker->seed_count  (seedCount  );
    if (*seedDensity_option) trekker->seed_density(seedDensity);
    if (*seedTrials_option)  trekker->seed_trials (seedTrials );

    if (!trekker->pathway_addSeed(seedInp))         return;
    
    std::vector<std::string> tmp;
    for (size_t n = 0; n < discardSeedInp.size(); n++) {
        tmp.push_back("discard_seed");
        tmp.push_back(discardSeedInp[n]);
    }
    if (!trekker->pathway_addRule(tmp))  return;
    // =======================

    // =======================
    // PATHWAY
    if (!trekker->pathway_minLength(minlength))         return;
    if (!trekker->pathway_maxLength(maxlength))         return;
    if (!trekker->pathway_stopAtMax(stopAtMax))         return;
    if (!trekker->pathway_oneSided(oneSided))           return;
    if (!trekker->pathway_skipSeed(skipSeed))           return;
    // if (!trekker->pathway_noEdgeSeed(!allowEdgeSeeds))  return;
    if (!trekker->pathway_inOrder(inOrder))             return;
    if (!trekker->pathway_addRule(pathway))             return;
    // =======================

    // =======================
    // OTHER
    trekker->saveSeedIndex(saveSeedIndexField);
    // =======================

    // =======================
    // PREPARE TRACTOGRAM WRITER
    NIBR::TractogramWriter writer(out_fname);
    if (!writer.open()) {
        disp(MSG_FATAL, "Failed to open output file: %s", out_fname.c_str());
        return;
    }
    // =======================



    // =======================
    // RUN TREKKER
    trekker->run(&writer);
    disp(MSG_DEBUG,"Tracking finished");

    disp(MSG_DEBUG,"Writing output");
    if (saveSeedIndexField) {
        std::vector<TractogramField> seedIdx;
        seedIdx.push_back(TRACKER::getSeedIndexField());
        writer.setVTKFields(seedIdx);
    }

    if (!writer.close()) {
        disp(MSG_ERROR, "Failed to finalize and close output file.");
    } else {
        disp(MSG_DEBUG, "Processing finished successfully.");
    }

    delete trekker;

    disp(MSG_INFO,"Done.");
    return;
} 

void track(CLI::App *app) 
{

    app->formatter(std::make_shared<CustomHelpFormatter>());
 
    const std::string info = "Pathway rules"
        "\n\nTrekker supports 12 different pathway rules for filtering streamlines, which can be defined using the \033[1m--pathway\033[0m or \033[1m-p\033[0m options:"
        "\n  \033[1m \u2022 require_entry:\033[0m tracks are required to enter this region"
        "\n  \033[1m \u2022 require_exit:\033[0m tracks are required to exit this region"
        "\n  \033[1m \u2022 require_end_inside:\033[0m tracks are required to end inside this region"
        "\n  \033[1m \u2022 discard_if_enters:\033[0m tracks are not allowed to enter this region"
        "\n  \033[1m \u2022 discard_if_exits:\033[0m tracks are not allowed to exit this region"
        "\n  \033[1m \u2022 discard_if_ends_inside:\033[0m tracks are not allowed to end inside this region"
        "\n  \033[1m \u2022 stop_at_entry:\033[0m tracks will stop at the point where they enter the region. The end point is at the interface. (requires \033[1mseed\033[0m to be defined)"
        "\n  \033[1m \u2022 stop_after_entry:\033[0m tracks will stop immediately after they enter the region. The end point is inside the region. (requires \033[1mseed\033[0m to be defined)"
        "\n  \033[1m \u2022 stop_before_entry:\033[0m tracks will stop just before they enter the region. The end point is outside the region. (requires \033[1mseed\033[0m to be defined)"
        "\n  \033[1m \u2022 stop_at_exit:\033[0m tracks will stop at the point where they exit the region. The end point is at the interface. (requires \033[1mseed\033[0m to be defined)"
        "\n  \033[1m \u2022 stop_after_exit:\033[0m tracks will stop immediately after they exit the region. The end point is outside the region. (requires \033[1mseed\033[0m to be defined)"
        "\n  \033[1m \u2022 stop_before_exit:\033[0m tracks will stop just before they exit the region. The end point is inside the region. (requires \033[1mseed\033[0m to be defined)"
        "\n\nWhen --inOrder option is used, \'require_entry\' and \'require_exit\' rules are adhered in the order that they are specified."
        "\n\nIn addition to the above pathway rules, Trekker supports the definition of seed region both for tracking and filtering, using the \033[1m--seed\033[0m or \033[1m-s\033[0m options. When a seed is defined for filtering, all the streamlines are treated as if they were computed using that region as seed, i.e., if only seed is provided without any other filtering option, the result is identical to the case where only \'require_entry\' rule is defined."
        "\n\nFollowing filtering features can \033[1monly\033[0m be used only when a seed is defined:"
        "\n  \033[1m \u2022 Side specific filtering:\033[0m the above pathway rules can be defined separately for each side of the track, using \'_A\' and  \'_B\' extensions, e.g., \'require_entry_A\' or \'discard_if_ends_inside_B\'. Here, seed is considered to be somewhere between the end points A and B. If one of the pathway rules is defined using the \'_A\' or \'_B\' extensions, then all the other rules must also have an \'_A\' or \'_B\' extension."
        "\n  \033[1m \u2022 One-sided filtering:\033[0m the --oneSided option can only be used when pathway rules are not side specific. When --oneSided is used, starting from the seed, only one side of the track is taken into account, the other side is ignored and it will not appear in the output."
        "\n  \033[1m \u2022 Skipping seed:\033[0m the --skipSeed option can only used together with --oneSided. When --skipSeed is enabled, the output tracks only have one point that falls within the seed, and that point is one of the end points of the tracks."
        "\n  \033[1m \u2022 Using stop rules:\033[0m the \'stop_at_entry\', \'stop_at_exit\' etc. rules can only be used when a seed is defined."
        "\n\nTrekker supports the following input options to define the seed and pathway rules:"
        "\n  \033[1m \u2022 Sphere:\033[0m e.g. \'-p require_entry 1.2,2.4,33.2,4\' defines a sphere using x,y,z,r notation."
        "\n  \033[1m \u2022 Image files (.nii, .nii.gz):\033[0m The use of images are interpreted in six different ways:"
        "\n       \033[1m 1.\033[0m If only an integer type image is provided, as in \'-s img.nii\', then it is considered as a mask that is created by thresholding values above zero. And during filtering, nearest neighbor interpolation is used."
        "\n       \033[1m 2.\033[0m If only a floating type image is provided, as in \'-s img.nii\', then it is considered to provide partial volume fraction. A value above zero is considered inside, and during filtering, linear interpolation is used."
        "\n       \033[1m 3.\033[0m If an image is provided followed by \'label\', as in \'-s img.nii label\', then it is considered as a mask that is created by thresholding values above zero. And during filtering, nearest neighbor interpolation is used."
        "\n       \033[1m 4.\033[0m If an image is provided followed by \'pvf\', as in \'-s img.nii pvf\', then it is considered to provide partial volume fraction. A value above zero is considered inside, and during filtering, linear interpolation is used."
        "\n       \033[1m 5.\033[0m If an image is provided followed by \'label\' and an integer, as in \'-s img.nii label 1023\', then the provided integer value is considered as a label and a label image is created only using that value. And during filtering, nearest neighbor interpolation is used."
        "\n       \033[1m 6.\033[0m If an image is provided followed by \'pvf\' and an integer, as in \'-s img.nii label 0\', then it is considered that the input image is 4 dimensional, where the 4th dimension contains partial volume fractions, and the provided integer indicates the volume to use for filtering. A value above zero is considered inside, and during filtering, linear interpolation is used."
        "\n  \033[1m \u2022 Surface files (.vtk, .gii): \033[0m "
        "\n       \033[1m 1.\033[0m Surfaces can be provided only as they are, as in \'-s surf.vtk\'."
        "\n       \033[1m 2.\033[0m If the surface is followed by x,y,z,r notation, as in \'-s surf.vtk 1.2,2.4,33.2,4\', then a disc centered at x,y,z with radius r is extracted and used for the rule."
        "\n       \033[1m 3.\033[0m If the surface is followed by a string and an integer, as in \'-s surf.vtk label,3\', then the surface is considered to contain a field with the provided string. The integer is used as a label, which is used for filtering, e.g., a surface containing labels for different parts of the brain can be used for filtering."
        "\n       \033[1m 4.\033[0m If the surface is defined as in \'-s surf.vtk fileName,VERT,int,3\', then the fileName is considered to contain labels for each VERTices, the file contains \'int\' (integer) data type, and the filtering should only consider VERTices with label 3."
        "\n       \033[1m Note 1:\033[0m Inside/outside of a surface is determined by the direction of the normal vectors. It is considered that normals point towards the outside of surface meshes. This convention can be changed by using ``-s surf.vtk flipNormals``, which will internally flip the direction of the surface normals when tracking/filtering."
        "\n       \033[1m Note 2:\033[0m For fast filtering, Trekker first discretizes the surface meshes onto images. The default discretization resolution is 1, which can be changed by proving a scalar number after the surface file is defined, as in \'-s surf.vtk 0.4 label,3\', where 0.4 will be used to discretize the surface."
        "\n       \033[1m Note 3:\033[0m If the surface is closed, it is possible exclude the interior region, and only apply the rules based on the surface mesh (boundary). This can be done by adding \'2D\' after the file name, as in \'-s surf.vtk 2D label,3\'"
        "\n       \033[1m Note 4:\033[0m It is possible change the place of the surface options, e.g. \'-s surf.vtk label,3 0.5 2D\' is same as \'-s surf.vtk 0.5 label,3 2D\'"
        "\n\n\033[1mREFERENCES\033[0m:"
        "\n\n[Aydogan2021] Aydogan D.B., Shi Y., “Parallel transport tractography”, in IEEE Transactions on Medical Imaging, vol. 40, no. 2, pp. 635-647, Feb. 2021, doi: 10.1109/TMI.2020.3034038."
        "\n\n[Aydogan2019] Aydogan D.B., Shi Y., “A novel fiber tracking algorithm using parallel transport frames”, Proceedings of the 27th Annual Meeting of the International Society of Magnetic Resonance in Medicine (ISMRM) 2019.";


    setInfo(app,info);

    app->description("fiber tracker");

    // General options
    auto general = app->add_option_group(center_text("GENERAL OPTIONS",45));
    
    general->add_option ("<FOD>",                     fod,                   "Input FOD image (.nii, .nii.gz). Trekker supports both symmetric and asymmetric FODs, i.e. spherical harmonics with both even and odd orders.")->required();
    general->add_option ("--output,-o",               out_fname,             "Output tractogram (.vtk, .tck)")->required()->type_name("FILE");
    general->add_flag   ("--saveSeedIndexField",      saveSeedIndexField,    "Save seed indices as tractogram field. Available only for binary vtk output.");
    general->add_flag   ("--ascii,-a",                ascii,                 "Write ASCII output (.vtk only)");
    general->add_option ("--writeStepSize,-w",        writeStepSize,         "Because stepSize might be very small, it might not be desirable to save each step of propagation in the output. writeStepSize enables skipping of a number of steps before saving them. This parameter does not change the propagation algorithm, internally Trekker always walks with the specified --stepSize. --writeStepSize by Default: is set, so that the distance between the output tracks is 0.5 x the smallest of the FOD voxel dimensions.");
    general->add_option("--numberOfThreads, -n",      numberOfThreads,       "Number of threads.");
    general->add_option("--verbose, -v",              verbose,               "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    general->add_flag("--force, -f",                  force,                 "Force overwriting of existing file");


    // Fiber tracking options
    auto tracking  = app->add_option_group(center_text("FIBER TRACKING OPTIONS",45));

    // Algorithm options
    auto algorithm = tracking->add_option_group("ALGORITHM PARAMETERS");
    // algorithm->add_option ("--algorithm",              alg,                    "Fiber tracking algorithm. Default=ptt");
    algorithm->add_option ("--stepSize",               stepSize,               "Step size. Default=1/40 x the smallest of the FOD voxel dimensions.");
    algorithm->add_option ("--minRadiusOfCurvature",   minRadiusOfCurvature,   "Minimum radius of curvature. Default=1/2 x the smallest of the FOD voxel dimensions.");
    algorithm->add_option ("--minDataSupport",         minDataSupport,         "Threshold for minimum data support. For PTT algorithm this is minimum FOD threshold. Default=0.05.");
    algorithm->add_option ("--dataSupportExponent",    dataSupportExponent,    "Data support to the power dataSupportExponent is used for rejection sampling. Default=1.");
    algorithm->add_option ("--orderOfDirections",      orderOfDirections,      "Switches direction of FOD. By default XYZ is assumed (no change). A lower case can be used to switch the direction, e.g., xYZ switches the sign of the first component. The order of components can be changed as well, e.g., yXz switches the sign of the second component while replacing it with the first, at the same time the sign of the last component is also changed.");
    algorithm->add_flag   ("--dontDiscretizeFod",      dontDiscretizeFod,      "Turns of FOD discretization. For faster tracking, Trekker by default will discretize the FOD on a sphere (using 2076 points for symmetric functions and 1004 points for asymmetric functions).");
    algorithm->add_option ("--runTimeLimit",           runTimeLimit,           "Sets the maximum allowed duration in minutes for Trekker to continue tracking. Tracking stops when time limit is reached. When tracking stops due to time limit, all the streamlines that are computed so far are written to the disk.");
    algorithm->add_option ("--idleTimeLimit",          idleTimeLimit,          "Sets the maximum allowed duration in minutes from the computation of the last successful streamline, i.e., if no new streamlines were computed within this time, Trekker stops.");
    
    
    
    // Sampling options
    auto sampling = tracking->add_option_group("SAMPLING PARAMETERS");
    sampling->add_option ("--maxEstInterval",         maxEstInterval,         "For random sampling, an estimate for the maximum posterior probability is needed. If set to n, this estimate is computed at every n^th step. Default=1.");
    sampling->add_option ("--initMaxEstTrials",       initMaxEstTrials,       "Number of trials done for the estimation of maximum posterior probability during initialization. Default: is set adaptively based on previous trials.");
    sampling->add_option ("--propMaxEstTrials",       propMaxEstTrials,       "Number of trials done for the estimation of maximum posterior probability during propagation. Default: is set adaptively based on previous trials.");
    sampling->add_option ("--maxSamplingPerStep",     maxSamplingPerStep,     "Maximum number of random sampling to select an acceptable candidate for propagation. Tracking stops if a suitable candidate cannot be found after this many attempts. Default: 1000.");
    sampling->add_flag   ("--useBestAtInit",          useBestAtInit,          "If used, during initialization, random sampling is skipped and the candidate that has the largest data support is used for propagation. Default: off.");
    sampling->add_flag   ("--useLegacySampling",      useLegacySampling,      "If used, during propagation, random sampling will be done using rejection sampling. Default: off.");
    sampling->add_option ("--samplingQuality",        samplingQuality,        "Integer value between 1 and 7 that controls the quality of sampling. Default=2.");

    // Probe options
    auto probe = tracking->add_option_group("PTT PROBE PARAMETERS");
    probe->add_option ("--probeLength",            probeLength,            "Length of the probe. Default=1/4 x the smallest of the FOD voxel dimensions.");
    probe->add_option ("--probeRadius",            probeRadius,            "Radius of the probe. Default=0.");
    probe->add_option ("--probeCount",             probeCount,             "probeCount determines the number of parallel lines used to model the cylinder. Default: 1 if probeRadius=0, 4 if probeRadius≠0.");
    probe->add_option ("--probeQuality",           probeQuality,           "This parameter sets the number of segments to split the cylinder along the length of the probe. Default=4.");
    probe->add_option ("--ignoreWeakLinks",        ignoreWeakLinks,        "If the individual data support (FOD amplitude along the direction of the segment) for any of the points in the probe is lower than what is specified with this parameter, this candidate probe is ignored. Default: 0.");

    // Seeding options
    auto seeding = tracking->add_option_group("SEEDING PARAMETERS");
    seeding->add_option ("--seed, -s",                              seedInp,         "Seed definition")->multi_option_policy(CLI::MultiOptionPolicy::Throw)->required();
    seeding->add_option ("--discard_seed",                          discardSeedInp,  "If a seed point falls into this region, it will be discarded")->multi_option_policy(CLI::MultiOptionPolicy::Throw);
    seeding->add_flag   ("--skipSeed",                              skipSeed,        "Does not output the points that are within seed region");
    // seeding->add_flag   ("--allowEdgeSeeds",                        allowEdgeSeeds,  "Allows seeding at the edges of pathway rules. Default: false");
    seedTrials_option  = seeding->add_option ("--seed_trials",      seedTrials,      "Sets the maximum number of attempts to generate streamline from the seed point. Default=1.");
    seedCount_option   = seeding->add_option ("--seed_count",          seedCount,       "Number of seeds. Trekker tries to generate a single streamline from each seed. For that it makes maximum amount of \"trials\". If an acceptable streamline cannot be generated then it is skipped. If this happens, there will be less streamlines in the output tractogram than what is defined with \"count\".");
    seedDensity_option = seeding->add_option ("--seed_density",        seedDensity,     "Density of seeds. If seed is an image, density is the number of seeds per mm^3. If seed is a surface mesh, density is the number of seeds per mm^2.");
    seeding->add_option("--seed_surf_faceDensity",      seed_surf_faceDensity,          "A text file containing density information for each face of the input seed surface.");
    seeding->add_option("--seed_surf_vertDensity",      seed_surf_vertDensity,          "A text file containing density information for each vertex of the input seed surface.");
    seeding->add_option("--seed_surf_fieldDensity",     seed_surf_fieldDensity,         "The given field in the input seed surface will be used to set seed density.");
    seed_surf_useSurfNorm_option    = seeding->add_flag("--seed_surf_useSurfNorm",      seed_surf_useSurfNorm,          "Surface normals will be used as the initial direction.");  

    // Pathway options
    auto pathwayOpt = app->add_option_group(center_text("PATHWAY OPTIONS",45));    
    pathwayOpt->add_option ("--pathway, -p",            pathway,                "Pathway rules. (See below for details.)")->multi_option_policy(CLI::MultiOptionPolicy::TakeAll);
    pathwayOpt->add_option ("--minlength",              minlength,              "Minimum length of output streamlines. Default=0");
    pathwayOpt->add_option ("--maxlength",              maxlength,              "Maximum length of output streamlines. Default=infinite");
    pathwayOpt->add_flag   ("--oneSided",               oneSided,               "If enabled tracking is done only towards the one direction. Default=OFF");
    pathwayOpt->add_flag   ("--stopAtMax",              stopAtMax,              "If used, propagation stops when maxLength is reached. By default, streamlines are discarded when propagation reaches maxLength.");
    pathwayOpt->add_flag   ("--inOrder",                inOrder,                "If enabled all pathway requirements are going to be satisfied in the order that they are input to Trekker-> All pathway options should be defined for pathway_A/pathway_B in order to use this option");

    app->callback(run_track);
    
}
