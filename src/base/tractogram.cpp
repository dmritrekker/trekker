#include "tractogram.h"

Tractogram::Tractogram() {

	streamlineCount                             = 0;

	total_tried 								= 0;
	total_generated 							= 0;
	total_success 								= 0;
	total_discard 								= 0;
	total_fail 									= 0;

	total_success_REACHED_MAXLENGTH_LIMIT 		= 0;
	total_success_REACHED_MINDATASUPPORT_LIMIT 	= 0;
	total_success_SATISFIED_PATHWAY_RULES 		= 0;

	total_discard_TOO_SHORT 					= 0;
	total_discard_TOO_LONG 						= 0;
	total_discard_DISCARD_ROI_REACHED 			= 0;
	total_discard_REQUIRED_ROI_NOT_MET 			= 0;
	total_discard_REQUIRED_ROI_ORDER_NOT_MET 	= 0;
	total_discard_ENDED_INSIDE_DISCARD_ROI 		= 0;
    total_discard_REENTERED_SEED_ROI            = 0;
	total_discard_REACHED_TIME_LIMIT 			= 0;
	total_UNEXPECTED_TRACKING_STATUS 			= 0;

	total_failed_DISCARDED_BY_THE_ALGORITHM_DURING_INITIALIZATION 	= 0;
	total_failed_DISCARDED_BY_THE_ALGORITHM 						= 0;
	total_failed_REACHED_TRIAL_LIMIT_DURING_INITIALIZATION 			= 0;
	total_failed_REACHED_TRIAL_LIMIT 								= 0;

	total_propagationCount 						= 0;

	totalPointCount 							= 0;
	countGoodStreamlines 						= 0;

}

Tractogram::~Tractogram() {
	for (std::vector<Streamline*>::iterator it=streamlines.begin(); it!=streamlines.end(); it++){
		delete *it;
	}
}

void Tractogram::reset() {

	for (std::vector<Streamline*>::iterator it=streamlines.begin(); it!=streamlines.end(); it++){
		delete *it;
	}

	streamlines.clear();

	streamlineCount                             = 0;

	total_tried 								= 0;
	total_generated 							= 0;
	total_success 								= 0;
	total_discard 								= 0;
	total_fail 									= 0;

	total_success_REACHED_MAXLENGTH_LIMIT 		= 0;
	total_success_REACHED_MINDATASUPPORT_LIMIT 	= 0;
	total_success_SATISFIED_PATHWAY_RULES 		= 0;

	total_discard_TOO_SHORT 					= 0;
	total_discard_TOO_LONG 						= 0;
	total_discard_DISCARD_ROI_REACHED 			= 0;
	total_discard_REQUIRED_ROI_NOT_MET 			= 0;
	total_discard_REQUIRED_ROI_ORDER_NOT_MET 	= 0;
	total_discard_ENDED_INSIDE_DISCARD_ROI 		= 0;
    total_discard_REENTERED_SEED_ROI            = 0;
	total_discard_REACHED_TIME_LIMIT 			= 0;
	total_UNEXPECTED_TRACKING_STATUS 			= 0;

	total_failed_DISCARDED_BY_THE_ALGORITHM_DURING_INITIALIZATION 	= 0;
	total_failed_DISCARDED_BY_THE_ALGORITHM 						= 0;
	total_failed_REACHED_TRIAL_LIMIT_DURING_INITIALIZATION 			= 0;
	total_failed_REACHED_TRIAL_LIMIT 								= 0;

	total_propagationCount 						= 0;

	totalPointCount 							= 0;
	countGoodStreamlines 						= 0;

}

