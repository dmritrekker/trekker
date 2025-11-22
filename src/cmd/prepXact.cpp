#include "cmd.h"

using namespace NIBR;

// Labels
//  1 - out_L_WM    (Left White Matter)
//  2 - R_WM    (Right White Matter)
//  3 - L_GM    (Left Gray Matter)
//  4 - R_GM    (Right Gray Matter)
//  5 - L_SUB   (Left Subcortical)
//  6 - R_SUB   (Right Subcortical)
//  7 - CSF     (Cerebrospinal Fluid)
//  8 - CER_WM  (Cerebellar White Matter)
//  9 - CER_GM  (Cerebellar Gray Matter)
// 10 - BS      (Brain Stem)
// 11 - I_BS    (Inferior Brain Stem)
// 12 - BG      (Background)


namespace CMDARGS_PREPXACT {
    std::string fsPath;
    std::string fslFirstFolder = "";

    std::string out_COMBINED    = "";
    std::string out_L_WM        = "";
    std::string out_R_WM        = "";
    std::string out_L_GM        = "";
    std::string out_R_GM        = "";
    std::string out_L_SUB       = "";
    std::string out_R_SUB       = "";
    std::string out_CSF         = "";
    std::string out_CER_WM      = "";
    std::string out_CER_GM      = "";
    std::string out_BS          = "";
    std::string out_I_BS        = "";
    std::string out_BG          = "";
    
    float cereDistThresh            = 0.5;
    float enlargeBrainStem          = 5;
    float inferiorBrainStemCutLevel = 20;
    float faceArea                  = 0.25;

    int numberOfThreads     = 0;
    std::string verbose     = "info";
    bool force              = false;
}

using namespace CMDARGS_PREPXACT;

void run_prepXact()
{

    parseCommon(numberOfThreads,verbose);
    if (!parseForceOutput(out_COMBINED,force)   || ((out_COMBINED != "")    && !ensureVTK(out_COMBINED) ) ) return;
    if (!parseForceOutput(out_L_WM,force)       || ((out_L_WM != "")        && !ensureVTK(out_L_WM)     ) ) return;
    if (!parseForceOutput(out_R_WM,force)       || ((out_R_WM != "")        && !ensureVTK(out_R_WM)     ) ) return;
    if (!parseForceOutput(out_L_GM,force)       || ((out_L_GM != "")        && !ensureVTK(out_L_GM)     ) ) return;
    if (!parseForceOutput(out_R_GM,force)       || ((out_R_GM != "")        && !ensureVTK(out_R_GM)     ) ) return;
    if (!parseForceOutput(out_L_SUB,force)      || ((out_L_SUB != "")       && !ensureVTK(out_L_SUB)    ) ) return;
    if (!parseForceOutput(out_R_SUB,force)      || ((out_R_SUB != "")       && !ensureVTK(out_R_SUB)    ) ) return;
    if (!parseForceOutput(out_CSF,force)        || ((out_CSF != "")         && !ensureVTK(out_CSF)      ) ) return;
    if (!parseForceOutput(out_CER_WM,force)     || ((out_CER_WM != "")      && !ensureVTK(out_CER_WM)   ) ) return;
    if (!parseForceOutput(out_CER_GM,force)     || ((out_CER_GM != "")      && !ensureVTK(out_CER_GM)   ) ) return;
    if (!parseForceOutput(out_BS,force)         || ((out_BS != "")          && !ensureVTK(out_BS)       ) ) return;
    if (!parseForceOutput(out_I_BS,force)       || ((out_I_BS != "")        && !ensureVTK(out_I_BS)     ) ) return;
    if (!parseForceOutput(out_BG,force)         || ((out_BG != "")          && !ensureVTK(out_BG)       ) ) return;

    XactPrepOption opt = XACT_PREP_OPT_UNSET;

    opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_COMBINED);

    if (out_L_WM     != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_L_WM);
    if (out_R_WM     != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_R_WM);
    if (out_L_GM     != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_L_GM);
    if (out_R_GM     != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_R_GM);
    if (out_L_SUB    != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_L_SUB);
    if (out_R_SUB    != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_R_SUB);
    if (out_CSF      != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_CSF);
    if (out_CER_WM   != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_CER_WM);
    if (out_CER_GM   != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_CER_GM);
    if (out_BS       != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_BS);
    if (out_I_BS     != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_I_BS);
    if (out_BG       != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_BG);
    

    auto surf = prepXact(fsPath,fslFirstFolder,NULL,cereDistThresh,enlargeBrainStem,inferiorBrainStemCutLevel,faceArea,opt);

    surf[0].write(out_COMBINED);

    if (out_L_WM     != "")  surf[1].write(out_L_WM);
    if (out_R_WM     != "")  surf[2].write(out_R_WM);
    if (out_L_GM     != "")  surf[3].write(out_L_GM);
    if (out_R_GM     != "")  surf[4].write(out_R_GM);
    if (out_L_SUB    != "")  surf[5].write(out_L_SUB);
    if (out_R_SUB    != "")  surf[6].write(out_R_SUB);
    if (out_CSF      != "")  surf[7].write(out_CSF);
    if (out_CER_WM   != "")  surf[8].write(out_CER_WM);
    if (out_CER_GM   != "")  surf[9].write(out_CER_GM);
    if (out_BS       != "")  surf[10].write(out_BS);
    if (out_I_BS     != "")  surf[11].write(out_I_BS);
    if (out_BG       != "")  surf[12].write(out_BG);

    return;

}

