#include "cmd.h"

using namespace NIBR;

namespace CMDARGS_INFO {
    std::string  inp_fname;

    int numberOfThreads     =  0;
    std::string verbose     = "info";
}

using namespace CMDARGS_INFO;


void run_info()
{

    parseCommon(numberOfThreads,verbose);

    NIBR::TractogramReader tractogram;
    if (!tractogram.initReader(inp_fname)) {
        std::cout << "Can't read " << inp_fname << std::endl << std::flush;
        return;
    }


    std::cout << "Format:                   ";
    if (tractogram.fileFormat==NIBR::TCK)              std::cout << "tck"          << std::endl << std::flush;
    else if (tractogram.fileFormat==NIBR::TRK)         std::cout << "trk"          << std::endl << std::flush;
    else if (tractogram.fileFormat==NIBR::VTK_ASCII)   std::cout << "vtk (ascii)"  << std::endl << std::flush;
    else if (tractogram.fileFormat==NIBR::VTK_BINARY)  std::cout << "vtk (binary)" << std::endl << std::flush;
    else {
        std::cout << "unknown"      << std::endl << std::flush;
        return;
    }

    std::cout << "Description:              "  << tractogram.fileDescription      << std::endl << std::flush;
    std::cout << "Streamline count:         "  << tractogram.numberOfStreamlines  << std::endl << std::flush;
    std::cout << "Number of points:         "  << tractogram.numberOfPoints       << std::endl << std::flush;

    size_t total = 0;
    for (size_t i=0; i<tractogram.numberOfStreamlines; i++) {
        total += tractogram.len[i];
    }
    std::cout << "Number of points (check): "  << total                           << std::endl << std::flush;

    if (tractogram.fileFormat==NIBR::TRK) {

        std::cout << "ijk2xyz: " << std::endl << std::flush;
        
        std::cout << "   " << tractogram.ijk2xyz[0][0] << " " << tractogram.ijk2xyz[0][1] << " " << tractogram.ijk2xyz[0][2] << " " << tractogram.ijk2xyz[0][3] << std::endl << std::flush;
        std::cout << "   " << tractogram.ijk2xyz[1][0] << " " << tractogram.ijk2xyz[1][1] << " " << tractogram.ijk2xyz[1][2] << " " << tractogram.ijk2xyz[1][3] << std::endl << std::flush;
        std::cout << "   " << tractogram.ijk2xyz[2][0] << " " << tractogram.ijk2xyz[2][1] << " " << tractogram.ijk2xyz[2][2] << " " << tractogram.ijk2xyz[2][3] << std::endl << std::flush;
        std::cout << "   " << tractogram.ijk2xyz[3][0] << " " << tractogram.ijk2xyz[3][1] << " " << tractogram.ijk2xyz[3][2] << " " << tractogram.ijk2xyz[3][3] << std::endl << std::flush;

    }

    std::vector<NIBR::TractogramField> fields = findTractogramFields(tractogram);

    std::cout << "Field count: " << fields.size() << std::endl << std::flush;

    int i = 1;
    for (auto f : fields) {
        std::cout << "   " << i++ << ". " << f.name.c_str() << ": " << ((f.owner == POINT_OWNER) ? "Point" : "Streamline") << " field with dim " << f.dimension << std::endl << std::flush;
    }

}


void info(CLI::App* app)
{

    app->description("displays information about the input tractogram");

    app->add_option("<input tractogram>", inp_fname, "Input tractogram (.vtk, .tck,.trk)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quite\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    
    app->callback(run_info);
    
}

