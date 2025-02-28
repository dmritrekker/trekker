#include "conf/config.h"
#include "cmd/cmd.h"

using namespace NIBR;

int main(int argc, char *argv[]) {

    NIBR::INITIALIZE();

    // Parse input
    CLI::App app(TREKKER_EXE_STRING);
    
    argv = app.ensure_utf8(argv);

    app.footer("----------" + TREKKER_EXE_STRING + "---------\n© Copyright 2025, Dogu Baran Aydogan, baran.aydogan@uef.fi\n");
    // app.failure_message(CLI::FailureMessage::help);

    app.require_subcommand(1);
    
    info(app.add_subcommand("info", ""));
    track(app.add_subcommand("track", ""));
    filter(app.add_subcommand("filter", ""));
    track2img(app.add_subcommand("track2img", ""));
    track2surf(app.add_subcommand("track2surf", ""));
    img2track(app.add_subcommand("img2track", ""));
    select(app.add_subcommand("select", ""));
    resample(app.add_subcommand("resample", ""));
    convert(app.add_subcommand("convert", ""));
    transform(app.add_subcommand("transform", ""));
    diff(app.add_subcommand("diff", ""));
    merge(app.add_subcommand("merge", ""));
    addColor(app.add_subcommand("addColor", ""));
    fieldExport(app.add_subcommand("fieldExport", ""));
    fieldImport(app.add_subcommand("fieldImport", ""));
    fieldRemove(app.add_subcommand("fieldRemove", ""));
    dMRI_cmd(app.add_subcommand("dMRI", ""));

    std::vector<std::string> subcommands = {
        "info", 
        "track", 
        "filter",
        "track2img",
        "track2surf",
        "img2track",
        "select",
        "resample",
        "transform",
        "diff",
        "merge",
        "addColor",
        "fieldExport",
        "fieldImport",
        "fieldRemove",
        "dMRI"};

    const std::string dMRI_name          = "dMRI";
    const std::string recon_name         = "recon";
    const std::string tran_shi_2015_name = "tran_shi_2015";

    // Trekker is called alone. Display help and terminate.
    if (argc == 1) {
        displayHelp(app.help());
        NIBR::TERMINATE();
        return EXIT_SUCCESS;
    }

    // If trekker is called with a single subcommand, display help and terminate.
    if (argc == 2) {
        for (int i = 1; i < argc; ++i) {
            for (const std::string& name : subcommands) {
                if ((argv[i] == name) && (i == (argc-1))) {
                    displayHelp(app.get_subcommand(name)->help());
                    NIBR::TERMINATE();
                    return EXIT_SUCCESS;
                }
            }
        }
    }

    if (argc == 3) {
        if ((argv[1] == dMRI_name ) && (argv[2] == recon_name)) {
            displayHelp(app.get_subcommand(dMRI_name)->get_subcommand(recon_name)->help());
            NIBR::TERMINATE();
            return EXIT_SUCCESS;
        }
    }

    if (argc == 4) {
        if ((argv[1] == dMRI_name) && (argv[2] == recon_name) && (argv[3] == tran_shi_2015_name)) {
            displayHelp(app.get_subcommand(dMRI_name)->get_subcommand(recon_name)->get_subcommand(tran_shi_2015_name)->help());
            NIBR::TERMINATE();
            return EXIT_SUCCESS;
        }
    }

    // Execute program
    try {
        CLI11_PARSE(app,argc,argv);
    } catch (const CLI::ParseError &e) {
        NIBR::TERMINATE();
        return EXIT_SUCCESS;
    }

    NIBR::TERMINATE();
    return EXIT_SUCCESS;

}

