#ifndef SRC_INPUTPARSER_H_
#define SRC_INPUTPARSER_H_

#include <sstream>

#include "../config/config_general.h"
#include "../config/config_tracker.h"
#include "../config/config_seeding.h"
#include "../config/config_pathway.h"
#include "../config/config_output.h"

using namespace GENERAL;
using namespace TRACKER;
using namespace SEED;
using namespace PATHWAY;
using namespace OUTPUT;

class InputParser {

public:
	InputParser(int _argc, char **_argv);
	~InputParser();

private:

	int            argc;
	char**         argv;
	int            argv_index;

	bool Option(std::string option);
	void parse();
	void printConfig();
	void checkCompulsaryInputs();
	void setDefaultParametersWhenNecessary();
	void readAllImageInputs();

	void printHelp();

	// General config
	void parse_help();
	void parse_verboseLevel();
	void parse_numberOfThreads();
	void parse_usingAPI();
	void parse_timeLimit();

	// Tracker config
	void parse_fod();
    void parse_orderOfDirections();
	void parse_algorithm();
	void parse_stepSize();
	void parse_minRadiusOfCurvature();
	void parse_minFODamp();
    void parse_maxEstInterval();
    void parse_dataSupportExponent();
	void parse_minLength();
	void parse_maxLength();
	void parse_atMaxLength();
	void parse_writeInterval();
	void parse_directionality();
	void parse_maxSamplingPerStep();
	void parse_initMaxEstTrials();
	void parse_propMaxEstTrials();
	void parse_useBestAtInit();
    
	void parse_probeLength();
	void parse_probeRadius();
	void parse_probeCount();
	void parse_probeQuality();


	// Seed config
	void parse_seed_image();
	void parse_seed_coordinates();
	void parse_seed_count();
	void parse_seed_countPerVoxel();
	void parse_seed_maxTrials();

	// Pathway config
	void parse_pathway_require_entry();
	void parse_pathwayA_require_entry();
	void parse_pathwayB_require_entry();

	void parse_pathway_require_exit();
	void parse_pathwayA_require_exit();
	void parse_pathwayB_require_exit();

	void parse_pathway_stop_at_entry();
	void parse_pathwayA_stop_at_entry();
	void parse_pathwayB_stop_at_entry();

	void parse_pathway_stop_at_exit();
	void parse_pathwayA_stop_at_exit();
	void parse_pathwayB_stop_at_exit();

	void parse_pathway_discard_if_enters();
	void parse_pathwayA_discard_if_enters();
	void parse_pathwayB_discard_if_enters();

	void parse_pathway_discard_if_exits();
	void parse_pathwayA_discard_if_exits();
	void parse_pathwayB_discard_if_exits();

	void parse_pathway_discard_if_ends_inside();
	void parse_pathwayA_discard_if_ends_inside();
	void parse_pathwayB_discard_if_ends_inside();

	void parse_pathway_satisfy_requirements_in_order();

	// Output config
	void parse_output();
	void parse_enableOutputOverwrite();
	void parse_writeSeedCoordinates();
	void parse_writeColors();
	void parse_writeTangents();
	void parse_writek1axes();
	void parse_writek2axes();
	void parse_writek1s();
	void parse_writek2s();
	void parse_writeCurvatures();
	void parse_writePriors();
	void parse_writeLikelihoods();
	void parse_writePosteriors();

};



#endif
