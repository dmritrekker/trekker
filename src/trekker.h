#include "config/config_general.h"
#include "config/config_seeding.h"
#include "config/config_tracker.h"
#include <string>
#include <vector>

bool isInitialized();

class Trekker {

public:
    
    // Contructors
    Trekker(std::string pathToFODimage);
    Trekker(std::string pathToFODimage, bool discretizationFlag);
    Trekker(std::string pathToFODimage, std::string orderOfDirections);
    Trekker(std::string pathToFODimage, std::string orderOfDirections, bool discretizationFlag);
    Trekker(std::string pathToFODimage, bool discretizationFlag, std::string orderOfDirections);
    
    Trekker(std::string pathToFODimage, std::string pathToSphericalDomain, std::string symasym);
    Trekker(std::string pathToFODimage, std::string pathToSphericalDomain, std::string symasym, bool discretizationFlag);
    Trekker(std::string pathToFODimage, std::string pathToSphericalDomain, std::string symasym, std::string orderOfDirections);
    Trekker(std::string pathToFODimage, std::string pathToSphericalDomain, std::string symasym, std::string orderOfDirections, bool discretizationFlag);
    Trekker(std::string pathToFODimage, std::string pathToSphericalDomain, std::string symasym, bool discretizationFlag, std::string orderOfDirections);
    
    // Descructor
    ~Trekker();
    
    // Print current parameters
    void printParameters();
    
    // Run trekker
    std::vector< std::vector< std::vector<double> > > run();
    

    // PARAMETERS CAN BE SET USING THE FOLLOWING FUNCTIONS
    
    // Global config 
    void numberOfThreads(int n);
    void timeLimit(int t);    
    void resetParameters();

    int  getNumberOfThreads() {return GENERAL::numberOfThreads;}
    int  getTimeLimit() {return GENERAL::timeLimit;}

    
    // Tracker config
    void orderOfDirections(std::string ood);
    void stepSize(double _stepSize);
    void minRadiusOfCurvature(double x);
    void minFODamp(double _minFODamp);
    void dataSupportExponent(double x);
    void minLength(double x);
    void maxLength(double x);
    void atMaxLength(std::string aml);
    void writeInterval(int n);
    void directionality(std::string d);
    void maxEstInterval(int n);
    void initMaxEstTrials(int n);
    void propMaxEstTrials(int n);
    void maxSamplingPerStep(int n);
    void useBestAtInit(bool q);

    void probeLength(double x);
    void probeRadius(double x);
    void probeCount(int n);
    void probeQuality(int n);
    void ignoreWeakLinks(double x);

    std::string getOrderOfDirections() {return TRACKER::orderOfDirectionsTextInput;}
    float       getStepSize() {return TRACKER::stepSize;}
    float       getMinRadiusOfCurvature() {return TRACKER::minRadiusOfCurvature;}
    float       getMinFODamp() {return TRACKER::minFODamp;}
    float       getDataSupportExponent() {return TRACKER::dataSupportExponent;}
    float       getMinLength() {return TRACKER::minLength;}
    float       getMaxLength() {return TRACKER::maxLength;}
    std::string getAtMaxLength() {return (TRACKER::atMaxLength==ATMAXLENGTH_DISCARD)?"discard":"stop";}
    int         getWriteInterval() {return TRACKER::writeInterval;}
    std::string getDirectionality() {return (TRACKER::directionality==TWO_SIDED)?"two_sided":"one_sided";}
    int         getMaxEstInterval() {return TRACKER::maxEstInterval;}
    int         getInitMaxEstTrials() {return TRACKER::initMaxEstTrials;}
    int         getPropMaxEstTrials() {return TRACKER::propMaxEstTrials;}
    int         getMaxSamplingPerStep() {return TRACKER::triesPerRejectionSampling;}
    bool        getUseBestAtInit() {return (TRACKER::atInit==ATINIT_USEBEST)?true:false;}

