#include "cmd.h"

using namespace NIBR;

namespace CMDARGS_FIELDEXPORT {
    std::string inp_tractogram_fname;
    std::string out_fname;
    std::vector<std::string> field_name_list;
    bool skipWritingFieldName = false;

    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;
}

using namespace CMDARGS_FIELDEXPORT;

void run_fieldExport()
{
    parseCommon(numberOfThreads,verbose);
    if (!parseForceOutput(out_fname,force)) return;
    
    if (!ensureVTK(inp_tractogram_fname)) return;

    NIBR::TractogramReader tractogram(inp_tractogram_fname);
    if (!tractogram.isReady()) return;
    
    std::vector<NIBR::TractogramField> fields;

    NIBR::TractogramOwnerType owner = OWNER_NOTSET;
    
    for (size_t fieldNo=0; fieldNo<field_name_list.size(); fieldNo++) {
        
        auto field = readTractogramField(tractogram,field_name_list[fieldNo]);
        
        if (field.data!=NULL) {
            
            if (owner==OWNER_NOTSET) 
                owner = field.owner;
            else if (owner!=field.owner) {
                std::cout << "All fields should be either POINT or STREAMLINE" << std::endl;
                for (std::vector<NIBR::TractogramField>::iterator it=fields.begin(); it!=fields.end(); it++) {
                    clearField(*it,tractogram);
                }
                return;
            }
                
            fields.push_back(field);
        } else {
            disp(MSG_WARN,"Ignoring %s, which does not exist in the tractogram",field_name_list[fieldNo].c_str());
        }

    }
    
    size_t N = fields.size();
        
    if (N>0) {
        
        FILE *out;

        out = fopen(out_fname.c_str(),"wb");

        char buffer[256];
        
        if (skipWritingFieldName==false) {
            for (std::vector<NIBR::TractogramField>::iterator it=fields.begin(); it!=fields.end(); it++) {
                
                if (it->dimension==1) {
                    sprintf(buffer, "%s ", it->name.c_str()); 
                    fwrite(buffer, sizeof(char), strlen(buffer), out);
                } else {
                    for (int d=0; d<it->dimension; d++) {
                        sprintf(buffer, "%s_dim_%d ", it->name.c_str(), d); 
                        fwrite(buffer, sizeof(char), strlen(buffer), out);
                    }
                }
            }
            sprintf(buffer, "\n"); fwrite(buffer, sizeof(char), strlen(buffer), out);
        }

        auto cumLen = tractogram.getNumberOfPoints();
        
        if (owner==POINT_OWNER){
            for (size_t s=0; s<tractogram.numberOfStreamlines; s++) {
                uint32_t len = cumLen[s+1] - cumLen[s];
                for (uint32_t l=0; l<len; l++) {
                    for (std::vector<NIBR::TractogramField>::iterator it=fields.begin(); it!=fields.end(); it++) {
                        for (int d=0; d<it->dimension; d++) {
                            if (it->datatype==FLOAT32_DT) {sprintf(buffer, "%6.6f ",((float***)(it->data))[s][l][d]); fwrite(buffer, sizeof(char), strlen(buffer), out);}
                            if (it->datatype==INT32_DT)   {sprintf(buffer, "%d ",     ((int***)(it->data))[s][l][d]); fwrite(buffer, sizeof(char), strlen(buffer), out);}
                        }
                    }    
                    sprintf(buffer, "\n"); fwrite(buffer, sizeof(char), strlen(buffer), out);
                }
            }
        }
        
        if (owner==STREAMLINE_OWNER){
            for (size_t i=0; i<tractogram.numberOfStreamlines; i++) {
                for (std::vector<NIBR::TractogramField>::iterator it=fields.begin(); it!=fields.end(); it++) {
                    for (int d=0; d<it->dimension; d++) {
                        if (it->datatype==FLOAT32_DT) {sprintf(buffer, "%6.6f ",((float**)(it->data))[i][d]); fwrite(buffer, sizeof(char), strlen(buffer), out);}
                        if (it->datatype==INT32_DT)   {sprintf(buffer, "%d ",     ((int**)(it->data))[i][d]); fwrite(buffer, sizeof(char), strlen(buffer), out);}
                    }
                }    
                sprintf(buffer, "\n"); fwrite(buffer, sizeof(char), strlen(buffer), out);
            }
        }
        
        
        fclose(out);
        
    } else {
        NIBR::disp(MSG_WARN,"Field(s) not found");
    }
    
}

void fieldExport(CLI::App* app)
{
    app->description("exports a field from a tractogram (.vtk only)");
    
    app->add_option("<input_tractogram>",    inp_tractogram_fname,  "Input tractogram")
        ->required()
        ->check(CLI::ExistingFile);        
    
    app->add_flag("--skipFieldNames,-s",     skipWritingFieldName,  "Skip writing of field names in the first row of output");
    
    app->add_option("--fields",              field_name_list,       "Names of the fields to export that are separated with comma, e.g, field1,field2,field4 ")
        ->required()
        ->delimiter(',');

    app->add_option("<output_file>",         out_fname,             "Output file name (.csv)")
        ->required();

    app->add_option("--verbose, -v",         verbose,               "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,                 "Force overwriting of existing file");
    
    app->callback(run_fieldExport);

}