void Tractogram::printBaseSummary() {

	if (GENERAL::verboseLevel <= ON) {
		for (int i=0; i<lineCountToFlush; i++){
			printf("\033[1A");
			printf("\033[K");
		}
	}

	lineCountToFlush = 0;


	std::cout 	<< "Success: " 				<< total_success
			<< "      Discard: "  		<< total_discard
			<< "      Fail: "  			<< total_fail
			<< "      Total: "  		<< total_tried
			<< "      Duration: ";
	int tmp = GENERAL::runTime();
	if (tmp<GENERAL::timeLimit) std::cout << GENERAL::runTime();
	else 						std::cout << GENERAL::timeLimit;
	std::cout << " seconds" << std::endl;
	lineCountToFlush++;

	std::cout << std::endl;
	lineCountToFlush++;

	if (GENERAL::verboseLevel <= ON) {
		std::cout << "Success report" << std::endl;
		lineCountToFlush++;

		if (total_success_REACHED_MAXLENGTH_LIMIT>0) {
			std::cout 	<< " Reached maxlength                                : " << total_success_REACHED_MAXLENGTH_LIMIT << std::endl;
			lineCountToFlush++;
		}

		if (total_success_REACHED_MINDATASUPPORT_LIMIT>0) {
			std::cout 	<< " Reached min data support limit                   : " << total_success_REACHED_MINDATASUPPORT_LIMIT << std::endl;
			lineCountToFlush++;
		}

		if (total_success_SATISFIED_PATHWAY_RULES>0) {
			std::cout 	<< " Met pathway rule                                 : " << total_success_SATISFIED_PATHWAY_RULES << std::endl;
			lineCountToFlush++;
		}

		std::cout << std::endl;
		lineCountToFlush++;

		std::cout << "Discard report" << std::endl;
		lineCountToFlush++;

		if (total_discard_TOO_SHORT>0) {
			std::cout 	<< " Too short                                        : " << total_discard_TOO_SHORT << std::endl;
			lineCountToFlush++;
		}

		if (total_discard_TOO_LONG>0) {
			std::cout 	<< " Too long                                         : " << total_discard_TOO_LONG << std::endl;
			lineCountToFlush++;
		}

		if (total_discard_DISCARD_ROI_REACHED>0) {
			std::cout 	<< " Discard ROI reached                              : " << total_discard_DISCARD_ROI_REACHED << std::endl;
			lineCountToFlush++;
		}

		if (total_discard_REQUIRED_ROI_NOT_MET>0) {
			std::cout 	<< " Required ROI not met                             : " << total_discard_REQUIRED_ROI_NOT_MET << std::endl;
			lineCountToFlush++;
		}

		if (total_discard_REQUIRED_ROI_ORDER_NOT_MET>0) {
			std::cout 	<< " Required ROI order not met                       : " << total_discard_REQUIRED_ROI_ORDER_NOT_MET << std::endl;
			lineCountToFlush++;
		}

		if (total_discard_ENDED_INSIDE_DISCARD_ROI>0) {
			std::cout 	<< " Ended inside discard ROI                         : " << total_discard_ENDED_INSIDE_DISCARD_ROI << std::endl;
			lineCountToFlush++;
		}
		
        if (total_discard_REENTERED_SEED_ROI>0) {
			std::cout 	<< " Reentered seed ROI                               : " << total_discard_REENTERED_SEED_ROI << std::endl;
			lineCountToFlush++;
		}

		if (total_discard_REACHED_TIME_LIMIT>0) {
			std::cout 	<< " Reached time limit                               : " << total_discard_REACHED_TIME_LIMIT << std::endl;
			lineCountToFlush++;
		}

		std::cout << std::endl;
		lineCountToFlush++;

		std::cout << "Fail report" << std::endl;
		lineCountToFlush++;

		if (total_failed_DISCARDED_BY_THE_ALGORITHM_DURING_INITIALIZATION>0) {
			std::cout 	<< " Failed by algorithm during initialization        : " << total_failed_DISCARDED_BY_THE_ALGORITHM_DURING_INITIALIZATION << std::endl;
			lineCountToFlush++;
		}

		if (total_failed_DISCARDED_BY_THE_ALGORITHM>0) {
			std::cout 	<< " Failed by algorithm                              : " << total_failed_DISCARDED_BY_THE_ALGORITHM << std::endl;
			lineCountToFlush++;
		}

		if (total_failed_REACHED_TRIAL_LIMIT_DURING_INITIALIZATION>0) {
			std::cout 	<< " Reached seed trial limit during initialization   : " << total_failed_REACHED_TRIAL_LIMIT_DURING_INITIALIZATION << std::endl;
			lineCountToFlush++;
		}

		if (total_failed_REACHED_TRIAL_LIMIT>0) {
			std::cout 	<< " Reached seed trial limit                         : " << total_failed_REACHED_TRIAL_LIMIT << std::endl;
			lineCountToFlush++;
		}

		if (total_UNEXPECTED_TRACKING_STATUS>0) {
			std::cout 	<< " Unknown reason             : " << total_UNEXPECTED_TRACKING_STATUS << std::endl;
			lineCountToFlush++;
		}
	}

	if (GENERAL::verboseLevel > ON)
		std::cout << std::endl << std::endl;
}

