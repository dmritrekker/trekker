#include "tractogram_local_probabilistic.h"

Tractogram_Local_Probabilistic::Tractogram_Local_Probabilistic() {

	sampling_all_prop_postEstItCount 	= 0;
	sampling_all_prop_generated 		= 0;
	sampling_all_prop_tries			 	= 0;
	sampling_all_prop_fail			 	= 0;
	sampling_all_prop_reject		 	= 0;

	prop_pooler 						= 0;

	if (TRACKER::propMaxEstTrials==NOTSET)
		prop_postEstItCount 			= DEFAULT_LOCAL_PROBABILISTIC_PROP_POSTESTITCOUNT_INITIAL_CONDITION;
	else
		prop_postEstItCount 			= TRACKER::propMaxEstTrials;

	prop_numberOfRecentStreamlines 		= 10;
	prop_numberOfRecentFails 			= 0;

	sampling_all_init_postEstItCount 	= 0;
	sampling_all_init_generated 		= 0;
	sampling_all_init_tries 			= 0;
	sampling_all_init_fail 				= 0;
	sampling_all_init_reject 			= 0;

	if (TRACKER::initMaxEstTrials==NOTSET)
		init_postEstItCount 			= DEFAULT_LOCAL_PROBABILISTIC_INIT_POSTESTITCOUNT_INITIAL_CONDITION;
	else
		init_postEstItCount 			= TRACKER::initMaxEstTrials;

	init_numberOfRecentStreamlines 		= 10;
	init_numberOfRecentFails 			= 0;

}


void Tractogram_Local_Probabilistic::printSummary(int &lineCountToFlush) {

	printBaseSummary(lineCountToFlush);


	if (GENERAL::verboseLevel == ON) {
		std::cout   << std::endl << "Sampling details" << std::endl;

		std::cout   << " --Initialization--" << std::endl;
		std::cout   << " Average number of candidates per initialization  : " << (float)sampling_all_init_tries/(float)sampling_all_init_generated << std::endl;
		std::cout   << " Average number of candidates for max. post. est. : " << (float)sampling_all_init_postEstItCount/(float)sampling_all_init_generated << std::endl;
		std::cout   << " Ratio of candidates with < minFODamp             : " << (float)sampling_all_init_reject/(float)sampling_all_init_tries*100 << " %" << std::endl;
		std::cout   << " Maximum posterior estimation failure rate        : " << (float)sampling_all_init_fail/(float)sampling_all_init_generated*100 << " %" << std::endl;

		std::cout   << " --Propagation--" << std::endl;
		std::cout   << " Average number of candidates per propagation     : " << (float)sampling_all_prop_tries/(float)sampling_all_prop_generated << std::endl;
		std::cout   << " Average number of candidates for max. post. est. : " << (float)sampling_all_prop_postEstItCount/(float)sampling_all_prop_generated << std::endl;
		std::cout   << " Ratio of candidates with < minFODamp             : " << (float)sampling_all_prop_reject/(float)sampling_all_prop_tries*100 << " %" << std::endl;
		std::cout   << " Maximum posterior estimation failure rate        : " << (float)sampling_all_prop_fail/(float)sampling_all_prop_generated*100 << " %" << std::endl;

		lineCountToFlush += 12;
	}


}

