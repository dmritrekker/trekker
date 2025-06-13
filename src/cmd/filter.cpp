#include "cmd.h"

namespace CMDARGS_FILTER
{

    std::string inp_fname;
    std::string out_fname;
    
    int maxOut              = -1;
    bool inOrder            = false;
    bool skipSeed           = false;
    // bool allowEdgeSeeds     = false;
    int  seedTrials         = 0;
    bool stopAtMax          = false;
    bool oneSided           = false;
    
    float minlength         = 0;
    float maxlength         = FLT_MAX;
 
    std::vector<std::string> pathway; 
    std::vector<std::string> seedList;
    std::vector<std::string> discardSeedList;

    std::string saveDisc    = "";

    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;
    
}

using namespace CMDARGS_FILTER;



void run_filter()
{

    parseCommon(numberOfThreads,verbose);
    if (!parseForceOutput(out_fname,force)) return;
    if (saveDisc!="") { if (!parseForceOutput(saveDisc,force)) return; }

    if (!ensureVTKorTCK(out_fname)) return;

    // Initialize tractogram
    NIBR::TractogramReader reader(inp_fname);
    if (!reader.isReady()) return;

    if (reader.numberOfStreamlines < 1) {
        NIBR::Tractogram track;
        disp(MSG_ERROR,"Empty tractogram file.");
        NIBR::writeTractogram(out_fname, track);
        return;
    }  
 
    // Prepare pathway rules
    Pathway pw;
    pw.enableTracking(false); // This will prevent seeders to be created when a seed rule is defined

    auto seed  = parseSeedInput(seedList);
    if ((!seedList.empty()) && (seed.src==undef_src))
        return;
    if (seed.src != undef_src) {
        if(!pw.add(seed)) return;
    }

    std::vector<std::string> tmp;
    for (size_t n = 0; n < discardSeedList.size(); n++) {
        tmp.push_back("discard_seed");
        tmp.push_back(discardSeedList[n]);
    }

    auto discardSeed = parsePathwayInput(tmp);
    if ((!tmp.empty()) && discardSeed.empty())
        return;
    for (auto r : discardSeed) {
        if(!pw.add(r)) return;
    }

    if(!pw.inOrder(inOrder))            return;
    if(!pw.setMinLength(minlength))     return;
    if(!pw.setMaxLength(maxlength))     return;
    if(!pw.stopAtMax(stopAtMax))        return;
    if(!pw.oneSided(oneSided))          return;
    if(!pw.skipSeed(skipSeed))          return;
    // if((!seedList.empty()) && (!pw.noEdgeSeed(!allowEdgeSeeds))) return;
    if(!pw.setSeedTrials(seedTrials))   return;
 
    auto rules = parsePathwayInput(pathway);
    if ((!pathway.empty()) && rules.empty())
        return;
    for (auto r : rules) {
        if(!pw.add(r)) return;
    }

    pw.print();


    // Run pathFilter

    NIBR::TractogramWriter keepWriter(out_fname);
    if (!keepWriter.open()) {
        disp(MSG_FATAL, "Failed to open output file: %s", out_fname.c_str());
        return;
    }

    std::unique_ptr<NIBR::TractogramWriter> discWriter = nullptr;

    if (saveDisc != "") {
        discWriter = std::make_unique<NIBR::TractogramWriter>(saveDisc);
        if (!discWriter->open()) {
            disp(MSG_FATAL, "Failed to open output file: %s", saveDisc.c_str());
            return;
        }
    }


    
    int batch_size  = 50000;
    int batch_count = 0;
    int success     = 0;

    pw.startLogger(reader.numberOfStreamlines);
    pw.showLogger();

    while (true) {

        if (maxOut >= 0) {
            batch_size = std::min(maxOut-success,50000);
            if (batch_size <= 0) break;
        }

        NIBR::StreamlineBatch input_batch = reader.getNextStreamlineBatch(batch_size);

        if (input_batch.empty()) break;

        auto out = pw.apply(input_batch, (saveDisc != "") );

        success += std::get<0>(out).size();

        if (!std::get<0>(out).empty()) {
            if (!keepWriter.writeBatch(std::move(std::get<0>(out)))) {
                disp(MSG_ERROR, "Failed to write kept batch %d.", batch_count);
                break;
            }
        }

        if (!std::get<2>(out).empty()) {
            if (!discWriter->writeBatch(std::move(std::get<2>(out)))) {
                disp(MSG_ERROR, "Failed to write discarded batch %d.", batch_count);
                break;
            }
        }

        batch_count++;
        
    }

    pw.stopLogger();

    if (!keepWriter.close()) {
        disp(MSG_ERROR, "Failed to finalize and close output file.");
    } else {
        disp(MSG_INFO, "Output streamlines written.");
    }

    if (saveDisc != "") {
        if (!discWriter->close()) {
            disp(MSG_ERROR, "Failed to finalize and close output file.");
        } else {
            disp(MSG_INFO, "Discarded streamlines written.");
        }
    }

    disp(MSG_INFO, "Done.");

    return;
} 

void filter(CLI::App *app) 
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
        "\n       \033[1m Note 4:\033[0m It is possible change the place of the surface options, e.g. \'-s surf.vtk label,3 0.5 2D\' is same as \'-s surf.vtk 0.5 label,3 2D\'";

    setInfo(app,info);

    app->description("filters tractograms");

    app->add_option("<input_tractogram>",    inp_fname,          "Input tractogram (.vtk, .tck, .trk)")->required()->check(CLI::ExistingFile)->type_name("");   
    app->add_option ("--output,-o",          out_fname,          "Output tractogram (.vtk, .tck)")->required()->type_name("FILE");

    app->add_option("--pathway, -p",         pathway,            "Pathway rule")->multi_option_policy(CLI::MultiOptionPolicy::TakeAll);
    app->add_option("--seed, -s",            seedList,           "Seed definition")->multi_option_policy(CLI::MultiOptionPolicy::Throw);
    app->add_option("--discard_seed",        discardSeedList,    "Discard seed definition")->multi_option_policy(CLI::MultiOptionPolicy::Throw);
    app->add_option("--minlength",           minlength,          "Minimum length of the tracks. Default=0");
    app->add_option("--maxlength",           maxlength,          "Maximum length of the tracks. Default=infinite");

    app->add_flag("--stopAtMax",             stopAtMax,          "If set to \"stop\", when maxLength is reached the propagation stops and the streamline is written in the output. If set to \"discard\", the streamline is not written in the output. Default=discard");
    app->add_flag("--oneSided",              oneSided,           "If enabled tracking is done only towards the one direction.");
    app->add_flag("--skipSeed",              skipSeed,           "Does not output the points that are within seed region");
    // app->add_flag("--allowEdgeSeeds",        allowEdgeSeeds,     "Allows seeding at the edges of pathway rules. Default: false");
    app->add_option("--seed_trials",         seedTrials,         "Number of random trials for assigning seed. Default: 0");
    app->add_flag("--inOrder",               inOrder,            "If enabled all pathway requirements are going to be satisfied in the order that they are given. All pathway options should be defined for pathway_A/pathway_B in order to use this option");

    app->add_option("--maxOut",              maxOut,             "Maximum number of output streamlines.");

    app->add_option("--saveDiscarded",       saveDisc,           "Path for saving discarded streamlines");

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");

    app->callback(run_filter);
}
                                              
                                               