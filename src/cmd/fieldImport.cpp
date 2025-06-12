#include "cmd.h"

using namespace NIBR;

namespace CMDARGS_FIELDIMPORT {
    std::string inp_tractogram_fname;
    std::string inp_field_fname;

    std::string inp_field_owner;
    std::string inp_field_type;
    int         inp_field_dimension;
    std::string inp_field_name;

    bool isASCII = false;

    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;
}

using namespace CMDARGS_FIELDIMPORT;

void run_fieldImport()
{
 
    parseCommon(numberOfThreads,verbose);
    if (!ensureVTK(inp_tractogram_fname)) return;

    NIBR::TractogramReader tractogram(inp_tractogram_fname);
    if (!tractogram.isReady()) return;

    disp(MSG_DEBUG,"Streamline count: %d", tractogram.numberOfStreamlines);

    // Check if this field already exists
    int fieldId = -1;
    std::vector<NIBR::TractogramField> tmpFields = findTractogramFields(tractogram);
    for (size_t i=0; i<tmpFields.size(); i++) {
        disp(MSG_DEBUG,"Found field: %s", tmpFields[i].name.c_str());
        if (tmpFields[i].name == inp_field_name) {
            fieldId = i;
        }
    }
    
    if ((force==false) && (fieldId>-1)) {
        std::cout << "A field with name \"" << inp_field_name << "\" already exists in the tractogram. Use --force or -f to overwrite." << std::endl;
        return;
    }

    auto fields = readTractogramFields(tractogram);

    if ((force==true) && (fieldId>-1)) {
        clearField(fields[fieldId],tractogram);
        fields.erase(fields.begin()+fieldId);
    }    

    std::string extension = inp_field_name.substr(inp_field_name.find_last_of(".") + 1);
    
    if (extension == "csv") {
        NIBR::disp(MSG_ERROR,"Only binary files can be imported");
        return;
    } else {
        auto field = makeTractogramFieldFromFile(tractogram,inp_field_fname,inp_field_name,inp_field_owner,inp_field_type,inp_field_dimension,isASCII);
        fields.push_back(field);
    }

    NIBR::disp(MSG_DETAIL,"Writing %d fields", fields.size());

    writeTractogram(inp_tractogram_fname,tractogram.getTractogram(),fields);
    
    for (size_t i=0; i< fields.size(); i++) {
        NIBR::disp(MSG_DETAIL,"Deleting field %d: %s", i, fields[i].name.c_str());
        clearField(fields[i],tractogram);
    }
    
}

void fieldImport(CLI::App* app)
{
    app->description("adds a new field with values read from a file (.vtk only)");
    
    app->add_option("<input_tractogram>",           inp_tractogram_fname,   "Input tractogram")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("<input_field_data>",           inp_field_fname,        "Input field data")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("<input_field_owner>",          inp_field_owner,        "Can be either \"POINT\" or \"STREAMLINE\"")
        ->required();
        
    app->add_option("<input_field_data_type>",      inp_field_type,         "Can be either \"float\" or \"int\"")
        ->required();
        
    app->add_option("<input_field_data_dimension>", inp_field_dimension,    "Dimension of the field data")
        ->required();
        
    app->add_option("<field_name>",                 inp_field_name,         "Field name to write in the surface")
        ->required();

    app->add_option("--verbose, -v",                verbose,                "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",                    force,                  "Force overwriting of existing file");
    
    app->callback(run_fieldImport);

}
