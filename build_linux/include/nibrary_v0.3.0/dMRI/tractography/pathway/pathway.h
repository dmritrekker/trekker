#pragma once

#include "base/nibr.h"
#include "pathwayRule.h"
#include "../../../utility/seeder/seedingMethods.h"
#include "math/core.h"
#include "math/sphere.h"
#include "image/image_operators.h"
#include "surface/surface_operators.h"
#include "surface/surface2imageMapper.h"
#include "surface/findSegmentTriangleIntersection.h"

#define PVF_THRESH 0.9

namespace NIBR
{

    typedef enum {
        CONTINUE,
        STOP,
        DISCARD,
        FAIL,
        KEEP
    } WalkerAction;

    typedef enum {
        REQUIREMENT_ORDER_NOTSET = -1,
        NO_ORDER,
        IN_ORDER
    } RequirementOrder;

    typedef enum {
        DISCARDINGREASON_NOTSET,
        TOO_SHORT,
        TOO_LONG,
        DISCARD_REGION_REACHED,
        REQUIRED_ROI_NOT_MET,
        REQUIRED_ORDER_NOT_MET,
        ENDED_INSIDE_DISCARD_ROI,
        CANT_MEET_STOP_CONDITION,
        REACHED_TIME_LIMIT,
        SEED_NOT_FOUND,             // A seed was not found
        DISCARD_SEED,               // Seed was in discard seed region
        IMPROPER_SEED               // A seed was found but it is also within a stop region
    } DiscardingReason;

    typedef enum {
        FAILREASON_NOTSET,
        FAILED_BY_THE_ALGORITHM_AT_INITIALIZATION,
        FAILED_BY_THE_ALGORITHM
    } FailingReason;

    typedef enum {
        SUCCESSREASON_NOTSET,
        REACHED_MAXLENGTH_LIMIT,
        REACHED_MINDATASUPPORT_LIMIT,
        SATISFIED_PATHWAY_RULES
    } SuccessReason;

    typedef enum {
        TERMINATIONREASON_NOTSET,
        SEED_POINT,
        MIN_DATASUPPORT_REACHED,
        MAX_LENGTH_REACHED,
        STOP_ROI_REACHED
    } TerminationReason;

    typedef enum {
        ONE_SIDED,
        TWO_SIDED
    } Directionality;

    typedef enum {
        ATMAXLENGTH_STOP,
        ATMAXLENGTH_DISCARD
    } AtMaxLength;

    struct Walker {
        std::vector<Point>*        streamline;
        int                        ind;
        Tracking_Side              side;
        int                        sideAorder;
        int                        sideBorder;
        WalkerAction               action;
        int                        seedInd;
        std::vector<float>         seedRange;
        bool                       seedInserted;
        float                      begInd;
        float                      endInd;
        LineSegment                segment;
        float                      segStopLength;
        float                      trackedLength;
        
        TerminationReason          terminationReasonSideA;
        TerminationReason          terminationReasonSideB;
        DiscardingReason           discardingReason;
        FailingReason              failingReason;
        SuccessReason              successReason;
        
        std::vector<Entry_Status>  entry_status;
        std::vector<bool>          isDone;
    };

    class Pathway {
        
    public:
        Pathway();
        ~Pathway();

        // Set rules
        bool    add(PathwayRule prule);
        bool    remove(int ruleInd);
        
        bool    setMinLength(float l) {minLength = l;  isVerified = false; return verify();}
        bool    setMaxLength(float l) {maxLength = l;  isVerified = false; return verify();}
        bool    stopAtMax(bool p)     {atMaxLength = p ? ATMAXLENGTH_STOP : ATMAXLENGTH_DISCARD;  isVerified = false; return verify();}
        bool    oneSided(bool p)      {directionality = p ? ONE_SIDED : TWO_SIDED;                isVerified = false; return verify();}
        bool    inOrder(bool p)       {satisfy_requirements_in_order = p ? IN_ORDER : NO_ORDER;   isVerified = false; return verify();}
        bool    skipSeed(bool p)      {skipSeedROI = p;                                           isVerified = false; return verify();}
        // bool    noEdgeSeed(bool p)    {noEdgeSeeds = p;                                           isVerified = false; return verify();}
        bool    setSeedTrials(int p)  {seedTrials = p;                                            isVerified = false; return verify();}
          
        
        // Rules
        std::vector<PathwayRule>    prules;
        float                       minLength;
        float                       maxLength;
        AtMaxLength                 atMaxLength;
        Directionality              directionality;
        RequirementOrder            satisfy_requirements_in_order;
        bool                        skipSeedROI;
        // bool                        noEdgeSeeds;
        int                         seedTrials;
        float                       pvfThresh;

