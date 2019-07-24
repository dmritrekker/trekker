#include <string>
#include <vector>

class Trekker {

public:

	Trekker(int argc, char **argv);
	Trekker(std::string pathToFODimage);
	~Trekker();

	void set_seed_maxTrials(int n);
	void set_numberOfThreads(int n);
	void set_seeds(std::vector< std::vector<double> > seed_coordinates);


	void execute();
	std::vector< std::vector< std::vector<double> > > get_tractogram_coordinates();

	std::vector< std::vector< std::vector<double> > > run();

	bool timeUp;

};
