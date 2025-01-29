#pragma once

#include <stdint.h>
#include <cmath>

#include "base/nibr.h"

namespace NIBR
{

    template<typename T>
    class Image;

    typedef enum {
        INTERP_INSIDE,
        INTERP_BOUNDARY,
        INTERP_OUTSIDE
    } INTERPAT;

    typedef enum {
        NEAREST,
        LINEAR,
        CUBIC
    } INTERPMETHOD;

    template<typename OUT_T, typename INP_T>
    class Interpolator {
        
    public:
        
        Interpolator();
        ~Interpolator(){};


        static INTERPAT  init_interp_linear   (NIBR::Image<INP_T>* img, OUT_T *p, OUT_T* cfs, int64_t* cor_ijk);
        static OUT_T     interp_linear_3D     (NIBR::Image<INP_T>* img, OUT_T* p);
        static OUT_T     interp_linear_4D_att (NIBR::Image<INP_T>* img, OUT_T* p, int64_t t);
        static void      interp_linear_4D_all (NIBR::Image<INP_T>* img, OUT_T* p, OUT_T* out);
        
        static INTERPAT  init_interp_nearest  (NIBR::Image<INP_T>*,     OUT_T*, int64_t*);
        static OUT_T     interp_nearest_3D    (NIBR::Image<INP_T>* img, OUT_T*);
        static OUT_T     interp_nearest_4D_att(NIBR::Image<INP_T>* img, OUT_T* , int64_t );
        static void      interp_nearest_4D_all(NIBR::Image<INP_T>*,     OUT_T*, OUT_T*);
        
        // TODO: Cubic interpolation
        static INTERPAT  init_interp_cubic    (NIBR::Image<INP_T>* img, OUT_T*, OUT_T*, int64_t*) {return img->outsideVal;}
        static OUT_T     interp_cubic_3D      (NIBR::Image<INP_T>* img, OUT_T*)                   {return img->outsideVal;}
        static OUT_T     interp_cubic_4D_att  (NIBR::Image<INP_T>* img, OUT_T* , int64_t )        {return img->outsideVal;}
        static void      interp_cubic_4D_all  (NIBR::Image<INP_T>*,     OUT_T*, OUT_T*)           {}
    };

    // ==========================================
    // SPEED IS REDUCED IF FUNCTIONS ARE DEFINED IN A SEPARATE CPP FILE
    template<typename OUT_T, typename INP_T>
    INTERPAT Interpolator<OUT_T,INP_T>::init_interp_linear(NIBR::Image<INP_T>* img, OUT_T *p, OUT_T* cfs, int64_t* cor_ijk) 
    {
        
        OUT_T ijk[3];
        img->to_ijk(p,ijk);
        
        cor_ijk[0]  = int64_t(ijk[0] + 1.0f);
        cor_ijk[1]  = int64_t(ijk[1] + 1.0f);
        cor_ijk[2]  = int64_t(ijk[2] + 1.0f);
        
        cfs[0]      = cor_ijk[0] - ijk[0];
        cfs[1]      = cor_ijk[1] - ijk[1];
        cfs[2]      = cor_ijk[2] - ijk[2];

        if ( cor_ijk[0]>0 && cor_ijk[1]>0 && cor_ijk[2]>0 && cor_ijk[0]<img->imgDims[0] && cor_ijk[1]<img->imgDims[1] && cor_ijk[2]<img->imgDims[2] )
            return INTERP_INSIDE;
        
        if ( cor_ijk[0]<0 || cor_ijk[1]<0 || cor_ijk[2]<0 || cor_ijk[0]>=img->imgDims[0] || cor_ijk[1]>=img->imgDims[1] || cor_ijk[2]>=img->imgDims[2] )
            return INTERP_OUTSIDE;
        
        if (cfs[0]>1) cfs[0] -= 1.0f;
        if (cfs[1]>1) cfs[1] -= 1.0f;
        if (cfs[2]>1) cfs[2] -= 1.0f;
        if (cfs[0]<0) cfs[0] += 1.0f;
        if (cfs[1]<0) cfs[1] += 1.0f;
        if (cfs[2]<0) cfs[2] += 1.0f;

        return INTERP_BOUNDARY;
        
    }


