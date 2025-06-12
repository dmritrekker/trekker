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
    if (!ensureVTK(inp_fname)) return;
    if (!ensureVTK(out_fname)) return;

    NIBR::TractogramReader tractogram(inp_fname);
    if (!tractogram.isReady()) return;

    int fieldId = -1;
    std::vector<NIBR::TractogramField> tmpFields = findTractogramFields(tractogram);
    for (size_t i=0; i<tmpFields.size(); i++) {
        if (tmpFields[i].name == "RGB") {
            fieldId = i;
        }
    }
    if ((force==false) && (fieldId>-1)) {
        std::cout << "A field with name RGB already exists in the tractogram. Use --force or -f to overwrite." << std::endl;
        return;
    }

    auto fields = readTractogramFields(tractogram);

    if ((force==true) && (fieldId>-1)) {
        clearField(fields[fieldId],tractogram);
        fields.erase(fields.begin()+fieldId);
    }

    
    auto colors = NIBR::colorTractogram(&tractogram);

    fields.push_back(colors);

    writeTractogram(out_fname,tractogram.getTractogram(),fields);

    for (size_t i=0; i< fields.size(); i++) {
        clearField(fields[i],tractogram);
    }
       
}          
    
     
void addColor(CLI::App* app)   
{ 

    app->description("adds colors to streamlines (.vtk only)");

    app->add_option("<input_tractogram>", inp_fname, "Input tractogram (.vtk)")
        ->required()
        ->check(CLI::ExistingFile);
     
    app->add_option("<output_tractogram>", out_fname, "Output tractogram (.vtk)")
        ->required( );

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");

    app->callback(run_addColor);  
     
}

                       