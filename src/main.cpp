#include "conf/config.h"
#include "cmd/cmd.h"

using namespace NIBR;

int main(int argc, char *argv[]) {

    NIBR::INITIALIZE();

    // Parse input
    CLI::App app("Trekker v1.0.0-rc3");
    app.footer("----------Trekker v1.0.0-rc3---------\n© Copyright 2025, Dogu Baran Aydogan, baran.aydogan@uef.fi\n");
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

    
    CLI11_PARSE(app,argc,argv);
    

    // // If no option is used just display the help
    // try {

    //     std::cout << "argc: " << argc << std::endl << std::flush;

    //     if (argc > 1) {
    //         std::cout << "app.parse(argc, argv)" << std::endl << std::flush;
    //         app.parse(argc, argv);
    //         std::cout << "Done" << std::endl << std::flush;
    //     } else {
    //         std::cout << "Display help and exit" << std::endl << std::flush;
    //         displayHelp(app.help());
    //         NIBR::TERMINATE();
    //         return EXIT_SUCCESS;
    //     }

    //     std::cout << "Try success" << std::endl << std::flush;

    // } catch(const CLI::ParseError &e) {

    //     std::cout << "Caught exception" << std::endl << std::flush;

    //     // Check if a subcommand is run with no arguments/options, then display help
    //     CLI::App* subcmd = &app;
    //     int argCount = 1;

    //     std::cout << "Counting arguments" << std::endl << std::flush;

    //     while (subcmd->get_subcommands().size()>0) {
    //         std::cout << "Getting subcommands" << std::endl << std::flush;
    //         subcmd   = subcmd->get_subcommands()[0];
    //         std::cout << "Counting" << std::endl << std::flush;
    //         argCount = subcmd->count_all();
    //         std::cout << "Done" << std::endl << std::flush;
    //     }

    //     std::cout << "argCount: " << argCount << std::endl << std::flush;

    //     if (argCount<2) { // Check if subcmd is run with no arguments/options
    //         std::cout << "Display help and exit" << std::endl << std::flush;
    //         displayHelp(app.help());
    //         NIBR::TERMINATE();
    //         return EXIT_SUCCESS;
    //     }

    //     std::cout << "app.exit(e)" << std::endl << std::flush;

    //     auto q = app.exit(e);

    //     std::cout << "NIBR::TERMINATE()" << std::endl << std::flush;

    //     NIBR::TERMINATE();

    //     std::cout << "Finished handling exception" << std::endl << std::flush;

    //     return q;

    // }

    NIBR::TERMINATE();
    return EXIT_SUCCESS;

}

