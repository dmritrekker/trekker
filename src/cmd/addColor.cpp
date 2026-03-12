#include "cmd.h"

using namespace NIBR;

namespace CMDARGS_ADDCOLOR {
    std::string  inp_fname;
    std::string  out_fname;
    
    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;
}

using namespace CMDARGS_ADDCOLOR; 

 
void run_addColor()
{

    parseCommon(numberOfThreads,verbose);
    if (!parseForceOutput(out_fname,force)) return;

    std::string inp_ext = getFileExtension(inp_fname);
    std::string out_ext = getFileExtension(out_fname);

    if (out_ext != "vtk" && out_ext != "trx") {
        disp(MSG_ERROR, "Output must be .vtk or .trx");
        return;
    }

    NIBR::TractogramReader tractogram(inp_fname);
    if (!tractogram.isReady()) return;

    std::vector<NIBR::TractogramField> fields;

    if (out_ext == "vtk") {
        int fieldId = -1;
        std::vector<NIBR::TractogramField> tmpFields = findTractogramFields(tractogram);
        for (size_t i=0; i<tmpFields.size(); i++) {
            if (tmpFields[i].name == "RGB") fieldId = i;
        }
        if ((force==false) && (fieldId>-1)) {
            std::cout << "A field with name RGB already exists in the tractogram. Use --force or -f to overwrite." << std::endl;
            return;
        }
        fields = readTractogramFields(tractogram);
        if ((force==true) && (fieldId>-1)) {
            clearField(fields[fieldId],tractogram);
            fields.erase(fields.begin()+fieldId);
        }
    }

    auto colors = NIBR::colorTractogram(&tractogram);
    fields.push_back(colors);

    writeTractogram(out_fname, tractogram.getTractogram(), fields);

    for (size_t i=0; i< fields.size(); i++) {
        clearField(fields[i],tractogram);
    }

}


void addColor(CLI::App* app)
{

    app->description("adds direction-encoded colors to streamlines (.vtk or .trx)");

    app->add_option("<input_tractogram>", inp_fname, "Input tractogram (.vtk, .tck, .trk, .trx)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("<output_tractogram>", out_fname, "Output tractogram (.vtk or .trx)")
        ->required( );

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");

    app->callback(run_addColor);  
     
}

                       