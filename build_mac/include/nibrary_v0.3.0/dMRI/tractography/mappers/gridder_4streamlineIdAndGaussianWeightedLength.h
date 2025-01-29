#pragma once

#include "base/nibr.h"
#include "math/core.h"
#include "math/gaussian.h"
#include "dMRI/tractography/mappers/tractogram2imageMapper.h"
#include "dMRI/tractography/utility/segmentOperators.h"

namespace NIBR 
{

    template<typename T>
    inline void allocateGrid_4streamlineIdAndGaussionWeightedLength(Tractogram2ImageMapper<T>* tim) {
        tim->template allocateGrid<std::unordered_map<int,float>>();
    }

    template<typename T>
    inline void deallocateGrid_4streamlineIdAndGaussionWeightedLength(Tractogram2ImageMapper<T>* tim) {
        tim->template deallocateGrid<std::unordered_map<int,float>>();
    }

    template<typename T>
    inline void processor_4streamlineIdAndGaussionWeightedLength(Tractogram2ImageMapper<T>* tim, int* gridPos, NIBR::Segment& seg) {

        int64_t ind = tim->img->sub2ind(gridPos[0],gridPos[1],gridPos[2]);
        
        float voxCenter[3], p[3], step[3];
        // float stepSize = tim->img->pixDims[0] * 0.05;
        static float stepSize = [&]() { return tim->img->pixDims[0] * 0.05; }();

        auto G =  (Gaussian3D_evaluator_4squaredDist*)(std::get<0>(*(std::tuple<void*,std::vector<int64_t>*,std::string*,int*>*)(tim->data)));
        tim->img->to_xyz(gridPos, &voxCenter[0]);

        int fullSteps = static_cast<int>(seg.length / stepSize);
        float rem     = seg.length - fullSteps * stepSize;
        
        for (int i = 0; i < 3; i++) {
            step[i] = stepSize * seg.dir[i];
            p[i]    = seg.p[i];
        }

        float lineIntegral = 0.0;

        // Handle full steps
        for (int i = 0; i < fullSteps; ++i) {
            lineIntegral += G->eval(squared_dist(voxCenter, p)) * stepSize;
            p[0] += step[0];
            p[1] += step[1];
            p[2] += step[2];
        }

        // Handle the remaining length
        lineIntegral += G->eval(squared_dist(voxCenter, p)) * rem;

        {
            std::lock_guard<std::mutex> lock(tim->gridMutex[ind]);

            if (tim->grid[ind]==NULL) { // Allocate memory here
                std::unordered_map<int,float>* streamlineIdsAndGaussionWeightedLength = new std::unordered_map<int,float>();
                tim->grid[ind] = ((void*)streamlineIdsAndGaussionWeightedLength); // Each thread handles a single std::unordered_map<int,float> for each voxel in the output
            }

            (*((std::unordered_map<int,float>*)(tim->grid[ind])))[seg.streamlineNo] += lineIntegral;

        }


    }