    float       getProbeLength() {return TRACKER::probeLength;}
    float       getProbeRadius() {return TRACKER::probeRadius;}
    int         getProbeCount() {return TRACKER::probeCount;}
    int         getProbeQuality() {return TRACKER::probeQuality;}
    float       getIgnoreWeakLinks() {return TRACKER::weakLinkThresh;}

    
    
    // Seed config
    void seed_image(std::string pathToSeedImage);
    void seed_image(std::string pathToSeedImage, int label);
    
    void seed_coordinates(std::vector< std::vector<double> > seed_coordinates);
    void seed_coordinates_with_directions(std::vector< std::vector<double> > seed_coordinates,std::vector< std::vector<double> > seed_init_directions);
    void seed_count(int n);
    void seed_countPerVoxel(int n);
    void seed_maxTrials(int n);

    std::string getSeed_image() {return SEED::img_SEED->getFilePath();}
    int         getSeed_count() {return SEED::count;}
    int         getSeed_countPerVoxle() {return SEED::countPerVoxel;}
    int         getSeed_maxTrials() {return SEED::maxTrialsPerSeed;}
    
    
    
    // Pathway config
    void clearPathwayRules();
    
    void pathway_require_entry(std::string s);
    void pathway_A_require_entry(std::string s);
    void pathway_B_require_entry(std::string s);
    
    void pathway_require_exit(std::string s);
    void pathway_A_require_exit(std::string s);
    void pathway_B_require_exit(std::string s);
    
    void pathway_stop_at_entry(std::string s);
    void pathway_A_stop_at_entry(std::string s);
    void pathway_B_stop_at_entry(std::string s);
    
    void pathway_stop_at_exit(std::string s);
    void pathway_A_stop_at_exit(std::string s);
    void pathway_B_stop_at_exit(std::string s);
    
    void pathway_discard_if_enters(std::string s);
    void pathway_A_discard_if_enters(std::string s);
    void pathway_B_discard_if_enters(std::string s);
    
    void pathway_discard_if_exits(std::string s);
    void pathway_A_discard_if_exits(std::string s);
    void pathway_B_discard_if_exits(std::string s);
    
    void pathway_discard_if_ends_inside(std::string s);
    void pathway_A_discard_if_ends_inside(std::string s);
    void pathway_B_discard_if_ends_inside(std::string s);
    
    
    void pathway_require_entry(std::string s, int l);
    void pathway_A_require_entry(std::string s, int l);
    void pathway_B_require_entry(std::string s, int l);
    
    void pathway_require_exit(std::string s, int l);
    void pathway_A_require_exit(std::string s, int l);
    void pathway_B_require_exit(std::string s, int l);
    
    void pathway_stop_at_entry(std::string s, int l);
    void pathway_A_stop_at_entry(std::string s, int l);
    void pathway_B_stop_at_entry(std::string s, int l);
    
    void pathway_stop_at_exit(std::string s, int l);
    void pathway_A_stop_at_exit(std::string s, int l);
    void pathway_B_stop_at_exit(std::string s, int l);
    
    void pathway_discard_if_enters(std::string s, int l);
    void pathway_A_discard_if_enters(std::string s, int l);
    void pathway_B_discard_if_enters(std::string s, int l);
    
    void pathway_discard_if_exits(std::string s, int l);
    void pathway_A_discard_if_exits(std::string s, int l);
    void pathway_B_discard_if_exits(std::string s, int l);
    
    void pathway_discard_if_ends_inside(std::string s, int l);
    void pathway_A_discard_if_ends_inside(std::string s, int l);
    void pathway_B_discard_if_ends_inside(std::string s, int l);
    
    void pathway_satisy_requirements_in_order(bool q);
    

    // This is used to generate the trekker executable which is not a part of the API.
    Trekker(int argc, char **argv);
    
private:
    
    void execute();
    std::vector< std::vector< std::vector<double> > > get_tractogram_coordinates();
    bool timeUp;
    
};
