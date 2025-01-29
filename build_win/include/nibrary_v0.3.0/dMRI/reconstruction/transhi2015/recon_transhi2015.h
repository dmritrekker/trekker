#pragma once

#include "base/nibr.h"
#include "image/image.h"
#include "surface/surface.h"
#include <fstream>
#include <sstream>
#include <tuple>
#include <numeric>

#define TRANSHI2015_SH_ORDER                8
#define TRANSHI2015_DELTASTEP               EPS7
#define TRANSHI2015_INIT_D_INAX             0.0017
#define TRANSHI2015_INIT_D_TRAPPED          0
#define TRANSHI2015_INIT_D_EXAX             0.001
#define TRANSHI2015_INIT_MINNUMCONSTRAINTS  100
#define TRANSHI2015_CONSTRAINT_UPDATE_COUNT 1
#define TRANSHI2015_FAST_OPTIMIZATION       true
#define TRANSHI2015_BVAL_LOW                10
#define TRANSHI2015_BVAL_HIGH               1000000000000
#define TRANSHI2015_MAXITER                 100
#define TRANSHI2015_XI_INIT                 0.04
#define TRANSHI2015_XI_STEP                 0.02
#define TRANSHI2015_XI_STEPCOUNT            100
#define TRANSHI2015_MAXCROSSINGS            4
#define TRANSHI2015_NOISEFLOOR              0

namespace NIBR
{

    typedef enum {
        OPTIM_END_UNSET      = 0,
        TOO_SMALL_GRADIENT   = 1,
        END_OF_ITERATION     = 2,
        TOO_SMALL_DISO       = 3
    } REASON_OF_TERMINATION;

    class TranShi2015 {

        public:

            TranShi2015(Image<float>& _dMRI,std::vector<std::vector<float>>& _grad);
            ~TranShi2015() {delete FOD; delete TM;}

            void recon();
            void print();

            // Necassary inputs
            Image<float>* dMRI{NULL};
            std::vector<std::vector<float>> gradTable;

            // Optional input
            Image<bool>* mask{NULL};
            void setMask(Image<bool>& _mask) {mask = &_mask;};

            // Reconstruction parameters
            int     shOrder{TRANSHI2015_SH_ORDER};
            double  deltaStep{TRANSHI2015_DELTASTEP};
            double  init_D_inAx{TRANSHI2015_INIT_D_INAX};
            double  init_D_trapped{TRANSHI2015_INIT_D_TRAPPED};
            double  init_D_exAx_iso{TRANSHI2015_INIT_D_EXAX};
            int     init_minNumConstraint{TRANSHI2015_INIT_MINNUMCONSTRAINTS};
            int     constraintUpdateCount{TRANSHI2015_CONSTRAINT_UPDATE_COUNT};
            bool    fastOptimization{TRANSHI2015_FAST_OPTIMIZATION};
            double  bValLow{TRANSHI2015_BVAL_LOW};
            double  bValHigh{TRANSHI2015_BVAL_HIGH};
            int     maxIter{TRANSHI2015_MAXITER};
            double  xi_init{TRANSHI2015_XI_INIT};
            double  xi_step{TRANSHI2015_XI_STEP};
            int     xi_stepCount{TRANSHI2015_XI_STEPCOUNT};
            int     maxCrossings{TRANSHI2015_MAXCROSSINGS};
            double  noiseFloor{TRANSHI2015_NOISEFLOOR};

            // Outputs
            Image<float>* FOD;
            Image<float>* TM;

            Image<float>& getFOD() {return *FOD;}
            Image<float>& getTM()  {return  *TM;}

        private:

            void prep();

            float checkConstraints(const Eigen::VectorXd &d);

            Eigen::VectorXd setConstraints(Eigen::MatrixXd &aH,Eigen::VectorXd &g,
                                           Eigen::MatrixXd  &C,Eigen::VectorXd &u, 
                                           int &minConstraintIndex,float& positivityRatio);
            
            void updateConstraints(Eigen::MatrixXd &aH,Eigen::VectorXd &g,
                                   Eigen::MatrixXd  &C,Eigen::VectorXd &u, 
                                   Eigen::VectorXd d, int &minConstraintIndex, float& positivityRatio);

            REASON_OF_TERMINATION optimizeDiso(Eigen::MatrixXd &aH,  Eigen::VectorXd &g,
                                         Eigen::MatrixXd &C,   Eigen::VectorXd &u, 
                                         Eigen::VectorXd &s_NF, 
                                         Eigen::VectorXd &d, 
                                         Eigen::MatrixXd &loc_YlmG,
                                         double &D_exAx_iso, 
                                         const double xi,
                                         int &iterCnt);

            std::vector<float> getPeakValues(const Eigen::VectorXd& y, double peakThresh);
            float              getTargetPeakCountAndThresh(const std::vector<float>& peakValues, int &targetPeakCount);

            std::vector<int>    voxelIndicesToReconstruct;
            size_t              init_minConstraintIndex;

            std::vector<size_t> B0_ind;                         // Indices where Bvalue = 0
            std::vector<size_t> DTI_ind;                        // Indices where Bvalue is in the accepted threshold
            Eigen::VectorXd     bval;                           // Bvalues from DTI_ind indices

            Eigen::VectorXd     I;
            Eigen::MatrixXd     Y;              // Used to check peaks, which uses a sparser sphere
            Eigen::MatrixXd     Y_constraint;   // Used to set constraints, which using a dense sphere
            Eigen::MatrixXd     init_A_plus;
            Eigen::MatrixXd     qp_A;
            Eigen::VectorXd     qp_b;

            Surface             sphere;

    };

    void recon_transhi2015(Image<float>& FOD, Image<float>& TM, 
                             Image<float>& dMRI,
                             std::vector<std::vector<float>> grad, 
                             Image<bool>& mask, 
                             int     shOrder,
                             double  deltaStep,
                             double  init_D_inAx,
                             double  init_D_trapped,
                             double  init_D_exAx_iso,
                             int     init_minNumConstraint,
                             int     constraintUpdateCount,
                             bool    fastOptimization,
                             double  bValLow,
                             double  bValHigh,
                             int     maxIter, 
                             double  xi_init, 
                             double  xi_step, 
                             int     xi_stepCount,
                             int     maxCrossings,
                             double  noiseFloor);

    std::vector<std::vector<double>> G_mat(std::vector<float> bval, double D_inAx, double D_trapped, int maxOrder);
    
}