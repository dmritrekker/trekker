#include "inputParser.h"

InputParser::InputParser(int _argc, char** _argv) {

	time(&(GENERAL::startDate)); // Write start date

	argc            = _argc;
	argv            = _argv;
	argv_index 		= 1;

	parse();
	checkCompulsaryInputs();
	setDefaultParametersWhenNecessary();

	if (GENERAL::verboseLevel!=QUITE)
		printConfig();

	readAllImageInputs();

}

InputParser::~InputParser() {
	TRACKER::cleanConfigTracker();
	SEED::cleanConfigSeeding();
	PATHWAY::cleanConfigROI();
}

void InputParser::printHelp() {

#ifdef BUILD_FOR_WINDOWS
	std::cout << "Opening help in web browser" << std::endl;
#endif

#ifdef SHOWMANUAL
	system(SHOWMANUAL);
#endif
	exit(EXIT_SUCCESS);
}


bool InputParser::Option(std::string option) {
	return (option == std::string(argv[argv_index])) ? 1 : 0 ;
}

void InputParser::parse() {

	if (argc==1)
		printHelp();

	while (argv_index!=argc) {

		// Global config
		if      (Option("-help")) 				    parse_help();
		else if (Option("-timeLimit")) 				parse_timeLimit();
		else if (Option("-verboseLevel")) 			parse_verboseLevel();
		else if (Option("-numberOfThreads")) 		parse_numberOfThreads();
		else if (Option("-USING_API")) 				parse_usingAPI();

		// Tracker config
		else if (Option("-fod"))        			parse_fod();
        else if (Option("-dontDiscretizeFod"))      parse_dontDiscretizeFod();
        else if (Option("-orderOfDirections"))      parse_orderOfDirections();
		else if (Option("-algorithm"))     			parse_algorithm();
		else if (Option("-stepSize"))   			parse_stepSize();
		else if (Option("-minRadiusOfCurvature"))   parse_minRadiusOfCurvature();
		else if (Option("-minFODamp"))        		parse_minFODamp();
        else if (Option("-dataSupportExponent"))    parse_dataSupportExponent();
        else if (Option("-maxEstInterval"))        	parse_maxEstInterval();
		else if (Option("-minLength"))        		parse_minLength();
		else if (Option("-maxLength"))        		parse_maxLength();
		else if (Option("-atMaxLength"))        	parse_atMaxLength();
		else if (Option("-writeInterval"))     		parse_writeInterval();
		else if (Option("-directionality"))     	parse_directionality();
		else if (Option("-maxSamplingPerStep"))     parse_maxSamplingPerStep();
		else if (Option("-initMaxEstTrials"))       parse_initMaxEstTrials();
		else if (Option("-propMaxEstTrials"))       parse_propMaxEstTrials();
		else if (Option("-useBestAtInit"))       	parse_useBestAtInit();

        
		else if (Option("-probeLength"))        	parse_probeLength();
		else if (Option("-probeRadius"))        	parse_probeRadius();
		else if (Option("-probeCount"))     		parse_probeCount();
		else if (Option("-probeQuality"))     		parse_probeQuality();
        else if (Option("-ignoreWeakLinks"))        parse_ignoreWeakLinks();
        
        else if (Option("-dispersionImage"))        parse_dispersionImage();

		// Seed config
		else if (Option("-seed_image"))     			parse_seed_image();
		else if (Option("-seed_coordinates"))     		parse_seed_coordinates();
		else if (Option("-seed_count"))  				parse_seed_count();
		else if (Option("-seed_countPerVoxel"))  		parse_seed_countPerVoxel();
		else if (Option("-seed_maxTrials"))  			parse_seed_maxTrials();

		// ROI config
		else if (Option("-pathway=require_entry")) 				   parse_pathway_require_entry();
		else if (Option("-pathway_A=require_entry")) 			   parse_pathwayA_require_entry();
		else if (Option("-pathway_B=require_entry")) 			   parse_pathwayB_require_entry();

		else if (Option("-pathway=require_exit"))  				   parse_pathway_require_exit();
		else if (Option("-pathway_A=require_exit"))  			   parse_pathwayA_require_exit();
		else if (Option("-pathway_B=require_exit"))  			   parse_pathwayB_require_exit();

		else if (Option("-pathway=stop_at_entry")) 				   parse_pathway_stop_at_entry();
		else if (Option("-pathway_A=stop_at_entry")) 			   parse_pathwayA_stop_at_entry();
		else if (Option("-pathway_B=stop_at_entry")) 			   parse_pathwayB_stop_at_entry();

		else if (Option("-pathway=stop_at_exit")) 				   parse_pathway_stop_at_exit();
		else if (Option("-pathway_A=stop_at_exit")) 			   parse_pathwayA_stop_at_exit();
		else if (Option("-pathway_B=stop_at_exit")) 			   parse_pathwayB_stop_at_exit();

		else if (Option("-pathway=discard_if_enters")) 			   parse_pathway_discard_if_enters();
		else if (Option("-pathway_A=discard_if_enters")) 		   parse_pathwayA_discard_if_enters();
		else if (Option("-pathway_B=discard_if_enters")) 		   parse_pathwayB_discard_if_enters();

		else if (Option("-pathway=discard_if_exits"))  			   parse_pathway_discard_if_exits();
		else if (Option("-pathway_A=discard_if_exits"))  		   parse_pathwayA_discard_if_exits();
		else if (Option("-pathway_B=discard_if_exits"))  		   parse_pathwayB_discard_if_exits();

		else if (Option("-pathway=discard_if_ends_inside"))  	   parse_pathway_discard_if_ends_inside();
		else if (Option("-pathway_A=discard_if_ends_inside"))  	   parse_pathwayA_discard_if_ends_inside();
		else if (Option("-pathway_B=discard_if_ends_inside"))  	   parse_pathwayB_discard_if_ends_inside();

		else if (Option("-pathway=satisfy_requirements_in_order")) parse_pathway_satisfy_requirements_in_order();

		// Output config
		else if (Option("-output")) 				      parse_output();
		else if (Option("-enableOutputOverwrite")) 	      parse_enableOutputOverwrite();
		else if (Option("-writeSeedCoordinates")) 	      parse_writeSeedCoordinates();
        else if (Option("-dontWriteSegmentsInSeedROI"))   parse_dontWriteSegmentsInSeedROI();
		else if (Option("-writeColors")) 			      parse_writeColors();
        else if (Option("-writeFODamp")) 			      parse_writeFODamp();
		else if (Option("-writeTangents")) 			      parse_writeTangents();
		else if (Option("-writek1axes")) 			      parse_writek1axes();
		else if (Option("-writek2axes")) 			      parse_writek2axes();
		else if (Option("-writek1s")) 				      parse_writek1s();
		else if (Option("-writek2s")) 				      parse_writek2s();
		else if (Option("-writeCurvatures")) 		      parse_writeCurvatures();
		else if (Option("-writeLikelihoods")) 	          parse_writeLikelihoods();

		else {
			std::cout << "Unknown option: " << argv[argv_index] << std::endl;
			std::cout << "Use \"./trekker\" or \"./trekker -help\" for accepted options." << std::endl << std::endl;
			exit(EXIT_SUCCESS);
		}
	}

}