    template<typename T>
    inline void indexStreamlineIdAndGaussionWeightedLength(Tractogram2ImageMapper<T>* tim) {

        // auto data       =  std::get<0>(*(std::tuple<void*,std::vector<int64_t>*,std::string*,int*>*)(tim->data)); // Not used this particular function
        auto inds       =  std::get<1>(*(std::tuple<void*,std::vector<int64_t>*,std::string*,int*>*)(tim->data));
        auto prefix     = *std::get<2>(*(std::tuple<void*,std::vector<int64_t>*,std::string*,int*>*)(tim->data));
        auto batchNo    = *std::get<3>(*(std::tuple<void*,std::vector<int64_t>*,std::string*,int*>*)(tim->data));

        std::ofstream file_idx(prefix + "_idx_" + std::to_string(batchNo) + ".bin", std::ios::binary | std::ios::out);
        std::ofstream file_pos(prefix + "_pos_" + std::to_string(batchNo) + ".bin", std::ios::binary | std::ios::out);

        int64_t numberOfIndices = inds->size();

        std::vector<std::streampos> positions;
        positions.resize(numberOfIndices+1); // Also add end of file, which is handy when reading between pos[n] pos[n+1]

        std::unordered_map<int,float> noIdx;
        noIdx[-1] = 0;

        int starting  = 0;
        int remaining = numberOfIndices;
        int batchSize = (remaining < 125000) ? remaining : 125000;

        while (remaining > 0) {

            std::vector<std::unordered_map<int,float>*> batchData;
            batchData.resize(batchSize);
            std::atomic_int counter(0);

            auto run = [&](NIBR::MT::TASK task)->void{

                int64_t ind = inds->at(starting+task.no);

                if (tim->grid[ind]!=NULL) {
                    auto vals          = (std::unordered_map<int,float>*)(tim->grid[ind]);
                    batchData[task.no] = vals;
                    counter.fetch_add(vals->size(), std::memory_order_relaxed);
                } else {
                    batchData[task.no] = &noIdx;
                    counter.fetch_add(1, std::memory_order_relaxed);
                }


            };
            NIBR::MT::MTRUN(batchSize,run);

            // First write on buffer then on disc
            std::vector<char> writeBuffer;
            writeBuffer.reserve(counter * (sizeof(int)+sizeof(float)));
            
            for (int i = 0; i < batchSize; i++) {

                // Store the position for this streamline
                positions[i+starting] = file_idx.tellp() + std::streampos(writeBuffer.size());

                // Write contents of batchData to write buffer
                for(const auto &p : *batchData[i]) {
                    std::copy(reinterpret_cast<const char*>(&(p.first )), reinterpret_cast<const char*>(&(p.first )) + sizeof(int),   std::back_inserter(writeBuffer));
                    std::copy(reinterpret_cast<const char*>(&(p.second)), reinterpret_cast<const char*>(&(p.second)) + sizeof(float), std::back_inserter(writeBuffer));
                    // std::cout << "streamline: " << p.first << " - value: " << p.second << std::endl;
                }

            }

            // Write buffer on the disc
            file_idx.write(writeBuffer.data(), writeBuffer.size());
            writeBuffer.clear();

            starting  += batchSize;
            remaining -= batchSize;

            if ((remaining>0) && (remaining<batchSize)) {
                batchSize = remaining;
            }

        }

        
        positions[numberOfIndices] = file_idx.tellp();
        file_idx.close();

        for(const auto &pos : positions) {
            file_pos.write(reinterpret_cast<const char*>(&pos), sizeof(pos));
        }
        file_pos.close();

        
        /*
        std::cout << "Size of positions: "  << positions.size() << std::endl;
        std::cout << "Positions: "          << positions[0] << " - " << positions[1] << std::endl;
        std::cout << "Size of a pos: "      << sizeof(std::streampos) << std::endl  << std::endl ;
        */

    }


    /*
    template<typename T>
    inline void indexStreamlineIdAndGaussionWeightedLengthToData(Tractogram2ImageMapper<T>* tim) {

        using indData = std::tuple<std::vector<int64_t>*,std::vector<void*>*>;

        auto inds     = std::get<1>(*(indData*)(tim->data));
        auto indexing = std::get<2>(*(indData*)(tim->data));

        int64_t numberOfIndices = inds->size();
        
        indexing->resize(numberOfIndices);

        auto indexStreamlines = [&](NIBR::MT::TASK task)->void{
            const int64_t& ind = inds->at(task.no);

            if (tim->grid[ind] != NULL) {
                indexing->at(task.no) = tim->grid[ind];
                tim->grid[ind]        = NULL;
            } else {
                indexing->at(task.no) = (void*)(new std::unordered_map<int,float>());
            }
            
        };
        NIBR::MT::MTRUN(numberOfIndices,indexStreamlines);

    }
    */

}