void Tractogram_Local_Probabilistic::update(TrackingThread *thread) {

	baseUpdate(thread);

	Streamline_Local_Probabilistic *streamline = ((Streamline_Local_Probabilistic *)thread->streamline);

	sampling_all_prop_generated			+= streamline->sampling_prop_generated;
	sampling_all_prop_postEstItCount 	+= streamline->sampling_prop_postEstItCount*streamline->sampling_prop_generated;
	sampling_all_prop_tries			 	+= streamline->sampling_prop_tries;
	sampling_all_prop_fail			 	+= streamline->sampling_prop_fail;
	sampling_all_prop_reject		 	+= streamline->sampling_prop_reject;

	sampling_all_init_generated 		+= streamline->sampling_init_generated;
	sampling_all_init_postEstItCount 	+= streamline->sampling_init_postEstItCount;
	sampling_all_init_tries 			+= streamline->sampling_init_tries;
	sampling_all_init_fail 				+= streamline->sampling_init_fail;
	sampling_all_init_reject 			+= streamline->sampling_init_reject;


	if (TRACKER::propMaxEstTrials==NOTSET) {
		if (streamline->sampling_init_fail==0) {

			prop_pooler++;
			prop_numberOfRecentFails += streamline->sampling_prop_fail;

			if (prop_pooler % prop_numberOfRecentStreamlines == 0) {
				if (prop_numberOfRecentFails>0)
					prop_postEstItCount++;
				else
					prop_postEstItCount--;

				if (prop_postEstItCount < DEFAULT_LOCAL_PROBABILISTIC_MINPOSTESTIT)  prop_postEstItCount = DEFAULT_LOCAL_PROBABILISTIC_MINPOSTESTIT;
				if (prop_postEstItCount > DEFAULT_LOCAL_PROBABILISTIC_MAXPOSTESTIT ) prop_postEstItCount = DEFAULT_LOCAL_PROBABILISTIC_MAXPOSTESTIT;
				prop_numberOfRecentFails = 0;
			}

		}
	}

	if (TRACKER::initMaxEstTrials==NOTSET) {
		init_numberOfRecentFails += streamline->sampling_init_fail;
		if (total_generated % init_numberOfRecentStreamlines == 0) {
			if (init_numberOfRecentFails>0)
				init_postEstItCount++;
			else
				init_postEstItCount--;

			if (init_postEstItCount < DEFAULT_LOCAL_PROBABILISTIC_MINPOSTESTITINIT)  init_postEstItCount = DEFAULT_LOCAL_PROBABILISTIC_MINPOSTESTITINIT;
			if (init_postEstItCount > DEFAULT_LOCAL_PROBABILISTIC_MAXPOSTESTITINIT ) init_postEstItCount = DEFAULT_LOCAL_PROBABILISTIC_MAXPOSTESTITINIT;
			init_numberOfRecentFails = 0;
		}
	}


}


void Tractogram_Local_Probabilistic::writeOutput() {

	FILE *out = writeBaseOutput();

	char buffer[256];

	if (colorWriteMode==WRITE_ON) {
		sprintf(buffer,"POINT_DATA %d\n",totalPointCount); 		fwrite(buffer, sizeof(char), strlen(buffer), out);
		sprintf(buffer,"SCALARS colors float 3\n"); 			fwrite(buffer, sizeof(char), strlen(buffer), out);
		sprintf(buffer,"LOOKUP_TABLE default\n"); 				fwrite(buffer, sizeof(char), strlen(buffer), out);
		for (size_t i=0; i<streamlineCount; i++)
			if (streamlines[i]->status == STREAMLINE_GOOD) {
				Coordinate direction;

				for (size_t j=0; j<(streamlines[i]->coordinates.size()-1); j++) {
					direction = streamlines[i]->coordinates.at(j+1)-streamlines[i]->coordinates.at(j);
					direction.normalize();
					float tmp;
					tmp = fabs(direction.x); 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
					tmp = fabs(direction.y); 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
					tmp = fabs(direction.z); 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
				}

				float tmp;
				tmp = fabs(direction.x); 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
				tmp = fabs(direction.y); 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
				tmp = fabs(direction.z); 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);

			}
	}

	fclose (out);
}

