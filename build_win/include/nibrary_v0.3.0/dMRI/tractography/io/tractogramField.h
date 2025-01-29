#pragma once

#include "base/nibr.h"
#include "base/dataTypeHandler.h"
#include "dMRI/tractography/io/tractogramReader.h"

using namespace NIBR;

namespace NIBR
{
    typedef enum {
        STREAMLINE_OWNER,
        POINT_OWNER,
        OWNER_NOTSET
    } TractogramOwnerType;

    struct TractogramField {
        TractogramOwnerType     owner{OWNER_NOTSET};
        std::string             name{""};
        DATATYPE                datatype{UNKNOWN_DT};
        int                     dimension{0};
        void*                   data{NULL};
    };

    void clearField(TractogramField& field, TractogramReader& tractogram);
    void clearField(TractogramField& field, std::vector<std::vector<std::vector<float>>>& tractogram);

    std::vector<NIBR::TractogramField> findTractogramFields(TractogramReader& tractogram);
    std::vector<NIBR::TractogramField> readTractogramFields(TractogramReader& tractogram);

    TractogramField readTractogramField(TractogramReader& tractogram,std::string name);

    TractogramField makeTractogramFieldFromFile(TractogramReader& tractogram, std::string filePath, std::string name, std::string owner, std::string dataType, int dimension, bool isASCII);

}


template<class T>
void clearFieldWrapper(TractogramField& field,TractogramReader& tractogram) {    

    if (field.data != NULL) {

        if (field.owner == POINT_OWNER) {

            T*** toDel = reinterpret_cast<T***>(field.data);

            for (size_t s = 0; s < tractogram.numberOfStreamlines; s++) {
                for (uint32_t l = 0; l < tractogram.len[s]; l++) {
                    delete[] toDel[s][l];
                }
                delete[] toDel[s];
            }

            delete[] toDel; 

        }

        if (field.owner == STREAMLINE_OWNER) {

            T** toDel = reinterpret_cast<T**>(field.data);

            for (size_t s = 0; s < tractogram.numberOfStreamlines; s++) {
                delete[] toDel[s];
            }
            
            delete[] toDel;

        }

    }

    field.owner     = POINT_OWNER;
    field.name      = "";
    field.datatype  = UNKNOWN_DT;
    field.dimension = 0;
    field.data      = NULL;

}

template<class T>
void clearFieldWrapper(TractogramField& field, std::vector<std::vector<std::vector<float>>>& tractogram) {    

    if (field.data != NULL) {

        if (field.owner == POINT_OWNER) {

            T*** toDel = reinterpret_cast<T***>(field.data);

            for (size_t s = 0; s < tractogram.size(); s++) {
                for (uint32_t l = 0; l < tractogram[s].size(); l++) {
                    delete[] toDel[s][l];
                }
                delete[] toDel[s];
            }

            delete[] toDel; 

        }

        if (field.owner == STREAMLINE_OWNER) {

            T** toDel = reinterpret_cast<T**>(field.data);

            for (size_t s = 0; s < tractogram.size(); s++) {
                delete[] toDel[s];
            }
            
            delete[] toDel;

        }

    }

    field.owner     = POINT_OWNER;
    field.name      = "";
    field.datatype  = UNKNOWN_DT;
    field.dimension = 0;
    field.data      = NULL;

}

