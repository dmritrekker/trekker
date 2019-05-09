#ifndef SRC_CONFIG_CONFIG_OUTPUT_H_
#define SRC_CONFIG_CONFIG_OUTPUT_H_

#include <cstdlib>
#include <iostream>
#include <fstream>

typedef enum {
	WRITE_NOTSET = -1,
	WRITE_OFF,
	WRITE_ON
} WriteMode;

namespace OUTPUT {

// Output parameters
extern std::string   outputFilePath;
extern WriteMode     overwriteMode;
extern WriteMode     seedCoordinateWriteMode;
extern WriteMode     colorWriteMode;


// Output options for ptt algorithm
extern WriteMode     tangentWriteMode;
extern WriteMode     k1axisWriteMode;
extern WriteMode     k2axisWriteMode;
extern WriteMode     k1WriteMode;
extern WriteMode     k2WriteMode;
extern WriteMode     curvatureWriteMode;
extern WriteMode     priorWriteMode;
extern WriteMode     likelihoodWriteMode;
extern WriteMode     posteriorWriteMode;

void setDefaultParametersWhenNecessary();
void print();

}

#endif
