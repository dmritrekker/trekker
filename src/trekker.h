#include <string>
#include <vector>

class Trekker {

public:

	Trekker(int argc, char **argv);
	Trekker(std::string pathToFODimage);
	~Trekker();

	void execute();
    void threadedExecute();
    
	std::vector< std::vector< std::vector<double> > > get_tractogram_coordinates();

	std::vector< std::vector< std::vector<double> > > run();

	bool timeUp;
    
    // Global config 
	void numberOfThreads(int n);
    void timeLimit(int t);
    
    void printParameters();
    
    // Tracker config
    void stepSize(double _stepSize);
    void minRadiusOfCurvature(double x);
    void minFODamp(double _minFODamp);
    void ignoreWeakLinks(double x);
    void maxEstInterval(int n);
    void dataSupportExponent(double x);
    void minLength(double x);
	void maxLength(double x);
    void atMaxLength(std::string aml);
    void writeInterval(int n);
    void directionality(std::string d);
    void maxSamplingPerStep(int n);
    void initMaxEstTrials(int n);
    void propMaxEstTrials(int n);
    void useBestAtInit(bool q);

	void probeLength(double x);
	void probeRadius(double x);
	void probeCount(int n);
	void probeQuality(int n);

    
    // Seed config
    void seed_image(std::string pathToSeedImage);
    void seed_image_using_label(std::string pathToSeedImage, int label);
    void seed_coordinates(std::vector< std::vector<double> > seed_coordinates);
    void seed_coordinates_with_directions(std::vector< std::vector<double> > seed_coordinates,std::vector< std::vector<double> > seed_init_directions);
    void seed_count(int n);
    void seed_countPerVoxel(int n);
    void seed_maxTrials(int n);
    
};