void InputParser::checkCompulsaryInputs() {

	if (GENERAL::usingAPI==false) {
		if (img_FOD->getFilePath()=="") {
			std::cout << "Use -fod <FOD_FNAME.NII.GZ> to specify the input FOD image" << std::endl;
			exit(EXIT_FAILURE);
		}

		if (SEED::seedingMode == SEED_NOTSET) {
			std::cout << "Use -seed_image or -seed_coordinates to specify seeds" << std::endl;
			exit(EXIT_FAILURE);
		}

		if (GENERAL::timeLimit<=0) {
			if   ( (SEED::seedingMode 			== 	SEED_IMAGE)
					&& (SEED::count       		==	NOTSET)
					&& (SEED::countPerVoxel		==	NOTSET) ) {
				std::cout << "Use -seed_count <NUMBER> or -seed_countPerVoxel <NUMBER> to specify the count of seeds " << std::endl;
				std::cout << "or use -timeLimit for the tracking duration" << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		if (OUTPUT::outputFilePath=="") {
			std::cout << "Use -output <OUTPUT_FILE_NAME.vtk> to specify output" << std::endl;
			exit(EXIT_FAILURE);
		}
		
		if (TRACKER::algorithm==PTT_WITH_PARAMETER_PRIORS) {
            if (img_dispersion->getFilePath()=="") {
                std::cout << "Use -dispersionImage <DISPERSION_FNAME.NII.GZ> to specify the input dispersion image" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
	}

	PATHWAY::checkROIorderConsistency();

}

void InputParser::setDefaultParametersWhenNecessary() {

	if (GENERAL::verboseLevel!=QUITE) {
		std::cout << std::endl;
		std::cout << "--------------------" << std::endl;
		std::cout << "Parsing input" << std::endl;
	}
	GENERAL::setDefaultParametersWhenNecessary();

	// Defaults for the seed image has to be set and
	// seed image/data has to be read before tracker
	// setup is made. This is necessary because if the
	// user defined -seed_countPerVoxel, the tracker
	// has to know how many streamlines this will
	// correspond to
	SEED::setDefaultParametersWhenNecessary();
	SEED::readSeedImage();

	TRACKER::setDefaultParametersWhenNecessary();
	OUTPUT::setDefaultParametersWhenNecessary();
	if (GENERAL::verboseLevel!=QUITE) {
		std::cout << "--------------------" << std::endl << std::endl;
	}
}

void InputParser::readAllImageInputs() {
	if (GENERAL::verboseLevel!=QUITE) std::cout << "--------------------" << std::endl;
	if (GENERAL::verboseLevel!=QUITE) std::cout << "Reading input images" << std::endl;
	TRACKER::readFODImage();
	PATHWAY::readROIImages();
    if (TRACKER::algorithm==PTT_WITH_PARAMETER_PRIORS) {
        TRACKER::readDispersionImage();
    }
	if (GENERAL::verboseLevel!=QUITE) std::cout << "--------------------" << std::endl << std::endl;
}

void InputParser::printConfig() {
	std::cout << "--------------------" << std::endl;
	GENERAL::print();
	TRACKER::print();
	SEED::print();
	PATHWAY::print();
	OUTPUT::print();
	std::cout << std::endl << "--------------------" << std::endl << std::endl;
}

void InputParser::parse_help() {
	printHelp();
}

void InputParser::parse_verboseLevel() {

	if (GENERAL::verboseLevel != VERBOSELEVEL_NOTSET) {
		std::cout << "Cannot use -verboseLevel option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input the detail of verbose reporting after -verboseLevel" << std::endl;
		std::cout << "-verboseLevel can only be 0, 1, 2, 3, 4" << std::endl;
		exit(EXIT_FAILURE);
	}
	GENERAL::verboseLevel = VerboseLevel(atoi(argv[argv_index]));

	if ((GENERAL::verboseLevel<0) || (GENERAL::verboseLevel>4)) {
		std::cout << "-verboseLevel can only be 0, 1, 2, 3, 4" << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

}

void InputParser::parse_numberOfThreads() {

	if (GENERAL::numberOfThreads != NOTSET) {
		std::cout << "Cannot use -numberOfThreads option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input the number of parallel threads after -numberOfThreads" << std::endl;
		std::cout << "-numberOfThreads has to be bigger than 0" << std::endl;
		exit(EXIT_FAILURE);
	}
	GENERAL::numberOfThreads = atoi(argv[argv_index]);

	if (GENERAL::numberOfThreads<0) {
		std::cout << "-numberOfThreads has to be bigger than 0" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (GENERAL::numberOfThreads>1000) {
		std::cout << "Maximum -numberOfThreads is 1000" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;

}

void InputParser::parse_usingAPI() {
	GENERAL::usingAPI=true;
	argv_index++;
}

void InputParser::parse_timeLimit() {

	if (GENERAL::timeLimit != NOTSET) {
		std::cout << "Cannot use -timeLimit option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input time limit in minutes after -timeLimit, the value must be positive" << std::endl;
		exit(EXIT_FAILURE);
	}
	GENERAL::timeLimit = atof(argv[argv_index])*60;

	argv_index++;

}

void InputParser::parse_output() {

	if (OUTPUT::outputFilePath != "") {
		std::cout << "Cannot use -output option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Write the output file name after -output" << std::endl;
		exit(EXIT_FAILURE);
	}

	OUTPUT::outputFilePath = std::string(argv[argv_index]);

	argv_index++;

}

void InputParser::parse_enableOutputOverwrite() {

	if (OUTPUT::overwriteMode != WRITE_NOTSET) {
		std::cout << "Cannot use -overwriteMode option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	OUTPUT::overwriteMode = WRITE_ON;
	argv_index++;

}

void InputParser::parse_writeSeedCoordinates() {
	if (OUTPUT::seedCoordinateWriteMode != WRITE_NOTSET) {
		std::cout << "Cannot use -writeSeedCoordinates option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}
	OUTPUT::seedCoordinateWriteMode = WRITE_ON;
	argv_index++;
}

void InputParser::parse_dontWriteSegmentsInSeedROI() {
	if (OUTPUT::dontWriteSegmentsInSeedROI != WRITE_NOTSET) {
		std::cout << "Cannot use -dontWriteSegmentsInSeedROI option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}
	OUTPUT::dontWriteSegmentsInSeedROI = WRITE_ON;
	argv_index++;
}

void InputParser::parse_writeColors() {
	if (OUTPUT::colorWriteMode != WRITE_NOTSET) {
		std::cout << "Cannot use -writeColors option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}
	OUTPUT::colorWriteMode = WRITE_ON;
	argv_index++;
}

void InputParser::parse_writeFODamp() {
	if (OUTPUT::FODampWriteMode != WRITE_NOTSET) {
		std::cout << "Cannot use -writeFODamp option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}
	OUTPUT::FODampWriteMode = WRITE_ON;
	argv_index++;
}

void InputParser::parse_writeTangents() {
	if (OUTPUT::tangentWriteMode != WRITE_NOTSET) {
		std::cout << "Cannot use -writeTangents option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}
	OUTPUT::tangentWriteMode = WRITE_ON;
	argv_index++;
}


void InputParser::parse_writek1axes() {
	if (OUTPUT::k1axisWriteMode != WRITE_NOTSET) {
		std::cout << "Cannot use -writek1axes option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}
	OUTPUT::k1axisWriteMode = WRITE_ON;
	argv_index++;
}

void InputParser::parse_writek2axes() {
	if (OUTPUT::k2axisWriteMode != WRITE_NOTSET) {
		std::cout << "Cannot use -writek2axes option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}
	OUTPUT::k2axisWriteMode = WRITE_ON;
	argv_index++;
}

void InputParser::parse_writek1s() {
	if (OUTPUT::k1WriteMode != WRITE_NOTSET) {
		std::cout << "Cannot use -writek1s option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}
	OUTPUT::k1WriteMode = WRITE_ON;
	argv_index++;
}

void InputParser::parse_writek2s() {
	if (OUTPUT::k2WriteMode != WRITE_NOTSET) {
		std::cout << "Cannot use -writek2s option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}
	OUTPUT::k2WriteMode = WRITE_ON;
	argv_index++;
}

void InputParser::parse_writeCurvatures() {
	if (OUTPUT::curvatureWriteMode != WRITE_NOTSET) {
		std::cout << "Cannot use -writeCurvatures option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}
	OUTPUT::curvatureWriteMode = WRITE_ON;
	argv_index++;
}

void InputParser::parse_writeLikelihoods() {
	if (OUTPUT::likelihoodWriteMode != WRITE_NOTSET) {
		std::cout << "Cannot use -writeLikelihoods option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}
	OUTPUT::likelihoodWriteMode = WRITE_ON;
	argv_index++;
}


void InputParser::parse_fod() {

	if (img_FOD->getFilePath()!="") {
		std::cout << "Cannot use -fod option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -fod" << std::endl;
		exit(EXIT_FAILURE);
	}

	if(!img_FOD->readHeader(argv[argv_index])) {
		std::cout << "Cannot read FOD image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;
    
    
    if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {

		std::string directions;
		std::ifstream sphere(argv[argv_index]);

		if (!sphere.good()) {
			std::cout << "Cannot read sphere vertices from " << argv[argv_index] << std::endl;
			exit(EXIT_FAILURE);
		}

		int lineNo = 1;
		bool readError = false;
        float p[3];
		while(std::getline(sphere,directions)) {
			if (!directions.empty()) {
				std::stringstream xyz(directions);
				if (xyz.good()) xyz >> p[0]; else { readError = true; break; }
				if (xyz.good()) xyz >> p[1]; else { readError = true; break; }
				if (xyz.good()) xyz >> p[2]; else { readError = true; break; }
				normalize(p);
				img_FOD->inpSphCoords.push_back(Coordinate(p[0],p[1],p[2]));
			}
			lineNo++;
		}
		sphere.close();
		if (readError) {
			std::cout << "Cannot read sphere vertices from " << argv[argv_index] << ", line " << lineNo << std::endl;
			exit(EXIT_FAILURE);
		}

		if ((img_FOD->nim->nt - img_FOD->inpSphCoords.size()) !=0 ) {
			std::cout << "Number of sphere vertices does not match the number of volumes in the FOD image" << std::endl;
			exit(EXIT_FAILURE);
		}
		
		img_FOD->isspheresliced = true;
		img_FOD->sphereFileName = std::string{argv[argv_index]};
		argv_index++;
        
        if      (Option("sym"))  img_FOD->iseven = true;
        else if (Option("asym")) img_FOD->iseven = false;
        else {
            std::cout << "Please indicate if FOD is symmetric or asymmetric using options \"sym\" or \"asym\" " << std::endl;
            std::cout << "  Example: -fod FODimg.nii.gz sphere.txt asym" << std::endl;
            exit(EXIT_FAILURE);
        }
    
        argv_index++;
		
	}
    
}

void InputParser::parse_dontDiscretizeFod() {

	if (TRACKER::fodDiscretization != FODDISC_NOTSET) {
		std::cout << "Cannot use -dontDiscretizeFod option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	TRACKER::fodDiscretization  = FODDISC_OFF;
    img_FOD->discretizationFlag = false;
	argv_index++;

}

void InputParser::parse_ignoreWeakLinks() {

	if (TRACKER::checkWeakLinks != CHECKWEAKLINKS_NOTSET) {
		std::cout << "Cannot use -ignoreWeakLinks option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	TRACKER::checkWeakLinks  = CHECKWEAKLINKS_ON;
	argv_index++;
    
    if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
        TRACKER::weakLinkThresh = (atof(argv[argv_index]));
		argv_index++;
	}
    
}


void InputParser::parse_dispersionImage() {

	if (img_dispersion->getFilePath()!="") {
		std::cout << "Cannot use -dispersionImage option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -dispersionImage" << std::endl;
		exit(EXIT_FAILURE);
	}

	if(!img_dispersion->readHeader(argv[argv_index])) {
		std::cout << "Cannot read dispersion image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;
    
}

void InputParser::parse_orderOfDirections() {

	if (TRACKER::orderOfDirections != ORDEROFDIRECTIONS_NOTSET) {
		std::cout << "Cannot use -orderOfDirections option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
    
    TRACKER::orderOfDirectionsTextInput=std::string(argv[argv_index]);
    
    if      (Option("XYZ")) TRACKER::orderOfDirections = XYZ;
    else if (Option("XYz")) TRACKER::orderOfDirections = XYz;
    else if (Option("XyZ")) TRACKER::orderOfDirections = XyZ;
    else if (Option("Xyz")) TRACKER::orderOfDirections = Xyz;
    else if (Option("xYZ")) TRACKER::orderOfDirections = xYZ;
    else if (Option("xYz")) TRACKER::orderOfDirections = xYz;
    else if (Option("xyZ")) TRACKER::orderOfDirections = xyZ;
    else if (Option("xyz")) TRACKER::orderOfDirections = xyz;
	else if (Option("XZY")) TRACKER::orderOfDirections = XZY;
    else if (Option("XZy")) TRACKER::orderOfDirections = XZy;
    else if (Option("XzY")) TRACKER::orderOfDirections = XzY;
    else if (Option("Xzy")) TRACKER::orderOfDirections = Xzy;
    else if (Option("xZY")) TRACKER::orderOfDirections = xZY;
    else if (Option("xZy")) TRACKER::orderOfDirections = xZy;
    else if (Option("xzY")) TRACKER::orderOfDirections = xzY;
    else if (Option("xzy")) TRACKER::orderOfDirections = xzy;
    else if (Option("YXZ")) TRACKER::orderOfDirections = YXZ;
    else if (Option("YXz")) TRACKER::orderOfDirections = YXz;
    else if (Option("YxZ")) TRACKER::orderOfDirections = YxZ;
    else if (Option("Yxz")) TRACKER::orderOfDirections = Yxz;
    else if (Option("yXZ")) TRACKER::orderOfDirections = yXZ;
    else if (Option("yXz")) TRACKER::orderOfDirections = yXz;
    else if (Option("yxZ")) TRACKER::orderOfDirections = yxZ;
    else if (Option("yxz")) TRACKER::orderOfDirections = yxz;
    else if (Option("YZX")) TRACKER::orderOfDirections = YZX;
    else if (Option("YZx")) TRACKER::orderOfDirections = YZx;
    else if (Option("YzX")) TRACKER::orderOfDirections = YzX;
    else if (Option("Yzx")) TRACKER::orderOfDirections = Yzx;
    else if (Option("yZX")) TRACKER::orderOfDirections = yZX;
    else if (Option("yZx")) TRACKER::orderOfDirections = yZx;
    else if (Option("yzX")) TRACKER::orderOfDirections = yzX;
    else if (Option("yzx")) TRACKER::orderOfDirections = yzx;
    else if (Option("ZYX")) TRACKER::orderOfDirections = ZYX;
    else if (Option("ZYx")) TRACKER::orderOfDirections = ZYx;
    else if (Option("ZyX")) TRACKER::orderOfDirections = ZyX;
    else if (Option("Zyx")) TRACKER::orderOfDirections = Zyx;
    else if (Option("zYX")) TRACKER::orderOfDirections = zYX;
    else if (Option("zYx")) TRACKER::orderOfDirections = zYx;
    else if (Option("zyX")) TRACKER::orderOfDirections = zyX;
    else if (Option("zyx")) TRACKER::orderOfDirections = zyx;
    else if (Option("ZXY")) TRACKER::orderOfDirections = ZXY;
    else if (Option("ZXy")) TRACKER::orderOfDirections = ZXy;
    else if (Option("ZxY")) TRACKER::orderOfDirections = ZxY;
    else if (Option("Zxy")) TRACKER::orderOfDirections = Zxy;
    else if (Option("zXY")) TRACKER::orderOfDirections = zXY;
    else if (Option("zXy")) TRACKER::orderOfDirections = zXy;
    else if (Option("zxY")) TRACKER::orderOfDirections = zxY;
    else if (Option("zxy")) TRACKER::orderOfDirections = zxy;
    else {
		std::cout << "Unknown order of directions: " << argv[argv_index] << ", valid options are e.g.\"xYz\", \"ZyX\" etc. "<< std::endl;
		exit(EXIT_FAILURE);
	}
    
	argv_index++;

}


void InputParser::parse_stepSize() {

	if (stepSize != NOTSET) {
		std::cout << "Cannot use -stepSize option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input step size after -stepSize" << std::endl;
		exit(EXIT_FAILURE);
	}
	stepSize	= atof(argv[argv_index]);
	argv_index++;

}

void InputParser::parse_minRadiusOfCurvature() {

	if (minRadiusOfCurvature != NOTSET) {
		std::cout << "Cannot use -minRadiusOfCurvature option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input maximum curvature after -minRadiusOfCurvature" << std::endl;
		exit(EXIT_FAILURE);
	}
	minRadiusOfCurvature = atof(argv[argv_index]);
	argv_index++;

}

void InputParser::parse_probeLength() {

	if (probeLength != NOTSET) {
		std::cout << "Cannot use -probeLength option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input length of probe after -probeLength" << std::endl;
		exit(EXIT_FAILURE);
	}
	probeLength 	= atof(argv[argv_index]);
	argv_index++;

}

void InputParser::parse_probeRadius() {

	if (probeRadius != NOTSET) {
		std::cout << "Cannot use -probeRadius option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input radius of probe after -probeRadius" << std::endl;
		exit(EXIT_FAILURE);
	}
	probeRadius 	= atof(argv[argv_index]);
	argv_index++;

}

void InputParser::parse_probeCount() {

	if (probeCount != NOTSET) {
		std::cout << "Cannot use -probeCount option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input number of probes after -probeCount, must be between 1 and 100" << std::endl;
		exit(EXIT_FAILURE);
	}
	probeCount = atoi(argv[argv_index]);

	if ((probeCount<1) || (probeCount>100)) {
		std::cout << "Input number of probes after -probeCount, must be between 1 and 100" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;

}

void InputParser::parse_probeQuality() {

	if (probeQuality != NOTSET) {
		std::cout << "Cannot use -probeQuality option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input quality level of the probe after -probeQuality, must be between 2 and 100" << std::endl;
		exit(EXIT_FAILURE);
	}
	probeQuality = atoi(argv[argv_index]);
	argv_index++;

}

void InputParser::parse_minFODamp() {

	if (minFODamp != NOTSET) {
		std::cout << "Cannot use -minFODamp option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input min FOD amplitude threshold for termination after -minFODamp" << std::endl;
		exit(EXIT_FAILURE);
	}
	minFODamp = atof(argv[argv_index]);
	argv_index++;

}

void InputParser::parse_maxEstInterval() {

	if (maxEstInterval != NOTSET) {
		std::cout << "Cannot use -maxEstInterval option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input how many steps after should the maximum posterior probability be updated after -maxEstInterval" << std::endl;
		exit(EXIT_FAILURE);
	}
	maxEstInterval = atoi(argv[argv_index]);
	argv_index++;

}

void InputParser::parse_dataSupportExponent() {

	if (dataSupportExponent != NOTSET) {
		std::cout << "Cannot use -dataSupportExponent option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input data support exponent after -dataSupportExponent" << std::endl;
		exit(EXIT_FAILURE);
	}
	dataSupportExponent = atof(argv[argv_index]);
	argv_index++;

}

void InputParser::parse_minLength() {

	if (minLength != NOTSET) {
		std::cout << "Cannot use -minLength option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input the minimum length of streamlines to reconstruct after -minLength" << std::endl;
		exit(EXIT_FAILURE);
	}
	minLength = atof(argv[argv_index]);
	argv_index++;

}

void InputParser::parse_maxLength() {

	if (maxLength != NOTSET) {
		std::cout << "Cannot use -maxLength option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input the maximum length of streamlines to reconstruct after -maxLength" << std::endl;
		exit(EXIT_FAILURE);
	}
	maxLength = atof(argv[argv_index]);
	argv_index++;

}

void InputParser::parse_atMaxLength() {

	if (TRACKER::atMaxLength != ATMAXLENGTH_NOTSET) {
		std::cout << "Cannot use -atMaxLength option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input tracking algorithm after -atMaxLength" << std::endl;
		exit(EXIT_FAILURE);
	}

	if      (Option("stop"))  		TRACKER::atMaxLength = ATMAXLENGTH_STOP;
	else if (Option("discard")) 	TRACKER::atMaxLength = ATMAXLENGTH_DISCARD;
	else {
		std::cout << "Unknown option: " << argv[argv_index] << ", -atMaxLength can be \"stop\" or \"discard\" " << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

}



void InputParser::parse_writeInterval() {

	if (writeInterval != NOTSET) {
		std::cout << "Cannot use -writeInterval option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input the writing interval after -writeInterval" << std::endl;
		exit(EXIT_FAILURE);
	}
	writeInterval = atoi(argv[argv_index]);
	argv_index++;

}

void InputParser::parse_directionality() {

	if (directionality != DIRECTIONALITY_NOTSET) {
		std::cout << "Cannot use -directionality option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input tracking directionality after -directionality" << std::endl;
		exit(EXIT_FAILURE);
	}

	if      (Option("one_sided"))              directionality = ONE_SIDED;
	else if (Option("two_sided"))              directionality = TWO_SIDED;
	else {
		std::cout << "Unknown directionality: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

}

void InputParser::parse_maxSamplingPerStep () {
	if (TRACKER::triesPerRejectionSampling != NOTSET) {
		std::cout << "Cannot use -maxSamplingPerStep option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input maximum number of samples to test at each step after -maxSamplingPerStep" << std::endl;
		exit(EXIT_FAILURE);
	}
	TRACKER::triesPerRejectionSampling = atoi(argv[argv_index]);

	if (TRACKER::triesPerRejectionSampling < 1) {
		std::cout << "Minimum number of samples to test at each step count is 1" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (TRACKER::triesPerRejectionSampling > 1e9) {
		std::cout << "Maximum number of maximum number of samples to test at each step cannot exceed 1e9" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
}

void InputParser::parse_initMaxEstTrials() {
	if (TRACKER::initMaxEstTrials != NOTSET) {
		std::cout << "Cannot use -initMaxEstTrials option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input number of trials to estimate maximum for initialization after -initMaxEstTrials" << std::endl;
		exit(EXIT_FAILURE);
	}
	TRACKER::initMaxEstTrials = atoi(argv[argv_index]);

	if (TRACKER::initMaxEstTrials < 1) {
		std::cout << "Minimum value is 1" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (TRACKER::initMaxEstTrials > 1e9) {
		std::cout << "Maximum value cannot exceed 1e9" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
}

void InputParser::parse_propMaxEstTrials() {
	if (TRACKER::propMaxEstTrials != NOTSET) {
		std::cout << "Cannot use -propMaxEstTrials option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input number of trials to estimate maximum for propagation after -propMaxEstTrials" << std::endl;
		exit(EXIT_FAILURE);
	}
	TRACKER::propMaxEstTrials = atoi(argv[argv_index]);

	if (TRACKER::propMaxEstTrials < 1) {
		std::cout << "Minimum value is 1" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (TRACKER::propMaxEstTrials > 1e9) {
		std::cout << "Maximum value cannot exceed 1e9" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
}

void InputParser::parse_useBestAtInit() {

	if (TRACKER::atInit != ATINIT_NOTSET) {
		std::cout << "Cannot use -useBestAtInit option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	TRACKER::atInit = ATINIT_USEBEST;
	argv_index++;

}

void InputParser::parse_algorithm() {

	if (TRACKER::algorithm != ALGORITHM_NOTSET) {
		std::cout << "Cannot use -algorithm option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;
    
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input tracking algorithm after -algorithm" << std::endl;
		exit(EXIT_FAILURE);
	}

	if      (Option("ptt"))    					     TRACKER::algorithm = PTT_C1;
	else if (Option("local_probabilistic"))  	     TRACKER::algorithm = LOCAL_PROBABILISTIC;
    else if (Option("ptt_with_parameter_priors"))  	 TRACKER::algorithm = PTT_WITH_PARAMETER_PRIORS;
	else {
		std::cout << "Unknown algorithm: " << argv[argv_index] << ", valid options are \"ptt\", \"local_probabilistic\" and \"ptt_with_parameter_priors\" "<< std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;
    
    if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
        
        if (TRACKER::algorithm == PTT_C1) {
            if      (Option("C1"))    					TRACKER::algorithm = PTT_C1;
            else if (Option("C2"))    					TRACKER::algorithm = PTT_C2;
            // else if (Option("C3"))    					TRACKER::algorithm = PTT_C3;
            else {
                // std::cout << "Invalid option for ptt tracking type: " << argv[argv_index] << ", valid options are \"C1\", \"C2\" and \"C3\""<< std::endl;
                std::cout << "Invalid option for ptt tracking type: " << argv[argv_index] << ", valid options are \"C1\" and \"C2\" "<< std::endl;
                exit(EXIT_FAILURE);
            }
        } 
        
        if (TRACKER::algorithm == LOCAL_PROBABILISTIC){
            std::cout << "Invalid option for local_probabilistic tracking type: " << argv[argv_index] << ", this algorithm does not support any options "<< std::endl;
            exit(EXIT_FAILURE);
        }
        
        if (TRACKER::algorithm == PTT_WITH_PARAMETER_PRIORS){
            std::cout << "Invalid option for ptt_with_parameter_priors tracking type: " << argv[argv_index] << ", this algorithm does not support any options "<< std::endl;
            exit(EXIT_FAILURE);
        }
        
		argv_index++;
	}
    
    

}

void InputParser::parse_seed_image() {

	if (SEED::seedingMode !=SEED_NOTSET) {
		std::cout << "Cannot use -seed_image since a seeding option is already defined before" << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -seed_image" << std::endl;
		exit(EXIT_FAILURE);
	}

	if(!SEED::img_SEED->readHeader(argv[argv_index])) {
		std::cout << "Cannot read seed image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	SEED::seedingMode = SEED_IMAGE;
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		SEED::img_SEED->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

}

void InputParser::parse_seed_coordinates() {

	if (SEED::seedingMode != SEED_NOTSET) {
		std::cout << "Cannot use -seed_coordinates since a seeding option is already defined before" << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input a text file with  a list of seed coordinates after -seed_coordinates" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string coordinates;
	std::ifstream seeds(argv[argv_index]);

	if (!seeds.good()) {
		std::cout << "Cannot read seed coordinates from " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}


	SEED::seed_coordinate_fname = std::string{argv[argv_index]};


	int lineNo = 1;
	bool readError = false;
	while(std::getline(seeds,coordinates)) {
		if (!coordinates.empty()) {
			std::stringstream xyz(coordinates);
			float x,y,z;
			if (xyz.good()) xyz >> x; else { readError = true; break; }
			if (xyz.good()) xyz >> y; else { readError = true; break; }
			if (xyz.good()) xyz >> z; else { readError = true; break; }
			SEED::seed_coordinates.push_back(Coordinate(x,y,z));
		}
		lineNo++;
	}
	seeds.close();
	if (readError) {
		std::cout << "Cannot read seed coordinates from " << argv[argv_index] << ", line " << lineNo << ""<< std::endl;
		exit(EXIT_FAILURE);
	}

	SEED::seedingMode = SEED_COORDINATES;
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {

		std::string directions;
		std::ifstream seeds(argv[argv_index]);

		if (!seeds.good()) {
			std::cout << "Cannot read initial directions for seeds from " << argv[argv_index] << std::endl;
			exit(EXIT_FAILURE);
		}

		lineNo = 1;
		readError = false;
		while(std::getline(seeds,directions)) {
			if (!directions.empty()) {
				std::stringstream xyz(directions);
				float x,y,z;
				if (xyz.good()) xyz >> x; else { readError = true; break; }
				if (xyz.good()) xyz >> y; else { readError = true; break; }
				if (xyz.good()) xyz >> z; else { readError = true; break; }
				SEED::seed_init_directions.push_back(Coordinate(x,y,z));
			}
			lineNo++;
		}
		seeds.close();
		if (readError) {
			std::cout << "Cannot read initial directions from " << argv[argv_index] << ", line " << lineNo << std::endl;
			exit(EXIT_FAILURE);
		}

		SEED::seedingMode = SEED_COORDINATES_WITH_DIRECTIONS;

		if (SEED::seed_coordinates.size() != SEED::seed_init_directions.size()) {
			std::cout << "Number of seed coordinates does not match number of directions" << std::endl;
			exit(EXIT_FAILURE);
		}

		SEED::seed_init_directions_fname = std::string{argv[argv_index]};
		argv_index++;
	}

}

void InputParser::parse_seed_count() {
	if (SEED::count != NOTSET) {
		std::cout << "Cannot use -seed_count option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input count of seeds after -seed_count" << std::endl;
		exit(EXIT_FAILURE);
	}
	SEED::count = atoi(argv[argv_index]);

	if (SEED::count < 1) {
		std::cout << "Minimum seed count is 1" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (SEED::count > MAXNUMBEROFSEEDS) {
		std::cout << "Maximum number of seeds cannot exceed 1e9" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
}

void InputParser::parse_seed_countPerVoxel() {
	if (SEED::countPerVoxel != NOTSET) {
		std::cout << "Cannot use -seed_countPerVoxel option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input count of seeds per seed_image voxel after -seed_countPerVoxel" << std::endl;
		exit(EXIT_FAILURE);
	}
	SEED::countPerVoxel = atoi(argv[argv_index]);
	argv_index++;
}

void InputParser::parse_seed_maxTrials() {
	if (SEED::maxTrialsPerSeed != NOTSET) {
		std::cout << "Cannot use -seed_maxTrials option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input number of maximum trials per seeds after -seed_maxTrials" << std::endl;
		exit(EXIT_FAILURE);
	}
	SEED::maxTrialsPerSeed = atoi(argv[argv_index]);

	if (SEED::maxTrialsPerSeed < 1) {
		std::cout << "Minimum trial amount is 1" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (SEED::maxTrialsPerSeed > 1e9) {
		std::cout << "Maximum number of trials cannot exceed 1e9" << std::endl;
		exit(EXIT_FAILURE);
	}

	argv_index++;
}

void InputParser::parse_pathway_require_entry() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -require_entry" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->type = roi_type_req_entry;
	PATHWAY::order_of_ROIs.push_back(tmp);
	PATHWAY::img_ROI.push_back(tmp);

}

void InputParser::parse_pathwayA_require_entry() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -require_entry" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->type = roi_type_req_entry;
	tmp->side = side_A;
	PATHWAY::order_of_side_A_ROIs.push_back(tmp);
	PATHWAY::img_ROI.push_back(tmp);

}

void InputParser::parse_pathwayB_require_entry() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -require_entry" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->type = roi_type_req_entry;
	tmp->side = side_B;
	PATHWAY::order_of_side_B_ROIs.push_back(tmp);
	PATHWAY::img_ROI.push_back(tmp);

}

void InputParser::parse_pathway_require_exit() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -require_exit" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->type = roi_type_req_exit;
	PATHWAY::order_of_ROIs.push_back(tmp);
	PATHWAY::img_ROI.push_back(tmp);

}

void InputParser::parse_pathwayA_require_exit() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -require_exit" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->type = roi_type_req_exit;
	tmp->side = side_A;
	PATHWAY::order_of_side_A_ROIs.push_back(tmp);
	PATHWAY::img_ROI.push_back(tmp);

}

void InputParser::parse_pathwayB_require_exit() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -require_exit" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->type = roi_type_req_exit;
	tmp->side = side_B;
	PATHWAY::order_of_side_B_ROIs.push_back(tmp);
	PATHWAY::img_ROI.push_back(tmp);

}



void InputParser::parse_pathway_stop_at_entry() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -stop_at_entry" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->type = roi_type_stop_at_entry;
	PATHWAY::order_of_ROIs.push_back(tmp);
	PATHWAY::img_ROI.push_back(tmp);

}

void InputParser::parse_pathwayA_stop_at_entry() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -stop_at_entry" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->type = roi_type_stop_at_entry;
	tmp->side = side_A;
	PATHWAY::order_of_side_A_ROIs.push_back(tmp);
	PATHWAY::img_ROI.push_back(tmp);

}

void InputParser::parse_pathwayB_stop_at_entry() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -stop_at_entry" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->type = roi_type_stop_at_entry;
	tmp->side = side_B;
	PATHWAY::order_of_side_B_ROIs.push_back(tmp);
	PATHWAY::img_ROI.push_back(tmp);

}



void InputParser::parse_pathway_stop_at_exit() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -stop_at_exit" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->type = roi_type_stop_at_exit;
	PATHWAY::order_of_ROIs.push_back(tmp);
	PATHWAY::img_ROI.push_back(tmp);

}

void InputParser::parse_pathwayA_stop_at_exit() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -stop_at_exit" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->type = roi_type_stop_at_exit;
	tmp->side = side_A;
	PATHWAY::order_of_side_A_ROIs.push_back(tmp);
	PATHWAY::img_ROI.push_back(tmp);

}

void InputParser::parse_pathwayB_stop_at_exit() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -stop_at_exit" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->type = roi_type_stop_at_exit;
	tmp->side = side_B;
	PATHWAY::order_of_side_B_ROIs.push_back(tmp);
	PATHWAY::img_ROI.push_back(tmp);

}

void InputParser::parse_pathway_discard_if_enters() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -discard_if_enters" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->type = roi_type_discard_if_enters;
	PATHWAY::order_of_ROIs.push_back(tmp);
	PATHWAY::img_ROI.push_back(tmp);

}

void InputParser::parse_pathwayA_discard_if_enters() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -discard_if_enters" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->side = side_A;
	tmp->type = roi_type_discard_if_enters;
	PATHWAY::img_ROI.push_back(tmp);

}

void InputParser::parse_pathwayB_discard_if_enters() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -discard_if_enters" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->side = side_B;
	tmp->type = roi_type_discard_if_enters;
	PATHWAY::img_ROI.push_back(tmp);

}


void InputParser::parse_pathway_discard_if_exits() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -discard_if_exits" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->type = roi_type_discard_if_exits;
	PATHWAY::order_of_ROIs.push_back(tmp);
	PATHWAY::img_ROI.push_back(tmp);

}

void InputParser::parse_pathwayA_discard_if_exits() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -discard_if_exits" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->side = side_A;
	tmp->type = roi_type_discard_if_exits;
	PATHWAY::img_ROI.push_back(tmp);

}

void InputParser::parse_pathwayB_discard_if_exits() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -discard_if_exits" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->side = side_B;
	tmp->type = roi_type_discard_if_exits;
	PATHWAY::img_ROI.push_back(tmp);

}

void InputParser::parse_pathway_discard_if_ends_inside() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -discard_if_ends_inside" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->type = roi_type_discard_if_ends_inside;
	PATHWAY::order_of_ROIs.push_back(tmp);
	PATHWAY::img_ROI.push_back(tmp);

}

void InputParser::parse_pathwayA_discard_if_ends_inside() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -discard_if_ends_inside" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->side = side_A;
	tmp->type = roi_type_discard_if_ends_inside;
	PATHWAY::img_ROI.push_back(tmp);

}

void InputParser::parse_pathwayB_discard_if_ends_inside() {

	argv_index++;

	if ( (argv_index==argc) || (*argv[argv_index]=='-') ) {
		std::cout << "Input nifti file after -discard_if_ends_inside" << std::endl;
		exit(EXIT_FAILURE);
	}

	ROI_Image *tmp = new ROI_Image;
	if(!tmp->readHeader(argv[argv_index])) {
		std::cout << "Cannot read image: " << argv[argv_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	argv_index++;

	if ( (argv_index<argc) && (*argv[argv_index]!='-') ) {
		tmp->setLabel(atoi(argv[argv_index]));
		argv_index++;
	}

	tmp->side = side_B;
	tmp->type = roi_type_discard_if_ends_inside;
	PATHWAY::img_ROI.push_back(tmp);

}



void InputParser::parse_pathway_satisfy_requirements_in_order() {
	if (PATHWAY::satisfy_requirements_in_order != REQUIREMENT_ORDER_NOTSET) {
		std::cout << "Cannot use -pathway=satisfy_requirements_in_order option more than once" << std::endl;
		exit(EXIT_FAILURE);
	}

	PATHWAY::satisfy_requirements_in_order = IN_ORDER;
	argv_index++;
}
