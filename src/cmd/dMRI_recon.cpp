#include "cmd.h"

void dMRI_recon(CLI::App* app)
{
    app->description("dMRI reconstruction");
    app->require_subcommand(1);

    dMRI_recon_transhi2015(app->add_subcommand("tran_shi_2015", ""));
}
