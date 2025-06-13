#include "cmd.h"
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include <iostream>

using namespace NIBR;

namespace CMDARGS_TRANSFORM {
    std::string inp_fname;
    std::string out_fname;
    std::string trans;

    bool invert             = false;

    int numberOfThreads     =  0;
    std::string verbose     = "info";
    bool force              = false;
}

using namespace CMDARGS_TRANSFORM;


void run_transform()
{

    parseCommon(numberOfThreads,verbose);
    if (!parseForceOutput(out_fname,force)) return;

    std::ifstream file;
    file.open(trans.c_str());

    float M[4][4];
    for (int i=0; i<4; i++)
        for (int j=0; j<4; j++)
            M[i][j]=0;

    M[3][3] = 1;

    if (getFileExtension(trans)=="mat") {

        std::string values;
        getline(file,values);

        for (int i=0; i<3; i++)
            for (int j=0; j<4; j++)
                file >> M[i][j];

    } else if ((getFileExtension(trans)=="txt")) {

        std::string values;

        for (int i = 0; i < 3; i++) {
            std::getline(file,values);
            if (!values.empty()) {
                std::stringstream xyzt(values);
                xyzt >> M[i][0];
                xyzt >> M[i][1];
                xyzt >> M[i][2];
                xyzt >> M[i][3];
            }
        }

    }
    file.close();

    disp(MSG_DETAIL, "Read transform:");
    for (int i=0; i<4; i++) {
        disp(MSG_DETAIL, "[%.2f %.2f %.2f %.2f]", M[i][0],M[i][1],M[i][2],M[i][3]);
    }

    if (invert) {
        float tmp[4][4];
        NIBR::inverseAffine(M,tmp);
        for (int i=0; i<4; i++)
            for (int j=0; j<4; j++)
                M[i][j] = tmp[i][j];

        disp(MSG_DETAIL, "Inverted transform:");
        for (int i=0; i<4; i++) {
            disp(MSG_DETAIL, "[%.2f %.2f %.2f %.2f]", M[i][0],M[i][1],M[i][2],M[i][3]);
        }
    }


    NIBR::TractogramReader reader(inp_fname);
    if (!reader.isReady()) return;

    NIBR::TractogramWriter writer(out_fname);
    if (!writer.open()) {
        disp(MSG_FATAL, "Failed to open output file: %s", out_fname.c_str());
        return;
    }
    
    const size_t batch_size  = 50000;
    size_t       batch_count = 0;

    while (true) {

        NIBR::StreamlineBatch input_batch = reader.getNextStreamlineBatch(batch_size);

        if (input_batch.empty()) break;

        NIBR::MT::SET_DISP_RANGE(batch_count*batch_size,reader.numberOfStreamlines);

        NIBR::StreamlineBatch output_batch = NIBR::tractogramTransform(input_batch,M);

        if (!writer.writeBatch(std::move(output_batch))) {
            disp(MSG_ERROR, "Failed to write batch %d.", batch_count);
            break;
        }

        batch_count++;
        
    }

    if (!writer.close()) {
        disp(MSG_ERROR, "Failed to finalize and close output file.");
    } else {
        disp(MSG_INFO, "Processing finished successfully.");
    }    

    return;
}
  
  
void transform(CLI::App* app) 
{  

    app->description("transform tractogram");

    app->add_option("<input_tractogram>",       inp_fname,          "Input tractogram (.vtk, .tck, .trk)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("<transform>",              trans,              "4x4 transform matrix (.mat, .txt). (.mat is ANTs' format.)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("<output_tractogram>",      out_fname,          "Output tractogram (.vtk, .tck)")
        ->required();

    app->add_flag("--invert, -i",               invert,             "Apply inverse of the transform");

    app->add_option("--numberOfThreads, -n",    numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",            verbose,            "Verbose level. Options are \"quiet\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",                force,              "Force overwriting of existing file");

    app->callback(run_transform);
    
} 

