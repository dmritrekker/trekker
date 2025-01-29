#pragma once

#include "base/nibr.h"
#include <stdio.h>
#include "dMRI/tractography/mappers/tractogram2imageMapper.h"

namespace NIBR 
{

    struct sim_stickZeppelinWM_GM_CSF {
        std::vector<std::vector<float>> grad;

        float d_WM_para{0.0022};
        float d_WM_perp{0.0002};

        float d_GM{0.0009};
        float d_CSF{0.003};

        float T1WM{832000};
        float T1GM{1331000};
        float T1CSF{37000};
        float T2WM{44000};
        float T2GM{51000};
        float T2CSF{500000};

        float TE{57000};
        float TR{88000};
    };

    template<typename T>
    inline void allocateGrid_4dMRIsim_stickZeppelinWM_GM_CSF(Tractogram2ImageMapper<T>* tim) {
        tim->template allocateGrid<float*>();
    }

    template<typename T>
    inline void deallocateGrid_4dMRIsim_stickZeppelinWM_GM_CSF(Tractogram2ImageMapper<T>* tim) {
        tim->template deallocateGrid<float*>();
    }

    template<typename T>
    inline void processor_4dMRIsim_stickZeppelinWM_GM_CSF(Tractogram2ImageMapper<T>* tim, int* gridPos, NIBR::Segment& seg) {

        int64_t ind = tim->img->sub2ind(gridPos[0],gridPos[1],gridPos[2]);

        {
            std::lock_guard<std::mutex> lock(tim->gridMutex[ind]);

            sim_stickZeppelinWM_GM_CSF* data = (sim_stickZeppelinWM_GM_CSF*)(tim->data);

            if (tim->grid[ind]==NULL) {
                float* sig      = new float[data->grad.size()];
                tim->grid[ind]  = ((void*)sig); // Allocate memory here
            }

            for (size_t i = 0; i < data->grad.size(); i++) {
                float dotPro  = seg.dir[0]*data->grad[i][0] + seg.dir[1]*data->grad[i][1] + seg.dir[2]*data->grad[i][2];
                float D_intra = 0.8 * std::exp(-data->grad[i][3]*data->d_WM_para)*dotPro*dotPro;

                float D[3][3];
                for (int m = 0; m < 3; m++) {
                    for (int n = 0; n < 3; n++) {
                        D[m][n] = seg.dir[m]*seg.dir[n]*(data->d_WM_para-data->d_WM_perp);
                        if (m==n)   {
                            D[m][n] += data->d_WM_perp;
                        }
                    }
                }

                float gDg = 0;
                for (int m = 0; m < 3; m++) {
                    float tmp = 0;
                    for (int n = 0; n < 3; n++) {
                        tmp += data->grad[i][n]*D[n][m];
                    }
                    gDg += tmp*data->grad[i][m];
                }

                float D_extra = 0.2 * std::exp(-data->grad[i][3]*gDg);

                ((float*)(tim->grid[ind]))[i] = D_intra + D_extra; // can be multiplied using weights, see tim->weightFile
            }

        }
    }

    template<typename T>
    inline void outputCompiler_4dMRIsim_stickZeppelinWM_GM_CSF(Tractogram2ImageMapper<T>* tim) {

        // Set index order for image such that the 4th dimension
        tim->img->indexOrder[0] = 3;
        tim->img->indexOrder[1] = 0;
        tim->img->indexOrder[2] = 1;
        tim->img->indexOrder[3] = 2;
        tim->img->indexOrder[4] = 4;
        tim->img->indexOrder[5] = 5;
        tim->img->indexOrder[6] = 6;
        tim->img->allocData();


        auto genOut = [&](NIBR::MT::TASK task)->void{

            int64_t ind    = task.no;
            int64_t sub[7] = {0,0,0,0,0,0,0};

            int64_t i,j,k,sind;

            tim->img->ind2sub(ind,i,j,k);

            if (tim->grid[ind]!=NULL) {
                
                sub[0] = i;
                sub[1] = j;
                sub[2] = k;
                sind   = tim->img->sub2ind(sub);

                for (size_t i = 0; i < ((sim_stickZeppelinWM_GM_CSF*)(tim->data))->grad.size(); i++) {
                    tim->img->data[sind+i] = ((float*)(tim->grid[ind]))[i];
                }

                delete ((float*)(tim->grid[ind])); // Deallocate memory here
                tim->grid[ind] = NULL;
            }

        };
        NIBR::MT::MTRUN(tim->img->voxCnt, genOut);

    }

}
