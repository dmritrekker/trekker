#include "cmd.h"

using namespace NIBR;

// Labels
//  1 - L_WM    (Left White Matter)
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

    std::string COMBINED    = "";
    std::string L_WM        = "";
    std::string R_WM        = "";
    std::string L_GM        = "";
    std::string R_GM        = "";
    std::string L_SUB       = "";
    std::string R_SUB       = "";
    std::string CSF         = "";
    std::string CER_WM      = "";
    std::string CER_GM      = "";
    std::string BS          = "";
    std::string I_BS        = "";
    std::string BG          = "";
    
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
    if (!parseForceOutput(COMBINED,force)   || ((COMBINED != "")    && !ensureVTK(COMBINED) ) ) return;
    if (!parseForceOutput(L_WM,force)       || ((L_WM != "")        && !ensureVTK(L_WM)     ) ) return;
    if (!parseForceOutput(R_WM,force)       || ((R_WM != "")        && !ensureVTK(R_WM)     ) ) return;
    if (!parseForceOutput(L_GM,force)       || ((L_GM != "")        && !ensureVTK(L_GM)     ) ) return;
    if (!parseForceOutput(R_GM,force)       || ((R_GM != "")        && !ensureVTK(R_GM)     ) ) return;
    if (!parseForceOutput(L_SUB,force)      || ((L_SUB != "")       && !ensureVTK(L_SUB)    ) ) return;
    if (!parseForceOutput(R_SUB,force)      || ((R_SUB != "")       && !ensureVTK(R_SUB)    ) ) return;
    if (!parseForceOutput(CSF,force)        || ((CSF != "")         && !ensureVTK(CSF)      ) ) return;
    if (!parseForceOutput(CER_WM,force)     || ((CER_WM != "")      && !ensureVTK(CER_WM)   ) ) return;
    if (!parseForceOutput(CER_GM,force)     || ((CER_GM != "")      && !ensureVTK(CER_GM)   ) ) return;
    if (!parseForceOutput(BS,force)         || ((BS != "")          && !ensureVTK(BS)       ) ) return;
    if (!parseForceOutput(I_BS,force)       || ((I_BS != "")        && !ensureVTK(I_BS)     ) ) return;
    if (!parseForceOutput(BG,force)         || ((BG != "")          && !ensureVTK(BG)       ) ) return;

    XactPrepOption opt = XACT_PREP_OPT_UNSET;

    opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_COMBINED);

    if (L_WM     != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_L_WM);
    if (R_WM     != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_R_WM);
    if (L_GM     != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_L_GM);
    if (R_GM     != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_R_GM);
    if (L_SUB    != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_L_SUB);
    if (R_SUB    != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_R_SUB);
    if (CSF      != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_CSF);
    if (CER_WM   != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_CER_WM);
    if (CER_GM   != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_CER_GM);
    if (BS       != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_BS);
    if (I_BS     != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_I_BS);
    if (BG       != "") opt = static_cast<XactPrepOption>(opt | XACT_PREP_OPT_BG);
    

    auto surf = prepXact(fsPath,fslFirstFolder,cereDistThresh,enlargeBrainStem,inferiorBrainStemCutLevel,faceArea,opt);

    surf[0].write(COMBINED);

    if (L_WM     != "")  surf[1].write(L_WM);
    if (R_WM     != "")  surf[2].write(R_WM);
    if (L_GM     != "")  surf[3].write(L_GM);
    if (R_GM     != "")  surf[4].write(R_GM);
    if (L_SUB    != "")  surf[5].write(L_SUB);
    if (R_SUB    != "")  surf[6].write(R_SUB);
    if (CSF      != "")  surf[7].write(CSF);
    if (CER_WM   != "")  surf[8].write(CER_WM);
    if (CER_GM   != "")  surf[9].write(CER_GM);
    if (BS       != "") surf[10].write(BS);
    if (I_BS     != "") surf[11].write(I_BS);
    if (BG       != "") surf[12].write(BG);

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

    app->add_option("<Output_xact_file>", COMBINED, "xact output (.vtk)")
        ->required()->type_name("FILE");

    app->add_option("--l_wm",       L_WM,       "Left white matter. (Label = 1)"         );
    app->add_option("--r_wm",       R_WM,       "Right white matter. (Label = 2)"        );
    app->add_option("--l_gm",       L_GM,       "Left gray matter. (Label = 3)"          );
    app->add_option("--r_gm",       R_GM,       "Right gray matter. (Label = 4)"         );
    app->add_option("--l_sub",      L_SUB,      "Left subcortical. (Label = 5)"          );
    app->add_option("--r_sub",      R_SUB,      "Right subcortical. (Label = 6)"         );
    app->add_option("--csf",        CSF,        "Cerebrospinal fluid. (Label = 7)"       );
    app->add_option("--cer_wm",     CER_WM,     "Cerebellar white matter. (Label = 8)"   );
    app->add_option("--cer_gm",     CER_GM,     "Cerebellar gray matter. (Label = 9)"    );
    app->add_option("--bs",         BS,         "Brain stem. (Label = 10)"               );
    app->add_option("--i_bs",       I_BS,       "Inferior brain stem. (Label = 11)"      );
    app->add_option("--bg",         BG,         "Background. (Label = 12)"               );

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
