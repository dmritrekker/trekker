#pragma once

#include "base/nibr.h"
#include "image/image.h"
#include "image/image_operators.h"
#include "math/conn3D.h"
#include <sys/types.h>
#include <tuple>
#include <queue>

namespace NIBR
{

    // imgErode
    template<typename T>
    void imgErode(NIBR::Image<T>& inp, CONN3D conn)
    {

        auto N = get3DNeighbors(conn);

        std::vector<int> inds = getNonZeroIndices(&inp);

        std::set<int> marked;

        auto f = [&](MT::TASK task)->void {
            
            int64_t i,j,k; 

            inp.ind2sub(inds[task.no],i,j,k);

            for (auto n : N) {

                int64_t ii = i + n[0];
                int64_t jj = j + n[1];
                int64_t kk = k + n[2];

                if ( inp.isInside(ii,jj,kk) && (inp(ii,jj,kk)==0) ) {
                    MT::PROC_MX().lock();
                    marked.insert(inds[task.no]);
                    MT::PROC_MX().unlock();
                    break;
                }

            }

        };

        MT::MTRUN(inds.size(),f);
        
        for (auto m : marked) {
            inp.data[m] = 0;
        }

    }
    
    template<typename T_OUT,typename T_INP>
    void imgErode(NIBR::Image<T_OUT>& out, NIBR::Image<T_INP>& inp, CONN3D conn) 
    {
        out = inp;
        imgErode(out,conn);
    }

    template<typename T>
    void imgErode(NIBR::Image<T>& inp) 
    {
        imgErode(inp,CONN6);
    }



    // imgDilate
    template<typename T>
    void imgDilate(NIBR::Image<T>& inp, CONN3D conn)
    {

        auto N = get3DNeighbors(conn);

        std::vector<int> inds = getNonZeroIndices(&inp);

        auto f = [&](MT::TASK task)->void {
            
            int64_t i,j,k; 

            inp.ind2sub(inds[task.no],i,j,k);

            for (auto n : N) {

                int64_t ii = i + n[0];
                int64_t jj = j + n[1];
                int64_t kk = k + n[2];

                if ( inp.isInside(ii,jj,kk) && (inp(ii,jj,kk)==0) ) {
                    MT::PROC_MX().lock();
                    *inp.at(ii,jj,kk) = 1;
                    MT::PROC_MX().unlock();
                }

            }

        };
        
        MT::MTRUN(inds.size(),f);

    }
    
    template<typename T_OUT,typename T_INP>
    void imgDilate(NIBR::Image<T_OUT>& out, NIBR::Image<T_INP>& inp, CONN3D conn) 
    {
        out = inp;
        imgDilate(out,conn);
    }

    template<typename T>
    void imgDilate(NIBR::Image<T>& inp) 
    {
        imgDilate(inp,CONN6);
    }



    // Connected component labeling
    template<typename T_INP>
    std::tuple<NIBR::Image<int>, std::vector<std::size_t>> imgConnectedComponents(NIBR::Image<T_INP>& inp, CONN3D conn)
    {

        if (inp.numberOfDimensions != 3) {
            disp(MSG_ERROR,"Input has to be 3-dimensional for connected component labeling");
            std::make_tuple(NIBR::Image<int>(), std::vector<size_t>());
        }

        // Initialize label image with zeros (background)
        NIBR::Image<int> labels;
        labels.createFromTemplate(inp,true);
        for (auto n = 0; n < labels.numel; n++) labels.data[n] = 0;
        
        // Vector to store voxel counts per label. Index 0 is unused (background)
        std::vector<size_t> voxelCounts;
        voxelCounts.push_back(0); // Placeholder for background

        auto N = get3DNeighbors(conn);
        int current_label = 1; // Start labeling from 1

        // Iterate through each voxel
        for(int64_t k = 0; k < inp.imgDims[2]; ++k)
        {
            for(int64_t j = 0; j < inp.imgDims[1]; ++j)
            {
                for(int64_t i = 0; i < inp.imgDims[0]; ++i)
                {
                    // Check if voxel is foreground and not yet labeled
                    if(inp(i, j, k) != 0 && labels(i, j, k) == 0)
                    {
                        // Initialize voxel count for the new component
                        size_t voxelCount = 0;

                        // Perform BFS to label all connected voxels
                        std::queue<std::tuple<int64_t, int64_t, int64_t>> q;
                        q.push({i, j, k});
                        (*labels.at(i, j, k)) = current_label;
                        voxelCount++;

                        while(!q.empty())
                        {
                            auto [x, y, z] = q.front();
                            q.pop();

                            for(const auto& n : N)
                            {
                                int64_t xn = x + n[0];
                                int64_t yn = y + n[1];
                                int64_t zn = z + n[2];

                                if(inp.isInside(xn, yn, zn) && inp(xn, yn, zn) != 0 && labels(xn, yn, zn) == 0)
                                {
                                    (*labels.at(xn, yn, zn)) = current_label;
                                    q.push({xn, yn, zn});
                                    voxelCount++;
                                }
                            }
                        }

                        // Append the voxel count for this component
                        voxelCounts.push_back(voxelCount);

                        ++current_label; // Increment label for next component
                    }
                }
            }
        }

        return std::make_tuple(labels, voxelCounts);
    }


    template<typename T_INP>
    std::tuple<NIBR::Image<int>, std::vector<std::size_t>> imgConnectedComponents(NIBR::Image<T_INP>& inp)
    {
        return imgConnectedComponents(inp, CONN6);
    }

}