    template<typename OUT_T, typename INP_T>
    OUT_T Interpolator<OUT_T,INP_T>::interp_linear_3D(NIBR::Image<INP_T>* img, OUT_T* p)
    {
        
        OUT_T     cfs[3];
        int64_t   cor_ijk[3];
        
        switch (init_interp_linear(img, p, cfs, cor_ijk)) {
        
            case INTERP_INSIDE:
                return          cfs[0] *   cfs[1] *   cfs[2] *OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]-1,cor_ijk[2]-1)]) +
                             (1-cfs[0])*   cfs[1] *   cfs[2] *OUT_T(img->data[img->sub2ind(cor_ijk[0],   cor_ijk[1]-1,cor_ijk[2]-1)]) +
                                cfs[0] *(1-cfs[1])*   cfs[2] *OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1],  cor_ijk[2]-1)]) +
                             (1-cfs[0])*(1-cfs[1])*   cfs[2] *OUT_T(img->data[img->sub2ind(cor_ijk[0],   cor_ijk[1],  cor_ijk[2]-1)]) +
                                cfs[0] *   cfs[1] *(1-cfs[2])*OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]-1,cor_ijk[2]  )]) +
                             (1-cfs[0])*   cfs[1] *(1-cfs[2])*OUT_T(img->data[img->sub2ind(cor_ijk[0],   cor_ijk[1]-1,cor_ijk[2]  )]) +
                                cfs[0] *(1-cfs[1])*(1-cfs[2])*OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1],  cor_ijk[2]  )]) +
                             (1-cfs[0])*(1-cfs[1])*(1-cfs[2])*OUT_T(img->data[img->sub2ind(cor_ijk[0],   cor_ijk[1],  cor_ijk[2]  )]);
                
            case INTERP_BOUNDARY: {
                
                OUT_T out = 0;

                out +=    cfs[0] *   cfs[1] *   cfs[2]  * ((img->isInside(cor_ijk[0]-1,cor_ijk[1]-1,cor_ijk[2]-1)) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]-1,cor_ijk[2]-1)]) : img->outsideVal);
                out += (1-cfs[0])*   cfs[1] *   cfs[2]  * ((img->isInside(cor_ijk[0]  ,cor_ijk[1]-1,cor_ijk[2]-1)) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]  , cor_ijk[1]-1,cor_ijk[2]-1)]) : img->outsideVal);
                out +=    cfs[0] *(1-cfs[1])*   cfs[2]  * ((img->isInside(cor_ijk[0]-1,cor_ijk[1]  ,cor_ijk[2]-1)) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]  ,cor_ijk[2]-1)]) : img->outsideVal);
                out += (1-cfs[0])*(1-cfs[1])*   cfs[2]  * ((img->isInside(cor_ijk[0]  ,cor_ijk[1]  ,cor_ijk[2]-1)) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]  , cor_ijk[1]  ,cor_ijk[2]-1)]) : img->outsideVal);
                out +=    cfs[0] *   cfs[1] *(1-cfs[2]) * ((img->isInside(cor_ijk[0]-1,cor_ijk[1]-1,cor_ijk[2]  )) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]-1,cor_ijk[2]  )]) : img->outsideVal);
                out += (1-cfs[0])*   cfs[1] *(1-cfs[2]) * ((img->isInside(cor_ijk[0]  ,cor_ijk[1]-1,cor_ijk[2]  )) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]  , cor_ijk[1]-1,cor_ijk[2]  )]) : img->outsideVal);
                out +=    cfs[0] *(1-cfs[1])*(1-cfs[2]) * ((img->isInside(cor_ijk[0]-1,cor_ijk[1]  ,cor_ijk[2]  )) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]  ,cor_ijk[2]  )]) : img->outsideVal);
                out += (1-cfs[0])*(1-cfs[1])*(1-cfs[2]) * ((img->isInside(cor_ijk[0]  ,cor_ijk[1]  ,cor_ijk[2]  )) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]  , cor_ijk[1]  ,cor_ijk[2]  )]) : img->outsideVal);

                return out;
                
                }

                
            case INTERP_OUTSIDE:
                return img->outsideVal;

            default:
                return img->outsideVal;
            
        }

    }



    template<typename OUT_T, typename INP_T>
    OUT_T Interpolator<OUT_T,INP_T>::interp_linear_4D_att(NIBR::Image<INP_T>* img, OUT_T* p, int64_t t)
    {
        OUT_T     cfs[3];
        int64_t   cor_ijk[3];
        
        switch (init_interp_linear(img, p, cfs, cor_ijk)) {
        
            case INTERP_INSIDE: {

                OUT_T tmp =    cfs[0] *   cfs[1] *   cfs[2] *OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]-1, cor_ijk[2]-1, t)]) +
                            (1-cfs[0])*   cfs[1] *   cfs[2] *OUT_T(img->data[img->sub2ind(cor_ijk[0],   cor_ijk[1]-1, cor_ijk[2]-1, t)]) +
                               cfs[0] *(1-cfs[1])*   cfs[2] *OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1],   cor_ijk[2]-1, t)]) +
                            (1-cfs[0])*(1-cfs[1])*   cfs[2] *OUT_T(img->data[img->sub2ind(cor_ijk[0],   cor_ijk[1],   cor_ijk[2]-1, t)]) +
                               cfs[0] *   cfs[1] *(1-cfs[2])*OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]-1, cor_ijk[2],   t)]) +
                            (1-cfs[0])*   cfs[1] *(1-cfs[2])*OUT_T(img->data[img->sub2ind(cor_ijk[0],   cor_ijk[1]-1, cor_ijk[2],   t)]) +
                               cfs[0] *(1-cfs[1])*(1-cfs[2])*OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1],   cor_ijk[2],   t)]) +
                            (1-cfs[0])*(1-cfs[1])*(1-cfs[2])*OUT_T(img->data[img->sub2ind(cor_ijk[0],   cor_ijk[1],   cor_ijk[2],   t)]);

                return tmp;
            }
                
            case INTERP_BOUNDARY: {
                
                OUT_T tmp = 0;
                
                tmp +=    cfs[0] *   cfs[1] *   cfs[2]  * ((img->isInside(cor_ijk[0]-1,cor_ijk[1]-1,cor_ijk[2]-1)) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]-1,cor_ijk[2]-1, t)]) : img->outsideVal);
                tmp += (1-cfs[0])*   cfs[1] *   cfs[2]  * ((img->isInside(cor_ijk[0]  ,cor_ijk[1]-1,cor_ijk[2]-1)) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]  , cor_ijk[1]-1,cor_ijk[2]-1, t)]) : img->outsideVal);
                tmp +=    cfs[0] *(1-cfs[1])*   cfs[2]  * ((img->isInside(cor_ijk[0]-1,cor_ijk[1]  ,cor_ijk[2]-1)) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]  ,cor_ijk[2]-1, t)]) : img->outsideVal);
                tmp += (1-cfs[0])*(1-cfs[1])*   cfs[2]  * ((img->isInside(cor_ijk[0]  ,cor_ijk[1]  ,cor_ijk[2]-1)) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]  , cor_ijk[1]  ,cor_ijk[2]-1, t)]) : img->outsideVal);
                tmp +=    cfs[0] *   cfs[1] *(1-cfs[2]) * ((img->isInside(cor_ijk[0]-1,cor_ijk[1]-1,cor_ijk[2]  )) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]-1,cor_ijk[2],   t)]) : img->outsideVal);
                tmp += (1-cfs[0])*   cfs[1] *(1-cfs[2]) * ((img->isInside(cor_ijk[0]  ,cor_ijk[1]-1,cor_ijk[2]  )) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]  , cor_ijk[1]-1,cor_ijk[2],   t)]) : img->outsideVal);
                tmp +=    cfs[0] *(1-cfs[1])*(1-cfs[2]) * ((img->isInside(cor_ijk[0]-1,cor_ijk[1]  ,cor_ijk[2]  )) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]  ,cor_ijk[2],   t)]) : img->outsideVal);
                tmp += (1-cfs[0])*(1-cfs[1])*(1-cfs[2]) * ((img->isInside(cor_ijk[0]  ,cor_ijk[1]  ,cor_ijk[2]  )) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]  , cor_ijk[1]  ,cor_ijk[2],   t)]) : img->outsideVal);
                
                return tmp;
                
                }
            
            case INTERP_OUTSIDE:
                return img->outsideVal;

            default:
                return img->outsideVal;
            
        }
        
    }


    template<typename OUT_T, typename INP_T>
    void Interpolator<OUT_T,INP_T>::interp_linear_4D_all(NIBR::Image<INP_T>* img, OUT_T* p, OUT_T* out)
    {
        OUT_T     cfs[3];
        int64_t   cor_ijk[3];
        
        switch (init_interp_linear(img, p, cfs, cor_ijk)) {
        
            case INTERP_INSIDE:
                for (int64_t c=0; c<img->valCnt; c++) {
                    out[c] =   cfs[0] *   cfs[1] *   cfs[2] *OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]-1, cor_ijk[2]-1, c)]) +
                            (1-cfs[0])*   cfs[1] *   cfs[2] *OUT_T(img->data[img->sub2ind(cor_ijk[0],   cor_ijk[1]-1, cor_ijk[2]-1, c)]) +
                               cfs[0] *(1-cfs[1])*   cfs[2] *OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1],   cor_ijk[2]-1, c)]) +
                            (1-cfs[0])*(1-cfs[1])*   cfs[2] *OUT_T(img->data[img->sub2ind(cor_ijk[0],   cor_ijk[1],   cor_ijk[2]-1, c)]) +
                               cfs[0] *   cfs[1] *(1-cfs[2])*OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]-1, cor_ijk[2],   c)]) +
                            (1-cfs[0])*   cfs[1] *(1-cfs[2])*OUT_T(img->data[img->sub2ind(cor_ijk[0],   cor_ijk[1]-1, cor_ijk[2],   c)]) +
                               cfs[0] *(1-cfs[1])*(1-cfs[2])*OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1],   cor_ijk[2],   c)]) +
                            (1-cfs[0])*(1-cfs[1])*(1-cfs[2])*OUT_T(img->data[img->sub2ind(cor_ijk[0],   cor_ijk[1],   cor_ijk[2],   c)]);
                }
                break;
                
            case INTERP_BOUNDARY: {
                
                for (int64_t c=0; c<img->valCnt; c++) {
                
                    OUT_T tmp = 0;
                    
                    tmp +=    cfs[0] *   cfs[1] *   cfs[2]  * ((img->isInside(cor_ijk[0]-1,cor_ijk[1]-1,cor_ijk[2]-1)) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]-1,cor_ijk[2]-1, c)]) : img->outsideVal);
                    tmp += (1-cfs[0])*   cfs[1] *   cfs[2]  * ((img->isInside(cor_ijk[0]  ,cor_ijk[1]-1,cor_ijk[2]-1)) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]  , cor_ijk[1]-1,cor_ijk[2]-1, c)]) : img->outsideVal);
                    tmp +=    cfs[0] *(1-cfs[1])*   cfs[2]  * ((img->isInside(cor_ijk[0]-1,cor_ijk[1]  ,cor_ijk[2]-1)) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]  ,cor_ijk[2]-1, c)]) : img->outsideVal);
                    tmp += (1-cfs[0])*(1-cfs[1])*   cfs[2]  * ((img->isInside(cor_ijk[0]  ,cor_ijk[1]  ,cor_ijk[2]-1)) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]  , cor_ijk[1]  ,cor_ijk[2]-1, c)]) : img->outsideVal);
                    tmp +=    cfs[0] *   cfs[1] *(1-cfs[2]) * ((img->isInside(cor_ijk[0]-1,cor_ijk[1]-1,cor_ijk[2]  )) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]-1,cor_ijk[2],   c)]) : img->outsideVal);
                    tmp += (1-cfs[0])*   cfs[1] *(1-cfs[2]) * ((img->isInside(cor_ijk[0]  ,cor_ijk[1]-1,cor_ijk[2]  )) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]  , cor_ijk[1]-1,cor_ijk[2],   c)]) : img->outsideVal);
                    tmp +=    cfs[0] *(1-cfs[1])*(1-cfs[2]) * ((img->isInside(cor_ijk[0]-1,cor_ijk[1]  ,cor_ijk[2]  )) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]-1, cor_ijk[1]  ,cor_ijk[2],   c)]) : img->outsideVal);
                    tmp += (1-cfs[0])*(1-cfs[1])*(1-cfs[2]) * ((img->isInside(cor_ijk[0]  ,cor_ijk[1]  ,cor_ijk[2]  )) ?  OUT_T(img->data[img->sub2ind(cor_ijk[0]  , cor_ijk[1]  ,cor_ijk[2],   c)]) : img->outsideVal);
                    
                    out[c] = tmp;
                }
                
                break;
                
                }
            
            case INTERP_OUTSIDE: {
                memset(out,img->outsideVal,img->valCnt*sizeof(OUT_T));
                break;
            }

            default: {
                memset(out,img->outsideVal,img->valCnt*sizeof(OUT_T));
                break;
            }
            
        }

        return;
        
    }


    // NEAREST NEIGHBOR INTERPOLATION

    template<typename OUT_T, typename INP_T>
    INTERPAT Interpolator<OUT_T,INP_T>::init_interp_nearest(NIBR::Image<INP_T>* img, OUT_T *p, int64_t* cor_ijk) 
    {
        
        OUT_T ijk[3];
        img->to_ijk(p,ijk);
        
        cor_ijk[0]  = std::round(ijk[0]);
        cor_ijk[1]  = std::round(ijk[1]);
        cor_ijk[2]  = std::round(ijk[2]);

        if ( cor_ijk[0]>=0 && cor_ijk[1]>=0 && cor_ijk[2]>=0 && cor_ijk[0]<img->imgDims[0] && cor_ijk[1]<img->imgDims[1] && cor_ijk[2]<img->imgDims[2] )
            return INTERP_INSIDE;
        
        if ( cor_ijk[0]<0 || cor_ijk[1]<0 || cor_ijk[2]<0 || cor_ijk[0]>=img->imgDims[0] || cor_ijk[1]>=img->imgDims[1] || cor_ijk[2]>=img->imgDims[2] )
            return INTERP_OUTSIDE;

        return INTERP_BOUNDARY;
        
    }

    template<typename OUT_T, typename INP_T>
    OUT_T Interpolator<OUT_T,INP_T>::interp_nearest_3D(NIBR::Image<INP_T>* img, OUT_T* p)
    {
        
        int64_t   cor_ijk[3];

        if (init_interp_nearest(img, p, cor_ijk)==INTERP_OUTSIDE)
            return img->outsideVal;
        else
            return OUT_T(img->data[img->sub2ind(cor_ijk[0],cor_ijk[1],cor_ijk[2])]);

    }

    template<typename OUT_T, typename INP_T>
    OUT_T Interpolator<OUT_T,INP_T>::interp_nearest_4D_att(NIBR::Image<INP_T>* img, OUT_T* p, int64_t t)
    {

        int64_t   cor_ijk[3];

        if (init_interp_nearest(img, p, cor_ijk)==INTERP_OUTSIDE)
            return img->outsideVal;
        else
            return OUT_T(img->data[img->sub2ind(cor_ijk[0],cor_ijk[1],cor_ijk[2],t)]);
        
    }


    template<typename OUT_T, typename INP_T>
    void Interpolator<OUT_T,INP_T>::interp_nearest_4D_all(NIBR::Image<INP_T>* img, OUT_T* p, OUT_T* out)
    {

        int64_t   cor_ijk[3];

        if (init_interp_nearest(img, p, cor_ijk)==INTERP_OUTSIDE) {
            memset(out,img->outsideVal,img->valCnt*sizeof(OUT_T));
            return;
        } else {
            for (int64_t c=0; c<img->valCnt; c++)
                out[c] = OUT_T(img->data[img->sub2ind(cor_ijk[0],cor_ijk[1],cor_ijk[2],c)]);
            
            return;
        }
        
    }

}