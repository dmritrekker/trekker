#include "cmd.h"

using namespace NIBR;

namespace CMDARGS_FIELDREMOVE {
    std::string inp_tractogram;
    std::string field_name;

    int numberOfThreads     =  0;
    std::string verbose     = "info";
}

using namespace CMDARGS_FIELDREMOVE;

void run_fieldRemove()
{
 
    parseCommon(numberOfThreads,verbose);

    if (!ensureVTKorTRX(inp_tractogram)) return;

    bool isTrx = (getFileExtension(inp_tractogram) == "trx");
    NIBR::TractogramReader tractogram(inp_tractogram, false, isTrx);
    if (!tractogram.isReady()) return;

    // Check if this field already exists
    int fieldId = -1;
    std::vector<NIBR::TractogramField> tmpFields = findTractogramFields(tractogram);
    for (size_t i=0; i<tmpFields.size(); i++) {
        disp(MSG_DEBUG,"Found field: %s", tmpFields[i].name.c_str());
        if (tmpFields[i].name == field_name) {
            fieldId = i;
        }
    }
    
    if (fieldId==-1) {
        disp(MSG_WARN,"Field %s was not found", field_name.c_str());
        return;
    }

    auto fields = readTractogramFields(tractogram);

    clearField(fields[fieldId],tractogram);
    fields.erase(fields.begin()+fieldId);

    auto groups = isTrx ? tractogram.getGroups() : std::map<std::string,std::vector<uint32_t>>{};
    writeTractogram(inp_tractogram, tractogram.getTractogram(), fields, groups);
    
    for (size_t i=0; i< fields.size(); i++) {
        NIBR::disp(MSG_DETAIL,"Deleting field %d: %s", i, fields[i].name.c_str());
        clearField(fields[i],tractogram);
    }
    
}

void fieldRemove(CLI::App* app)
{
    app->description("removes a field from a tractogram (.trx or .vtk)");
    
    app->add_option("<input_tractogram>",    inp_tractogram,     "Input tractogram")
        ->required()
        ->check(CLI::ExistingFile);
        
    
    app->add_option("<field_name>",          field_name,         "Field name to remove")
        ->required();

    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    
    app->callback(run_fieldRemove);

}
