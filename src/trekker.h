#include <string>
#include <vector>

class Trekker {

public:

	Trekker(int argc, char **argv);
	Trekker(std::string pathToFODimage);
	~Trekker();

	void set_seeds(std::vector< std::vector<double> > seed_coordinates);

	void execute();
    void threadedExecute();
    
	std::vector< std::vector< std::vector<double> > > get_tractogram_coordinates();

	std::vector< std::vector< std::vector<double> > > run();

	bool timeUp;


	void set_seed_maxTrials(int n);
	void set_numberOfThreads(int n);
	void set_algorithm();
    void set_timeLimit(int t);

	void set_minFODamp(double _minFODamp);
	void set_stepSize(double _stepSize);

	void set_minRadiusOfCurvature(double x);
	void set_minLength(double x);
	void set_maxLength(double x);
//	void set_atMaxLength();
	void set_writeInterval(int n);
//	void set_directionality();
//	void set_maxSamplingPerStep(int n);
//	void set_initMaxEstTrials(int n);
//	void set_propMaxEstTrials(int n);
//	void set_useBestAtInit();
//
	void set_probeLength(double x);
	void set_probeRadius(double x);
	void set_probeCount(int n);
	void set_probeQuality(int n);

};
