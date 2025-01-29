#pragma once

#include "base/nibr.h"
#include "dMRI/tractography/mappers/tractogram2imageMapper.h"
#include "dMRI/tractography/mappers/gridder_4mask.h"

namespace NIBR
{

    template<typename T>
    bool index2image(
        TractogramReader& tractogram, 
        Image<T>& img, 
        bool*** masker,
        std::vector<int64_t>& inds,
        std::function<void(Tractogram2ImageMapper<T>* tim, int* _gridPos, NIBR::Segment& _seg)> processor, 
        std::function<void(Tractogram2ImageMapper<T>* tim)> indexer, 
        std::function<void(Tractogram2ImageMapper<T>* tim)> allocater,
        std::function<void(Tractogram2ImageMapper<T>* tim)> deallocater,
        void* data,
        std::string filePrefix, 
        int splitCount)
    {

        // Generate task ranges so the tractogram is split into splitCount segments
        std::vector<std::pair<int,int>> streamlineRange = MT::createTaskRange(tractogram.numberOfStreamlines,splitCount);
        int batchCount = 0;
        for (auto range : streamlineRange) {
            int streamlineCount = range.second - range.first + 1;
            batchCount += (streamlineCount>0);
        }
        
        // gridData: <data,nonZeroIndices,prefix,batchNo>
        std::tuple<void*,std::vector<int64_t>*,std::string*,int*> gridData;
        std::get<0>(gridData) = data;
        std::get<1>(gridData) = &inds;
        std::get<2>(gridData) = &filePrefix;

        int batchNo = 1;
        for (auto range : streamlineRange) {

            int streamlineCount = range.second - range.first + 1;

            if (streamlineCount<1) continue;

            disp(MSG_INFO, ("Indexing streamline batch " + std::to_string(batchNo) + " / " + std::to_string(batchCount)).c_str() );

            std::get<3>(gridData) = &batchNo;

            Tractogram2ImageMapper<T> gridder(&tractogram,&img);
            gridder.setData((void*)(&gridData));
            gridder.setMask(masker);
            allocater(&gridder);
            gridder.run(processor,indexer,range.first,range.second);
            deallocater(&gridder);

            batchNo++;

        }       
        
        return true;
    }

    template<typename T>
    bool index2image(
        TractogramReader& tractogram, 
        Image<T>& img, 
        NIBR::Image<int>& mask, 
        std::function<void(Tractogram2ImageMapper<T>* tim, int* _gridPos, NIBR::Segment& _seg)> processor, 
        std::function<void(Tractogram2ImageMapper<T>* tim)> indexer, 
        std::function<void(Tractogram2ImageMapper<T>* tim)> allocater,
        std::function<void(Tractogram2ImageMapper<T>* tim)> deallocater,
        void* data,
        std::string filePrefix, 
        int splitCount)
    {

        // Prepare mask and write voxel ids
        for (int i=0; i<3; i++) {
            if (img.pixDims[i]!=mask.pixDims[i]) {
                disp(MSG_ERROR, "Mask and template do not have same voxel dimensions.");
                return false;
            }

            if (img.imgDims[i]!=mask.imgDims[i]) {
                disp(MSG_ERROR, "Mask and template do not have same image dimensions.");
                return false;
            }

            for (int j=0; j<4; j++)
                if (img.xyz2ijk[i][j]!=mask.xyz2ijk[i][j]) {
                    disp(MSG_ERROR, "Mask and template images do not have the same coordinate space.");
                    return false;
                }
        }
        
        std::ofstream file_vId(filePrefix+"_vId.bin", std::ios::binary | std::ios::out);
        std::vector<int64_t> inds;

        for (int64_t i = 0; i < mask.voxCnt; i++) {
            if (mask.data[i]>0) {
                inds.push_back(i);
                file_vId.write(reinterpret_cast<const char*>(&i), sizeof(int64_t));
            }
        }

        file_vId.close();

        bool*** masker = new bool**[mask.imgDims[0]];
        for (int i = 0; i < mask.imgDims[0]; i++) {
            masker[i] = new bool*[mask.imgDims[1]];
            for (int j = 0; j < mask.imgDims[1]; j++) {
                masker[i][j] = new bool[mask.imgDims[2]];
                for (int k = 0; k < mask.imgDims[2]; k++)
                    masker[i][j][k] = ((mask)(i,j,k)>0) ? true : false;
            }
        }


        bool success = index2image(tractogram,img,masker,inds,processor,indexer,allocater,deallocater,data,filePrefix,splitCount);
        
        for (int i = 0; i < mask.imgDims[0]; i++) {
            for (int j = 0; j < mask.imgDims[1]; j++) {
                delete[] masker[i][j];
            }
            delete[] masker[i];
        }
        delete[] masker;
        
        return success;
    }

    template<typename T>
    bool index2image(
        TractogramReader& tractogram, 
        Image<T>& img, 
        std::function<void(Tractogram2ImageMapper<T>* tim, int* _gridPos, NIBR::Segment& _seg)> processor, 
        std::function<void(Tractogram2ImageMapper<T>* tim)> indexer, 
        std::function<void(Tractogram2ImageMapper<T>* tim)> allocater,
        std::function<void(Tractogram2ImageMapper<T>* tim)> deallocater,
        void* data,
        std::string filePrefix, 
        int splitCount)
    {

        Image<int> mask;

        auto refBb = img.getBoundingBox();
        mask.createFromBoundingBox(3,refBb,img.pixDims[0],false);
        
        Tractogram2ImageMapper<int> masker(&tractogram,&mask);    
        allocateGrid_4mask(&masker);
        masker.run(processor_4mask<int>, outputCompiler_4mask<int>);
        deallocateGrid_4mask(&masker);
        
        return NIBR::index2image(tractogram,img,mask,processor,indexer,allocater,deallocater,data,filePrefix,splitCount);

    }
    

    /*
    // Return a void* with indexing information for each ind value in inds
    template<typename T>
    std::vector<void*> index2image(
        TractogramReader& tractogram, 
        Image<T>& img, 
        bool*** masker,
        std::vector<int64_t>& inds,
        std::function<void(Tractogram2ImageMapper<T>* tim, int* _gridPos, NIBR::Segment& _seg)> processor, 
        std::function<void(Tractogram2ImageMapper<T>* tim)> indexer, 
        std::function<void(Tractogram2ImageMapper<T>* tim)> allocater,
        std::function<void(Tractogram2ImageMapper<T>* tim)> deallocater)
    {
        std::vector<void*> indexing;

        std::tuple<std::vector<int64_t>*,std::vector<void*>*> gridData;

        std::get<0>(gridData) = &inds;
        std::get<1>(gridData) = &indexing;

        Tractogram2ImageMapper<T> gridder(&tractogram,&img);
        gridder.setData((void*)(&gridData));
        gridder.setMask(masker);
        allocater(&gridder);
        gridder.run(processor,indexer);
        deallocater(&gridder);
        
        return indexing;
    }
    */

}