void Tractogram::baseUpdate(TrackingThread *thread) {

	if (thread->streamline->status==STREAMLINE_GOOD) {
		total_success++;
		total_success_REACHED_MAXLENGTH_LIMIT 		+= thread->report_success_REACHED_MAXLENGTH_LIMIT;
		total_success_REACHED_MINDATASUPPORT_LIMIT 	+= thread->report_success_REACHED_MINDATASUPPORT_LIMIT;
		total_success_SATISFIED_PATHWAY_RULES 		+= thread->report_success_SATISFIED_PATHWAY_RULES;
	}

	total_discard_TOO_SHORT 										+= thread->report_discard_TOO_SHORT;
	total_discard_TOO_LONG 											+= thread->report_discard_TOO_LONG;
	total_discard_DISCARD_ROI_REACHED 								+= thread->report_discard_DISCARD_ROI_REACHED;
	total_discard_REQUIRED_ROI_NOT_MET 								+= thread->report_discard_REQUIRED_ROI_NOT_MET;
	total_discard_REQUIRED_ROI_ORDER_NOT_MET 						+= thread->report_discard_REQUIRED_ROI_ORDER_NOT_MET;
	total_discard_ENDED_INSIDE_DISCARD_ROI 							+= thread->report_discard_ENDED_INSIDE_DISCARD_ROI;
    total_discard_REENTERED_SEED_ROI 							    += thread->report_discard_REENTERED_SEED_ROI;
	total_discard_REACHED_TIME_LIMIT 								+= thread->report_discard_REACHED_TIME_LIMIT;

	total_discard =
			total_discard_TOO_SHORT+
			total_discard_TOO_LONG+
			total_discard_DISCARD_ROI_REACHED+
			total_discard_REQUIRED_ROI_NOT_MET+
			total_discard_REQUIRED_ROI_ORDER_NOT_MET+
			total_discard_ENDED_INSIDE_DISCARD_ROI+
			total_discard_REENTERED_SEED_ROI+
			total_discard_REACHED_TIME_LIMIT;


	total_failed_DISCARDED_BY_THE_ALGORITHM_DURING_INITIALIZATION 	+= thread->report_failed_BY_THE_ALGORITHM_DURING_INITIALIZATION;
	total_failed_DISCARDED_BY_THE_ALGORITHM 						+= thread->report_failed_BY_THE_ALGORITHM;
	total_failed_REACHED_TRIAL_LIMIT_DURING_INITIALIZATION 			+= thread->report_failed_REACHED_TRIAL_LIMIT_DURING_INITIALIZATION;
	total_failed_REACHED_TRIAL_LIMIT 								+= thread->report_failed_REACHED_TRIAL_LIMIT;
	total_UNEXPECTED_TRACKING_STATUS 								+= thread->report_UNEXPECTED_TRACKING_STATUS;

	total_fail =
			total_failed_DISCARDED_BY_THE_ALGORITHM_DURING_INITIALIZATION+
			total_failed_DISCARDED_BY_THE_ALGORITHM+
			total_failed_REACHED_TRIAL_LIMIT_DURING_INITIALIZATION+
			total_failed_REACHED_TRIAL_LIMIT+
			total_UNEXPECTED_TRACKING_STATUS;


	total_generated 	+= 1;

	total_tried 		 = total_success+total_discard+total_fail;

}

// VTK requires big-endian
void Tractogram::swapByteOrder_int(int& a) {
	char* byteArray = reinterpret_cast<char*>(&a);
	for(size_t i = 0; i < (sizeof(a)/2); i++)
		std::swap(byteArray[sizeof(a) - 1 - i],byteArray[i]);
}

// VTK requires big-endian
void Tractogram::swapByteOrder_float(float& a) {
	char* byteArray = reinterpret_cast<char*>(&a);
	for(size_t i = 0; i < (sizeof(a)/2); i++)
		std::swap(byteArray[sizeof(a) - 1 - i],byteArray[i]);
}


