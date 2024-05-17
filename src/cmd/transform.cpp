#include "cmd.h"
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include <iostream>

using namespace NIBR;

namespace CMDARGS_TRANSFORM {
    std::string inp_fname;
    std::string out_fname;
    std::string transform;

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

    if(!ensureVTKorTCK(inp_fname))      return;
    if(!ensureVTKorTCK(out_fname))      return;

    std::ifstream file;
    file.open(CMDARGS_TRANSFORM::transform.c_str());

    float M[4][4];
    for (int i=0; i<4; i++)
        for (int j=0; j<4; j++)
            M[i][j]=0;

    M[3][3] = 1;

    if (getFileExtension(CMDARGS_TRANSFORM::transform)=="mat") {

        std::string values;
        getline(file,values);

        for (int i=0; i<3; i++)
            for (int j=0; j<4; j++)
                file >> M[i][j];

    } else if ((getFileExtension(CMDARGS_TRANSFORM::transform)=="txt")) {

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


    NIBR::TractogramReader tractogram(inp_fname);

    auto out = NIBR::applyTransform(&tractogram,M);

    NIBR::writeTractogram(out_fname, out);

    if (out.size() == 0)
        NIBR::disp(MSG_INFO,"Output is empty.");
    else if (out.size() == 1)
        NIBR::disp(MSG_INFO,"1 streamline is written.");
    else
        NIBR::disp(MSG_INFO,"%d streamlines are written.", out.size());
    

    return;
}
  
  
void transform(CLI::App* app) 
{  

    app->description("transform tractogram");

    app->add_option("<input tractogram>", inp_fname, "Input tractogram (.vtk, .tck)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("<transform>", CMDARGS_TRANSFORM::transform, "4x4 transform matrix (.mat, .txt). (.mat is ANTs' format.)")
        ->required()
        ->check(CLI::ExistingFile);

    app->add_option("<output tractogram>", out_fname, "Output tractogram (.vtk, .tck)")
        ->required();

    app->add_flag("--invert, -i",            invert,             "Apply inverse of the transform");

    app->add_option("--numberOfThreads, -n", numberOfThreads,    "Number of threads.");
    app->add_option("--verbose, -v",         verbose,            "Verbose level. Options are \"quite\",\"fatal\",\"error\",\"warn\",\"info\" and \"debug\". Default=info");
    app->add_flag("--force, -f",             force,              "Force overwriting of existing file");

    app->callback(run_transform);
    
} 

