#include "conf/config.h"
#include "cmd/cmd.h"

using namespace NIBR;

int main(int argc, char *argv[]) {

    std::cout << "Print test: 0\n" << std::flush;

    
    NIBR::INITIALIZE();

    NIBR::disp(MSG_INFO, "Here!");

    std::cout << "Print test: 1\n" << std::flush;

    
    // Parse input
    CLI::App app("Trekker v1.0.0-rc3");
    app.footer("----------Trekker v1.0.0-rc3---------\n© Copyright 2025, Dogu Baran Aydogan, baran.aydogan@uef.fi\n");
    app.failure_message(CLI::FailureMessage::help);

    app.require_subcommand(1);
    
    info(app.add_subcommand("info", ""));
    // track(app.add_subcommand("track", ""));
    // filter(app.add_subcommand("filter", ""));
    // track2img(app.add_subcommand("track2img", ""));
    // track2surf(app.add_subcommand("track2surf", ""));
    // img2track(app.add_subcommand("img2track", ""));
    // select(app.add_subcommand("select", ""));
    // resample(app.add_subcommand("resample", ""));
    // convert(app.add_subcommand("convert", ""));
    // transform(app.add_subcommand("transform", ""));
    // diff(app.add_subcommand("diff", ""));
    // merge(app.add_subcommand("merge", ""));
    // addColor(app.add_subcommand("addColor", ""));
    // fieldExport(app.add_subcommand("fieldExport", ""));
    // fieldImport(app.add_subcommand("fieldImport", ""));
    // fieldRemove(app.add_subcommand("fieldRemove", ""));
    // dMRI_cmd(app.add_subcommand("dMRI", ""));

    // If no option is used just display the help
    try {

        std::cout << "try parse \n" << std::flush;
        std::cout << argc << " parameters \n" << std::flush;

        if (argv[0] != nullptr) {
            app.parse(argc, argv);
        } else {
            displayHelp(app.help());
            NIBR::TERMINATE();
            return EXIT_SUCCESS;
        }
            
        std::cout << "try done \n" << std::flush;

    } catch(const CLI::ParseError &e) {

        std::cout << "catch \n" << std::flush;

        // Check if a subcommand is run with no arguments/options, then display help
        CLI::App* subcmd = &app;
        int argCount = 1;

        while (subcmd->get_subcommands().size()>0) {
            subcmd   = subcmd->get_subcommands()[0];
            argCount = subcmd->count_all();
        }

        if (argCount==1) { // Check if subcmd is run with no arguments/options
            displayHelp(app.help());
            NIBR::TERMINATE();
            return EXIT_SUCCESS;
        }

        std::cout << "app exit \n" << std::flush;

        auto q = app.exit(e);

        std::cout << "TERMINATE \n" << std::flush;

        NIBR::TERMINATE();

        std::cout << "TERMINATE done \n" << std::flush;
        return q;

    }

    NIBR::TERMINATE();
    std::cout << "Print test: 2\n" << std::flush;
    
    return EXIT_SUCCESS;

}