FILE* Tractogram::writeBaseOutput() {

	FILE *out;

	out = fopen((OUTPUT::outputFilePath).c_str(),"wb");

	if (out==NULL) {
		std::cout << "Cannot write output." << std::endl;
		exit(EXIT_FAILURE);
	}

	char buffer[256];
	sprintf(buffer, "# vtk DataFile Version 3.0\n");	fwrite(buffer, sizeof(char), strlen(buffer), out);
	sprintf(buffer, "Generated by trekker v0.6\n"); 	fwrite(buffer, sizeof(char), strlen(buffer), out);
	sprintf(buffer, "BINARY\n"); 						fwrite(buffer, sizeof(char), strlen(buffer), out);
	sprintf(buffer, "DATASET POLYDATA\n"); 				fwrite(buffer, sizeof(char), strlen(buffer), out);

	for (size_t i=0; i<streamlineCount; i++) {

		if (streamlines[i]->status == STREAMLINE_GOOD) {
			int pointCount   = streamlines[i]->coordinates.size();
			totalPointCount += pointCount;
			pointCounts.push_back(pointCount);
			countGoodStreamlines++;
		}
	}

	sprintf(buffer, "POINTS %d float\n", totalPointCount);
	fwrite(buffer, sizeof(char), strlen(buffer), out);
	for (size_t i=0; i<streamlineCount; i++)
		if (streamlines[i]->status == STREAMLINE_GOOD) {
			for (std::vector<Coordinate>::iterator it = streamlines[i]->coordinates.begin(); it != streamlines[i]->coordinates.end(); ++it) {
				float tmp;
				tmp = it->x; 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
				tmp = it->y; 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
				tmp = it->z; 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
			}
		}


	sprintf(buffer, "LINES %d %d\n",countGoodStreamlines,countGoodStreamlines+totalPointCount);
	fwrite(buffer, sizeof(char), strlen(buffer), out);

	int continue_index = 0;
	for (std::vector<int>::iterator it = pointCounts.begin(); it != pointCounts.end(); ++it) {

		int first_count = (*it);
		swapByteOrder_int(first_count); fwrite(&first_count, sizeof(int), 1, out);

		int first_index	= continue_index;
		int last_index	= first_index+(*it);
		for (int i=continue_index; i<last_index; i++) {
			int tmp = i;
			swapByteOrder_int(tmp); fwrite(&tmp, sizeof(int), 1, out);
		}

		continue_index = last_index;
	}


	if (seedCoordinateWriteMode==WRITE_ON) {
		sprintf(buffer,"CELL_DATA %d\n",countGoodStreamlines); 	fwrite(buffer, sizeof(char), strlen(buffer), out);
		sprintf(buffer,"SCALARS seedCoordinates float 3\n"); 	fwrite(buffer, sizeof(char), strlen(buffer), out);
		sprintf(buffer,"LOOKUP_TABLE default\n"); 				fwrite(buffer, sizeof(char), strlen(buffer), out);
		for (size_t i=0; i<streamlineCount; i++)
			if (streamlines[i]->status == STREAMLINE_GOOD) {
				swapByteOrder_float(streamlines[i]->seed_coordinates.x); fwrite(&(streamlines[i]->seed_coordinates.x), sizeof(float), 1, out);
				swapByteOrder_float(streamlines[i]->seed_coordinates.y); fwrite(&(streamlines[i]->seed_coordinates.y), sizeof(float), 1, out);
				swapByteOrder_float(streamlines[i]->seed_coordinates.z); fwrite(&(streamlines[i]->seed_coordinates.z), sizeof(float), 1, out);
			}
	}


	return out;


}

