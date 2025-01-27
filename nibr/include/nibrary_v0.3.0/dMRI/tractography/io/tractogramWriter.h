#pragma once

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <float.h>
#include "base/nibr.h"
#include "base/dataTypeHandler.h"
#include "base/vectorOperations.h"
#include "math/core.h"
#include "dMRI/tractography/io/tractogramReader.h"
#include "tractogramField.h"

namespace NIBR
{

    bool writeTractogram            (std::string out_fname,NIBR::TractogramReader* tractogram);
    bool writeTractogram            (std::string out_fname,std::vector<std::vector<std::vector<float>>>& tractogram);
    template<typename T>
    bool writeTractogram            (std::string out_fname,std::vector<std::vector<std::vector<float>>>& tractogram,Image<T>* refImg);
    bool writeTractogram            (std::string out_fname,NIBR::TractogramReader* tractogram,std::vector<size_t>& idx);
    bool writeTractogram            (std::string out_fname,std::string inp_fname,std::vector<size_t>& idx);
    bool writeTractogram            (std::string out_kept_fname,std::string out_rmvd_fname,std::string inp_fname,std::vector<size_t>& idx);

    bool writeTractogram_VTK_binary (std::string out_fname,NIBR::TractogramReader* tractogram);
    bool writeTractogram_VTK_binary (std::string out_fname,NIBR::TractogramReader* tractogram,std::vector<size_t>& idx);
    bool writeTractogram_VTK_binary (std::string out_fname,std::vector<std::vector<std::vector<float>>>& tractogram);

    bool writeTractogram_VTK_ascii  (std::string out_fname,NIBR::TractogramReader* tractogram);
    bool writeTractogram_VTK_ascii  (std::string out_fname,NIBR::TractogramReader* tractogram,std::vector<size_t>& idx);
    bool writeTractogram_VTK_ascii  (std::string out_fname,std::vector<std::vector<std::vector<float>>>& tractogram);

    bool writeTractogram_TRK        (std::string out_fname,NIBR::TractogramReader* tractogram);
    bool writeTractogram_TRK        (std::string out_fname,NIBR::TractogramReader* tractogram,std::vector<size_t>& idx);
    template<typename T>
    bool writeTractogram_TRK        (std::string out_fname,std::vector<std::vector<std::vector<float>>>& tractogram,Image<T>* refImg);

    bool writeTractogram_TCK        (std::string out_fname,NIBR::TractogramReader* tractogram);
    bool writeTractogram_TCK        (std::string out_fname,NIBR::TractogramReader* tractogram,std::vector<size_t>& idx);
    bool writeTractogram_TCK        (std::string out_fname,std::vector<std::vector<std::vector<float>>>& tractogram);


    // The following only works for VTK files
    bool writeTractogram(std::string fname,std::vector<std::vector<std::vector<float>>>& tractogram,std::vector<TractogramField>& fields);

}
