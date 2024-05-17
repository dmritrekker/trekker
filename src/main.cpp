#include "conf/config.h"
#include "cmd/cmd.h"

using namespace NIBR;

int main(int argc, char *argv[]) {

    NIBR::INITIALIZE();

    // Parse input
    CLI::App app("Trekker v1.0 beta");
    app.footer("----------Trekker v1.0 beta---------\n© Copyright 2024, Dogu Baran Aydogan, baran.aydogan@uef.fi\n");
    // app.failure_message(CLI::FailureMessage::help);

    // app.require_subcommand(1);
    
    trekker(&app);

    // If no option is used just display the help
    try {

        app.parse(argc, argv);

    } catch(const CLI::ParseError &e) {

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

        auto q = app.exit(e);

        NIBR::TERMINATE();
        return q;

    }

    NIBR::TERMINATE();
    return EXIT_SUCCESS;

}

