#include "conf/config.h"
#include "cmd/cmd.h"

using namespace NIBR;

std::string TREKKER_CMD_LINE;

int main(int argc, char *argv[]) {

    NIBR::INITIALIZE();

    for(int i=0; i<argc; i++) {
        TREKKER_CMD_LINE += std::string(argv[i]) + " ";
    }

    // Parse input
    CLI::App app(TREKKER_EXE_STRING);
    
    argv = app.ensure_utf8(argv);

    app.footer("----------" + TREKKER_EXE_STRING + "---------\n© Copyright 2026, Dogu Baran Aydogan, baran.aydogan@uef.fi\n");
    // app.failure_message(CLI::FailureMessage::help);

    app.require_subcommand(1);
    
    addColor(app.add_subcommand("addColor", ""));
    convert(app.add_subcommand("convert", ""));
    diff(app.add_subcommand("diff", ""));
    dMRI_cmd(app.add_subcommand("dMRI", ""));
    fieldExport(app.add_subcommand("fieldExport", ""));
    fieldImport(app.add_subcommand("fieldImport", ""));
    fieldRemove(app.add_subcommand("fieldRemove", ""));
    filter(app.add_subcommand("filter", ""));
    harp(app.add_subcommand("harp", ""));
    img2track(app.add_subcommand("img2track", ""));
    info(app.add_subcommand("info", ""));
    merge(app.add_subcommand("merge", ""));
    prepXact(app.add_subcommand("prepXact", ""));
    purifibre(app.add_subcommand("purifibre", ""));
    resample(app.add_subcommand("resample", ""));
    select(app.add_subcommand("select", ""));
    track(app.add_subcommand("track", ""));
    track2img(app.add_subcommand("track2img", ""));
    track2surf(app.add_subcommand("track2surf", ""));  
    transform(app.add_subcommand("transform", ""));    

    std::vector<std::string> subcommands = {
        "addColor",
        "convert",
        "diff",
        "dMRI",
        "fieldExport",
        "fieldImport",
        "fieldRemove",
        "filter",
        "harp",
        "img2track",
        "info", 
        "merge",
        "prepXact",
        "purifibre",
        "resample",
        "select",
        "track",
        "track2img",
        "track2surf",
        "transform"
    };

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