        // Pathway functions
        bool    verify();
        void    enableTracking(bool p)     {isTracking = p;}
        bool    isverified()               {return isVerified;  }
        bool    hasStop()                  {return stopFlag;    }
        bool    hasSeed()                  {return (seedRuleNo != -1) ? true : false; }
        Seeder* getSeeder(int ruleInd)     {return seeds[ruleInd];}
        Seeder* getSeeder()                {if (hasSeed()) return seeds[seedRuleNo]; else return NULL;} 
        int     getSeedRuleInd()           {return seedRuleNo;}
        void    print();

        // Pathway checking functions
        Walker*                     createWalker(std::vector<Point>* streamline);
        Walker*                     createWalker(std::vector<Point>* streamline, int ind);
        void                        seedlessProcess(Walker* walker);
        void                        seededProcess(Walker* walker);
        WalkerAction                checkSeed(Walker *w);                           // For seeded, two_sided case
        WalkerAction                checkSeed(Walker *walker, Tracking_Side side);  // For other seeded, two_sided case
        WalkerAction                checkWalker(Walker* walker, int b, int e);            // b: segment begin index. e: segment end index
        WalkerAction                checkWalker(Walker* walker);                          // checks the last segment
        void                        walkFirstEnd(Walker* walker);
        void                        walkSecondEnd(Walker* walker);
        Walker*                     tieFirstEnd(Walker* walker);
        Walker*                     tieSecondEnd(Walker* walker);
        Walker*                     tieEnd(Walker* walker) {return tieSecondEnd(walker);}
        Walker*                     tieRequireRules(float* p, Walker* walker);
        Walker*                     tieDiscardRules(float* p, Walker* walker);
        void                        flipSide(Walker* walker);
        void                        softReset(Walker* walker);
        void                        printDiscardingReason(Walker* walker);
        void                        printWalker(Walker* walker);

        bool                        checkEndsInside(float* p, Walker* walker);
        bool                        skipSeed(Walker* walker, bool reverseDir);

private:

        bool                        addSurface(PathwayRule prule);

        void                        prepSegment(Walker* walker);
        bool                        getSeedInd(Walker* walker);

        bool                        isPointAtEdgeOfRule(float*  p, int ruleNo, float distThresh);
        bool                        isPointInsideRule(float*  p, int ruleNo);
        std::tuple<bool,float>      isSegmentEntering(const LineSegment& segment, int ruleNo);
        std::tuple<bool,float>      isSegmentExiting (const LineSegment& segment, int ruleNo);
        bool                        setEntryStatus(Walker* w, int ruleNo);    // Check and set entry status. Discard if stop conditions can't be satisfied.

        bool                        isTracking;
        

        // Used internally. Set by pathwayAdd and removed by pathwayRemove

        int ruleCnt;        // Number of active rules
        int uniqueIdCnt;    // Increases when a new rule is added. Never decreases.
        std::vector<Pathway_Src>                                              srcType;
        std::vector<NIBR::Seeder*>                                            seeds;
        std::vector<NIBR::Image<int8_t>*>                                     img_mask;
        std::vector<NIBR::Image<int>*>                                        img_label;
        std::vector<int>                                                      img_label_val;
        std::vector<NIBR::Image<float>*>                                      img_pvf;
        std::vector<int>                                                      pvf_vol;
        std::vector<float>                                                    maxSegSizeScaler;
        std::vector<NIBR::Surface*>                                           surf;
        std::vector<NIBR::SurfaceField*>                                      surfData;
        std::vector<float*>                                                   sphCenter;
        std::vector<float>                                                    sphRadius;
        std::vector<float>                                                    sphRadiusSquared;
        std::vector<std::vector<Point>*>                                      pntLists;
        std::vector<std::vector<Point>*>                                      dirLists;
        std::vector<std::vector<std::vector<float>>*>                         data;


        // Every time verify() is called, the following variables are reinitialized using "prules" and nothing else.      
        bool                        isVerified;
        bool                        stopFlag;
        int                         seedRuleNo{-1}; // If pathwaySeed has only one seed, seedRuleNo is the index of that rule. Otherwise it is -1
        std::vector<int>            pathwaySeed;
        std::vector<int>            order_of_prules;
        std::vector<int>            order_of_side_A_prules;
        std::vector<int>            order_of_side_B_prules;

        // This is an internally managed variable
        bool B_pulled;

    };

}