FILE* Tractogram::writeBaseMetadataOutput() {

	FILE *out;

	int outputFilePathLength = OUTPUT::outputFilePath.length();

	if (outputFilePathLength>3) {
		std::string checkExtension = OUTPUT::outputFilePath.substr(outputFilePathLength-4,outputFilePathLength);
		if (checkExtension==".vtk")
			out = fopen((OUTPUT::outputFilePath.substr(0,outputFilePathLength-4)+".json").c_str(),"w");
		else
			out = fopen((OUTPUT::outputFilePath+".json").c_str(),"w");

	} else
		out = fopen((OUTPUT::outputFilePath+".json").c_str(),"w");



	std::string tmp;

	fprintf(out,"{");
	fprintf(out,"\n\"tracker\": \"trekker v0.6\" ");



	struct tm * timeinfo;
	timeinfo = localtime(&startDate);
	std::ostringstream dateStamp;
	dateStamp << std::put_time(timeinfo, "%d %b %Y, %H:%M:%S");

	fprintf(out,(",\n\"date\":\"" + dateStamp.str() +"\"").c_str());

	fprintf(out,",\n\"GENERAL OPTIONS\": {");
	fprintf(out,("\n\"verbose level\":" + std::to_string(GENERAL::verboseLevel)).c_str());
	fprintf(out,(",\n\"number of threads\":" + std::to_string(GENERAL::numberOfThreads)).c_str());

	if (GENERAL::timeLimit==MAXTIMELIMIT)
		tmp = "infinite minutes";
	else {
		tmp = std::to_string(float(timeLimit)/60.0);
		if (float(timeLimit)/60.0<=1)
			tmp += " minute";
		else
			tmp += " minutes";
	}

	fprintf(out,(",\n\"time limit\":\"" + tmp +"\"").c_str());
	fprintf(out,"\n}");


	fprintf(out,",\n\"SEEDING OPTIONS\": {");
	switch(SEED::seedingMode) {
	case SEED_IMAGE:
		fprintf(out,"\n\"seeding mode\":\"seed_image\"");
		tmp = ",\n\"seed image\":\"";
		int label;
		if (SEED::img_SEED->getLabel(label))
			tmp += "(using label " + std::to_string(label) + ") ";
		tmp += SEED::img_SEED->getFilePath() + "\"";

		fprintf(out,tmp.c_str());

		break;
	case SEED_COORDINATES:
		fprintf(out,"\n\"seeding mode\":\"seed_coordinates\"");
		fprintf(out,(",\n\"seed coordinate file\":\"" + SEED::seed_coordinate_fname + "\"").c_str());
		break;
	case SEED_COORDINATES_WITH_DIRECTIONS:
		fprintf(out,"\n\"seeding mode\":\"seed_coordinates_with_directions\"");
		fprintf(out,(",\n\"seed coordinate file\":\"" + SEED::seed_coordinate_fname + "\"").c_str());
		fprintf(out,(",\n\"seed initial direction file\":\"" + SEED::seed_init_directions_fname + "\"").c_str());
		break;
	case SEED_NOTSET:
		break;
	default:
		break;
	}

	if ((SEED::count!=NOTSET) && (SEED::count!=MAXNUMBEROFSEEDS)) {
		fprintf(out,(",\n\"count\":" + std::to_string(SEED::count)).c_str());
	}

	if ((seedingMode==SEED_IMAGE) && (countPerVoxel!=NOTSET)) {
		fprintf(out,(",\n\"countPerVoxel\":" + std::to_string(SEED::countPerVoxel)).c_str());
	}

	fprintf(out,(",\n\"maxTrialsPerSeed\":" + std::to_string(SEED::maxTrialsPerSeed)).c_str());

	fprintf(out,"\n}");


	fprintf(out,",\n\"PATHWAY OPTIONS\": {");
	int ruleCount = 1;
	for (std::vector<ROI_Image*>::iterator it = img_ROI.begin(); it != img_ROI.end(); ++it) {
		switch ((*it)->type) {
		case roi_type_req_entry: 				tmp = "require_entry          "; break;
		case roi_type_req_exit: 				tmp = "require_exit           "; break;
		case roi_type_stop_at_entry:			tmp = "stop_at_entry          "; break;
		case roi_type_stop_at_exit: 			tmp = "stop_at_exit           "; break;
		case roi_type_discard_if_enters: 		tmp = "discard_if_enters      "; break;
		case roi_type_discard_if_exits: 		tmp = "discard_if_exits       "; break;
		case roi_type_discard_if_ends_inside: 	tmp = "discard_if_ends_inside "; break;
		default: break;
		}

		switch ((*it)->side) {
		case side_A: tmp += "(A) : "; break;
		case side_B: tmp += "(B) : "; break;
		default:     tmp += "    : "; break;
		}

		int label;
		if ((*it)->getLabel(label))
			tmp += "(using label " + std::to_string(label) + ") ";

		tmp += (*it)->getFilePath();

		if (ruleCount==1)
			fprintf(out, ("\n\"" + std::to_string(ruleCount) + "\":\"" + tmp + "\"").c_str());
		else
			fprintf(out,(",\n\"" + std::to_string(ruleCount) + "\":\"" + tmp + "\"").c_str());
		ruleCount++;

	}

	fprintf(out,"\n}");

	return out;

}