void prepXact(CLI::App* app)
{

    app->formatter(std::make_shared<CustomHelpFormatter>());
 
    const std::string info = "XACT output can be used for fiber tracking using the `track` command with the `xact` option for anatomically constrained whole-brain tractography.";

    setInfo(app,info);

    app->description("creates XACT surfaces for whole-brain tractography (experimental)");
    
    app->add_option("<Freesurfer_folder>", fsPath, "Path to Freesurfer aseg file")
        ->required()
        ->check(CLI::ExistingDirectory);

    app->add_option("<Output_xact_file>", out_COMBINED, "xact output (.vtk)")
        ->required()->type_name("FILE");

    app->add_option("--l_wm",       out_L_WM,       "Left white matter. (Label = 1)"         );
    app->add_option("--r_wm",       out_R_WM,       "Right white matter. (Label = 2)"        );
    app->add_option("--l_gm",       out_L_GM,       "Left gray matter. (Label = 3)"          );
    app->add_option("--r_gm",       out_R_GM,       "Right gray matter. (Label = 4)"         );
    app->add_option("--l_sub",      out_L_SUB,      "Left subcortical. (Label = 5)"          );
    app->add_option("--r_sub",      out_R_SUB,      "Right subcortical. (Label = 6)"         );
    app->add_option("--csf",        out_CSF,        "Cerebrospinal fluid. (Label = 7)"       );
    app->add_option("--cer_wm",     out_CER_WM,     "Cerebellar white matter. (Label = 8)"   );
    app->add_option("--cer_gm",     out_CER_GM,     "Cerebellar gray matter. (Label = 9)"    );
    app->add_option("--bs",         out_BS,         "Brain stem. (Label = 10)"               );
    app->add_option("--i_bs",       out_I_BS,       "Inferior brain stem. (Label = 11)"      );
    app->add_option("--bg",         out_BG,         "Background. (Label = 12)"               );

    app->add_option("--fslFirst", fslFirstFolder, "Use FSL First results for subcortical and brainstem surfaces");

    app->add_option("--cereDistThresh",             cereDistThresh,             "Distance threshold from the brain stem to separate cerebellar white matter. Default: 0.5 mm");
    app->add_option("--enlargeBrainStem",           enlargeBrainStem,           "Enlarge or shrink brain stem. Default: 5 mm");
    app->add_option("--inferiorBrainStemCutLevel",  inferiorBrainStemCutLevel,  "Cut level for inferior brain stem. Default: 20 mm");

    app->add_option("--faceArea",            faceArea,           "Average area of output triangles. Default: 0.25 mm^2");

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.")->check(CLI::Range(0, INT32_MAX));
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quite\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");
    
    app->callback(run_prepXact);

}