void Tractogram_Local_Probabilistic::writeMetadataOutput() {

	FILE *out = writeBaseMetadataOutput();
	fprintf(out,",\n\"TRACKER OPTIONS\": {");
	fprintf(out,"\n\"algorithm\": \"local probabilistic tracker (local_probabilistic) \" ");
	fprintf(out,(",\n\"stepSize\":" 			+ std::to_string(TRACKER::stepSize)).c_str());
	fprintf(out,(",\n\"minRadiusOfCurvature\":" + std::to_string(TRACKER::minRadiusOfCurvature)).c_str());
	fprintf(out,(",\n\"minFODamp\":" 			+ std::to_string(TRACKER::minFODamp)).c_str());
	fprintf(out,(",\n\"minLength\":" 			+ std::to_string(TRACKER::minLength)).c_str());


	if (TRACKER::maxLength==DEFAULT_LOCAL_PROBABILISTIC_MAXLENGTH)
		fprintf(out,",\n\"maxLength\":\"infinite\"");
	else
		fprintf(out,(",\n\"maxLength\":" 		+ std::to_string(TRACKER::maxLength)).c_str());


	switch(TRACKER::atMaxLength) {
	case(ATMAXLENGTH_STOP):
												fprintf(out,",\n\"atMaxLength\":\"stop\"");
	break;
	case(ATMAXLENGTH_DISCARD):
												fprintf(out,",\n\"atMaxLength\":\"discard\"");
	break;
	case(ATMAXLENGTH_NOTSET):
												break;
	default:
		break;
	}

	fprintf(out,(",\n\"writeInterval\":" 		+ std::to_string(TRACKER::writeInterval)).c_str());
	switch(TRACKER::directionality) {
	case(ONE_SIDED):
												fprintf(out,",\n\"directionality\":\"one_sided\"");
	break;
	case(TWO_SIDED):
												fprintf(out,",\n\"directionality\":\"two_sided\"");
	break;
	case(DIRECTIONALITY_NOTSET):
												break;
	default:
		break;
	}
	fprintf(out,(",\n\"maxSamplingPerStep\":" 		+ std::to_string(TRACKER::triesPerRejectionSampling)).c_str());

	if (TRACKER::initMaxEstTrials==NOTSET)
		fprintf(out,",\n\"initMaxEstTrials\":\"adaptive\"");
	else
		fprintf(out,(",\n\"initMaxEstTrials\":" 		+ std::to_string(TRACKER::initMaxEstTrials)).c_str());

	if (TRACKER::propMaxEstTrials==NOTSET)
		fprintf(out,",\n\"propMaxEstTrials\":\"adaptive\"");
	else
		fprintf(out,(",\n\"propMaxEstTrials\":" 		+ std::to_string(TRACKER::propMaxEstTrials)).c_str());


	if (TRACKER::atInit==ATINIT_USEBEST)
		fprintf(out,",\n\"useBestAtInit\":\"ON\"");
	else
		fprintf(out,",\n\"useBestAtInit\":\"OFF\"");


	fprintf(out,(",\n\"FOD image\":\"" + TRACKER::img_FOD->getFilePath() + "\"").c_str());
	fprintf(out,"\n}");

	fprintf(out,",\n\"OUTPUT OPTIONS\": {");
	fprintf(out,("\n\"output\":\"" + OUTPUT::outputFilePath + "\"").c_str());

	// Common output options
	if (OUTPUT::overwriteMode==WRITE_OFF) 			fprintf(out,",\n\"enableOutputOverwrite\":\"OFF\""); else fprintf(out,",\n\"enableOutputOverwrite\":\"ON\"");
	if (OUTPUT::seedCoordinateWriteMode==WRITE_OFF)	fprintf(out,",\n\"writeSeedCoordinates\":\"OFF\"");  else fprintf(out,",\n\"writeSeedCoordinates\":\"ON\"");
	if (OUTPUT::colorWriteMode==WRITE_OFF) 			fprintf(out,",\n\"writeColors\":\"OFF\"");  		 else fprintf(out,",\n\"writeColors\":\"ON\"");

	fprintf(out,"\n}");


	writeTrackingReportInMetadataOutput(out);
	fclose (out);

}