void Tractogram::writeTrackingReportInMetadataOutput(FILE *out) {

	fprintf(out,",\n\"TRACKING REPORT \": {");
	fprintf(out,("\n\"Duration (in seconds)\":" + std::to_string(GENERAL::runTime()) ).c_str());
	fprintf(out,(",\n\"Total number of streamlines generated\":" + std::to_string(total_tried) ).c_str());


	fprintf(out,",\n\"Success\": {");
	fprintf(out,("\n\"Total (in output)\":" + std::to_string(total_success)).c_str());

	if (total_success_REACHED_MAXLENGTH_LIMIT>0)
		fprintf(out,(",\n\"Reached maxlength\":" + std::to_string(total_success_REACHED_MAXLENGTH_LIMIT)).c_str());

	if (total_success_REACHED_MINDATASUPPORT_LIMIT>0)
		fprintf(out,(",\n\"Reached min data support limit\":" + std::to_string(total_success_REACHED_MINDATASUPPORT_LIMIT)).c_str());

	if (total_success_SATISFIED_PATHWAY_RULES>0)
		fprintf(out,(",\n\"Met pathway rule\":" + std::to_string(total_success_SATISFIED_PATHWAY_RULES)).c_str());

	fprintf(out,"\n}");


	fprintf(out,",\n\"Discarded (no problem with tracking but they have not met input constraints either)\": {");
	fprintf(out,("\n\"Total (not written in output due to discarding)\":" + std::to_string(total_discard)).c_str());

	if (total_discard_TOO_SHORT>0)
		fprintf(out,(",\n\"Too short\":" + std::to_string(total_discard_TOO_SHORT)).c_str());

	if (total_discard_TOO_LONG>0)
		fprintf(out,(",\n\"Too long\":" + std::to_string(total_discard_TOO_LONG)).c_str());

	if (total_discard_DISCARD_ROI_REACHED>0)
		fprintf(out,(",\n\"Discard ROI reached\":" + std::to_string(total_discard_DISCARD_ROI_REACHED)).c_str());

	if (total_discard_REQUIRED_ROI_NOT_MET>0)
		fprintf(out,(",\n\"Required ROI not met\":" + std::to_string(total_discard_REQUIRED_ROI_NOT_MET)).c_str());

	if (total_discard_REQUIRED_ROI_ORDER_NOT_MET>0)
		fprintf(out,(",\n\"Required ROI order not met\":" + std::to_string(total_discard_REQUIRED_ROI_ORDER_NOT_MET)).c_str());

	if (total_discard_ENDED_INSIDE_DISCARD_ROI>0)
		fprintf(out,(",\n\"Ended inside discard ROI\":" + std::to_string(total_discard_ENDED_INSIDE_DISCARD_ROI)).c_str());

    if (total_discard_REENTERED_SEED_ROI>0)
		fprintf(out,(",\n\"Reentered seed ROI\":" + std::to_string(total_discard_REENTERED_SEED_ROI)).c_str());
    
	if (total_discard_REACHED_TIME_LIMIT>0)
		fprintf(out,(",\n\"Reached time limit\":" + std::to_string(total_discard_REACHED_TIME_LIMIT)).c_str());

	fprintf(out,"\n}");


	fprintf(out,",\n\"Failed (tracking was not possible)\": {");
	fprintf(out,("\n\"Total (not written in output due to failure)\":" + std::to_string(total_fail)).c_str());

	if (total_failed_DISCARDED_BY_THE_ALGORITHM_DURING_INITIALIZATION>0)
		fprintf(out,(",\n\"Failed by algorithm during initialization \":" + std::to_string(total_failed_DISCARDED_BY_THE_ALGORITHM_DURING_INITIALIZATION)).c_str());

	if (total_failed_DISCARDED_BY_THE_ALGORITHM>0)
		fprintf(out,(",\n\"Failed by algorithm (e.g., reached sampling limit)\":" + std::to_string(total_failed_DISCARDED_BY_THE_ALGORITHM)).c_str());

	if (total_failed_REACHED_TRIAL_LIMIT_DURING_INITIALIZATION>0)
		fprintf(out,(",\n\"Reached seed trial limit during initialization\":" + std::to_string(total_failed_REACHED_TRIAL_LIMIT_DURING_INITIALIZATION)).c_str());

	if (total_failed_REACHED_TRIAL_LIMIT>0)
		fprintf(out,(",\n\"Reached seed trial limit\":" + std::to_string(total_failed_REACHED_TRIAL_LIMIT)).c_str());

	if (total_UNEXPECTED_TRACKING_STATUS>0)
		fprintf(out,(",\n\"Unknown reason\":" + std::to_string(total_UNEXPECTED_TRACKING_STATUS)).c_str());

	fprintf(out,"\n}");
	fprintf(out,"\n}");
	fprintf(out,"\n}");

}


