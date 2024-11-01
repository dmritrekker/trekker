#include "cmd.h"

void dMRI_cmd(CLI::App* app)
{
    app->description("dMRI commands");
    app->require_subcommand(1);

    dMRI_recon(app->add_subcommand("recon", ""));
}
