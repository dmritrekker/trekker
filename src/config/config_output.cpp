#include "config_output.h"
#include "config_tracker.h"

using namespace TRACKER;

namespace OUTPUT {

// Output parameters
std::string   outputFilePath 			= "";
WriteMode     overwriteMode 		    = WRITE_NOTSET;
WriteMode     seedCoordinateWriteMode 	= WRITE_NOTSET;
WriteMode     dontWriteSegmentsInSeedROI= WRITE_NOTSET;
WriteMode     colorWriteMode 			= WRITE_NOTSET;
WriteMode     FODampWriteMode 			= WRITE_NOTSET;

// Output options for ptt algorithm
WriteMode     tangentWriteMode          = WRITE_NOTSET;
WriteMode     k1axisWriteMode           = WRITE_NOTSET;
WriteMode     k2axisWriteMode           = WRITE_NOTSET;
WriteMode     k1WriteMode               = WRITE_NOTSET;
WriteMode     k2WriteMode               = WRITE_NOTSET;
WriteMode     curvatureWriteMode        = WRITE_NOTSET;
WriteMode     likelihoodWriteMode       = WRITE_NOTSET;

void setDefaultParametersWhenNecessary() {

	// Default output parameters
	if (overwriteMode==WRITE_NOTSET) 				overwriteMode 			   = WRITE_OFF;
	if (seedCoordinateWriteMode==WRITE_NOTSET) 		seedCoordinateWriteMode    = WRITE_OFF;
    if (dontWriteSegmentsInSeedROI==WRITE_NOTSET) 	dontWriteSegmentsInSeedROI = WRITE_OFF;
	if (colorWriteMode==WRITE_NOTSET) 				colorWriteMode 			   = WRITE_OFF;
    if (FODampWriteMode==WRITE_NOTSET) 				FODampWriteMode 		   = WRITE_OFF;

	// Default parameters for ptt output
	if (tangentWriteMode==WRITE_NOTSET) 	tangentWriteMode 	= WRITE_OFF;
	if (k1axisWriteMode==WRITE_NOTSET) 		k1axisWriteMode 	= WRITE_OFF;
	if (k2axisWriteMode==WRITE_NOTSET) 		k2axisWriteMode 	= WRITE_OFF;
	if (k1WriteMode==WRITE_NOTSET) 			k1WriteMode 		= WRITE_OFF;
	if (k2WriteMode==WRITE_NOTSET) 			k2WriteMode 		= WRITE_OFF;
	if (curvatureWriteMode==WRITE_NOTSET) 	curvatureWriteMode 	= WRITE_OFF;
	if (likelihoodWriteMode==WRITE_NOTSET) 	likelihoodWriteMode = WRITE_OFF;

	bool checkOutput = false;


	int outputFilePathLength = outputFilePath.length();

	if (outputFilePathLength>3) {
		std::string checkExtension = outputFilePath.substr(outputFilePathLength-4,outputFilePathLength);
		if (checkExtension!=".vtk")
			outputFilePath = outputFilePath + ".vtk";
	} else
		outputFilePath = outputFilePath + ".vtk";


	if (FILE *tmp = fopen(outputFilePath.c_str(), "r")) {
		fclose(tmp);
		checkOutput  = true;
	}

	if (checkOutput && (overwriteMode==WRITE_OFF)){
		std::cout << outputFilePath << " already exists. Change output file name or use -enableOutputOverwrite." << std::endl;
		exit(EXIT_FAILURE);
	}
	if (checkOutput && (overwriteMode==WRITE_ON))
		if (GENERAL::verboseLevel!=QUITE)
			std::cout << std::endl << outputFilePath << " will be overwritten." << std::endl;

}

void print() {
	std::cout << std::endl;
	std::cout << "OUTPUT OPTIONS"<< std::endl;
	std::cout << "Output                     : "  << outputFilePath 	<< std::endl;

	std::cout << "enableOutputOverwrite      : ";
	switch(overwriteMode){
	case WRITE_OFF: std::cout << "OFF"; break;
	case WRITE_ON:  std::cout << "ON";  break;
	default: break;
	}
	std::cout << std::endl;

	std::cout << "writeSeedCoordinates       : ";
	switch(seedCoordinateWriteMode){
	case WRITE_OFF: std::cout << "OFF"; break;
	case WRITE_ON:  std::cout << "ON";  break;
	default: break;
	}
	std::cout << std::endl;
    
	std::cout << "dontWriteSegmentsInSeedROI : ";
	switch(dontWriteSegmentsInSeedROI){
	case WRITE_OFF: std::cout << "OFF"; break;
	case WRITE_ON:  std::cout << "ON";  break;
	default: break;
	}
	std::cout << std::endl;

	std::cout << "writeColors                : ";   
	switch(colorWriteMode){
	case WRITE_OFF: std::cout << "OFF"; break;
	case WRITE_ON:  std::cout << "ON";  break;
	default: break;
	}
	std::cout << std::endl;
    
	std::cout << "writeFODamp                : ";
	switch(FODampWriteMode){
	case WRITE_OFF: std::cout << "OFF"; break;
	case WRITE_ON:  std::cout << "ON";  break;
	default: break;
	}
	std::cout << std::endl;

	switch (TRACKER::algorithm) {
    case PTT:
    {
		std::cout << "writeTangents              : ";
		switch(tangentWriteMode){
		case WRITE_OFF: std::cout << "OFF"; break;
		case WRITE_ON:  std::cout << "ON";  break;
		default: break;
		}
		std::cout << std::endl;

		std::cout << "writek1axes                : ";
		switch(k1axisWriteMode){
		case WRITE_OFF: std::cout << "OFF"; break;
		case WRITE_ON:  std::cout << "ON";  break;
		default: break;
		}
		std::cout << std::endl;

		std::cout << "writek2axes                : ";
		switch(k2axisWriteMode){
		case WRITE_OFF: std::cout << "OFF"; break;
		case WRITE_ON:  std::cout << "ON";  break;
		default: break;
		}
		std::cout << std::endl;

		std::cout << "writek1s                   : ";
		switch(k1WriteMode){
		case WRITE_OFF: std::cout << "OFF"; break;
		case WRITE_ON:  std::cout << "ON";  break;
		default: break;
		}
		std::cout << std::endl;

		std::cout << "writek2s                   : ";
		switch(k2WriteMode){
		case WRITE_OFF: std::cout << "OFF"; break;
		case WRITE_ON:  std::cout << "ON";  break;
		default: break;
		}
		std::cout << std::endl;

		std::cout << "writeCurvatures            : ";
		switch(curvatureWriteMode){
		case WRITE_OFF: std::cout << "OFF"; break;
		case WRITE_ON:  std::cout << "ON";  break;
		default: break;
		}
		std::cout << std::endl;

		std::cout << "writeLikelihoods           : ";
		switch(likelihoodWriteMode){
		case WRITE_OFF: std::cout << "OFF"; break;
		case WRITE_ON:  std::cout << "ON";  break;
		default: break;
		}
		std::cout << std::endl;

		break;
	}
	case LOCAL_PROBABILISTIC:
		break;
	default :
		break;
	}

}

}
