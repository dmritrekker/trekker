#include "tractogram_ptt.h"

Tractogram_PTT::Tractogram_PTT() {

	sampling_all_prop_postEstItCount 	= 0;
	sampling_all_prop_generated 		= 0;
	sampling_all_prop_tries			 	= 0;
	sampling_all_prop_fail			 	= 0;
	sampling_all_prop_reject		 	= 0;

	prop_pooler 						= 0;

	if (TRACKER::propMaxEstTrials==NOTSET)
		prop_postEstItCount 			= DEFAULT_PTT_PROP_POSTESTITCOUNT_INITIAL_CONDITION;
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
		init_postEstItCount 			= DEFAULT_PTT_INIT_POSTESTITCOUNT_INITIAL_CONDITION;
	else
		init_postEstItCount 			= TRACKER::initMaxEstTrials;

	init_numberOfRecentStreamlines 		= 10;
	init_numberOfRecentFails 			= 0;
}


void Tractogram_PTT::printSummary() {

	printBaseSummary();

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

void Tractogram_PTT::update(TrackingThread *thread) {

	baseUpdate(thread);

	Streamline_PTT *streamline 			 = ((Streamline_PTT*)thread->streamline);

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

				if (prop_postEstItCount < DEFAULT_PTT_PROP_MINPOSTESTIT)  prop_postEstItCount = DEFAULT_PTT_PROP_MINPOSTESTIT;
				if (prop_postEstItCount > DEFAULT_PTT_PROP_MAXPOSTESTIT)  prop_postEstItCount = DEFAULT_PTT_PROP_MAXPOSTESTIT;
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

			if (init_postEstItCount < DEFAULT_PTT_INIT_MINPOSTESTIT)  init_postEstItCount = DEFAULT_PTT_INIT_MINPOSTESTIT;
			if (init_postEstItCount > DEFAULT_PTT_INIT_MAXPOSTESTIT ) init_postEstItCount = DEFAULT_PTT_INIT_MAXPOSTESTIT;
			init_numberOfRecentFails = 0;
		}
	}

}


void Tractogram_PTT::writeOutput() {

	FILE *out = writeBaseOutput();


	char buffer[256];


	if (
			(colorWriteMode==WRITE_ON) 		||
			(FODampWriteMode==WRITE_ON) 	||
			(tangentWriteMode==WRITE_ON) 	||
			(k1axisWriteMode==WRITE_ON)  	||
			(k2axisWriteMode==WRITE_ON) 	||
			(k1WriteMode==WRITE_ON) 		||
			(k2WriteMode==WRITE_ON) 		||
			(curvatureWriteMode==WRITE_ON) 	||
			(likelihoodWriteMode==WRITE_ON) ) {
		sprintf(buffer,"POINT_DATA %d\n",totalPointCount); 		fwrite(buffer, sizeof(char), strlen(buffer), out);
	}

	if (colorWriteMode==WRITE_ON) {
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
	
    if (FODampWriteMode==WRITE_ON) {
		sprintf(buffer,"SCALARS FODamp float 1\n"); 			fwrite(buffer, sizeof(char), strlen(buffer), out);
		sprintf(buffer,"LOOKUP_TABLE default\n"); 				fwrite(buffer, sizeof(char), strlen(buffer), out);
		for (size_t i=0; i<streamlineCount; i++)
			if (streamlines[i]->status == STREAMLINE_GOOD) {
                float  point[3];
                float  direction[3];
                float* FOD = new float[SH::numberOfSphericalHarmonicCoefficients];
                float  FODamp;
                
				for (size_t j=0; j<(streamlines[i]->coordinates.size()-1); j++) {
                    
                    Coordinate tmp  = streamlines[i]->coordinates.at(j+1)-streamlines[i]->coordinates.at(j);
					tmp.normalize();
                    
                    streamlines[i]->coordinates.at(j).copyToFloatArray(point);
                    tmp.copyToFloatArray(direction);
                    
                    
                    if (TRACKER::fodDiscretization==FODDISC_OFF) {
                        img_FOD->getVal(point,FOD);
                        FODamp = SH::SH_amplitude(FOD,direction);
                    } else
                        FODamp = img_FOD->getFODval(point,direction);
                    
                    swapByteOrder_float(FODamp); fwrite(&FODamp, sizeof(float), 1, out);
                    
				}

				streamlines[i]->coordinates.back().copyToFloatArray(point);
                if (TRACKER::fodDiscretization==FODDISC_OFF) {
                        img_FOD->getVal(point,FOD);
                        FODamp = SH::SH_amplitude(FOD,direction);
                    } else
                        FODamp = img_FOD->getFODval(point,direction);
				
				swapByteOrder_float(FODamp); fwrite(&FODamp, sizeof(float), 1, out);
            
                delete[] FOD;
			}
			
	}

	if (tangentWriteMode==WRITE_ON) {
		sprintf(buffer,"SCALARS tangents float 3\n"); 			fwrite(buffer, sizeof(char), strlen(buffer), out);
		sprintf(buffer,"LOOKUP_TABLE default\n"); 				fwrite(buffer, sizeof(char), strlen(buffer), out);
		for (size_t i=0; i<streamlineCount; i++)
			if (streamlines[i]->status == STREAMLINE_GOOD) {
				Streamline_PTT *streamline = ((Streamline_PTT *)streamlines[i]);
				for (std::vector<Coordinate>::iterator it = streamline->tangent.begin(); it != streamline->tangent.end(); ++it) {
					float tmp;
					tmp = it->x; 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
					tmp = it->y; 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
					tmp = it->z; 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
				}
			}
	}

	if (k1axisWriteMode==WRITE_ON) {
		sprintf(buffer,"SCALARS k1axes float 3\n"); 			fwrite(buffer, sizeof(char), strlen(buffer), out);
		sprintf(buffer,"LOOKUP_TABLE default\n"); 				fwrite(buffer, sizeof(char), strlen(buffer), out);
		for (size_t i=0; i<streamlineCount; i++)
			if (streamlines[i]->status == STREAMLINE_GOOD) {
				Streamline_PTT *streamline = ((Streamline_PTT *)streamlines[i]);
				for (std::vector<Coordinate>::iterator it = streamline->k1axis.begin(); it != streamline->k1axis.end(); ++it) {
					float tmp;
					tmp = it->x; 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
					tmp = it->y; 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
					tmp = it->z; 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
				}
			}
	}

	if (k2axisWriteMode==WRITE_ON) {
		sprintf(buffer,"SCALARS k2axes float 3\n"); 			fwrite(buffer, sizeof(char), strlen(buffer), out);
		sprintf(buffer,"LOOKUP_TABLE default\n"); 				fwrite(buffer, sizeof(char), strlen(buffer), out);
		for (size_t i=0; i<streamlineCount; i++)
			if (streamlines[i]->status == STREAMLINE_GOOD) {
				Streamline_PTT *streamline = ((Streamline_PTT *)streamlines[i]);
				for (std::vector<Coordinate>::iterator it = streamline->k2axis.begin(); it != streamline->k2axis.end(); ++it) {
					float tmp;
					tmp = it->x; 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
					tmp = it->y; 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
					tmp = it->z; 	swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
				}
			}
	}

	if (k1WriteMode==WRITE_ON) {
		sprintf(buffer,"SCALARS k1s float 1\n"); 				fwrite(buffer, sizeof(char), strlen(buffer), out);
		sprintf(buffer,"LOOKUP_TABLE default\n"); 				fwrite(buffer, sizeof(char), strlen(buffer), out);
		for (size_t i=0; i<streamlineCount; i++)
			if (streamlines[i]->status == STREAMLINE_GOOD) {
				Streamline_PTT *streamline = ((Streamline_PTT *)streamlines[i]);
				for (std::vector<float>::iterator it = streamline->k1.begin(); it != streamline->k1.end(); ++it) {
					float tmp;
					tmp = *it; 		swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
				}
			}
	}

	if (k2WriteMode==WRITE_ON) {
		sprintf(buffer,"SCALARS k2s float 1\n"); 				fwrite(buffer, sizeof(char), strlen(buffer), out);
		sprintf(buffer,"LOOKUP_TABLE default\n"); 				fwrite(buffer, sizeof(char), strlen(buffer), out);
		for (size_t i=0; i<streamlineCount; i++)
			if (streamlines[i]->status == STREAMLINE_GOOD) {
				Streamline_PTT *streamline = ((Streamline_PTT *)streamlines[i]);
				for (std::vector<float>::iterator it = streamline->k2.begin(); it != streamline->k2.end(); ++it) {
					float tmp;
					tmp = *it; 		swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
				}
			}
	}

	if (curvatureWriteMode==WRITE_ON) {
		sprintf(buffer,"SCALARS curvatures float 1\n"); 		fwrite(buffer, sizeof(char), strlen(buffer), out);
		sprintf(buffer,"LOOKUP_TABLE default\n"); 				fwrite(buffer, sizeof(char), strlen(buffer), out);
		for (size_t i=0; i<streamlineCount; i++)
			if (streamlines[i]->status == STREAMLINE_GOOD) {
				Streamline_PTT *streamline = ((Streamline_PTT *)streamlines[i]);
				for (std::vector<float>::iterator it = streamline->curvature.begin(); it != streamline->curvature.end(); ++it) {
					float tmp;
					tmp = *it; 		swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
				}
			}
	}

	if (likelihoodWriteMode==WRITE_ON) {
		sprintf(buffer,"SCALARS likelihoods float 1\n"); 		fwrite(buffer, sizeof(char), strlen(buffer), out);
		sprintf(buffer,"LOOKUP_TABLE default\n"); 				fwrite(buffer, sizeof(char), strlen(buffer), out);
		for (size_t i=0; i<streamlineCount; i++)
			if (streamlines[i]->status == STREAMLINE_GOOD) {
				Streamline_PTT *streamline = ((Streamline_PTT *)streamlines[i]);
				for (std::vector<float>::iterator it = streamline->likelihood.begin(); it != streamline->likelihood.end(); ++it) {
					float tmp;
					tmp = *it; 		swapByteOrder_float(tmp); fwrite(&tmp, sizeof(float), 1, out);
				}
			}
	}

	fclose (out);

}

void Tractogram_PTT::writeMetadataOutput() {

	FILE *out = writeBaseMetadataOutput();

	fprintf(out,",\n\"TRACKER OPTIONS\": {");
    
    fprintf(out,"\n\"algorithm\": \"probabilistic parallel transport tracker (ptt) \" ");

	fprintf(out,(",\n\"stepSize\":" 			+ std::to_string(TRACKER::stepSize)).c_str());
	fprintf(out,(",\n\"minRadiusOfCurvature\":" + std::to_string(TRACKER::minRadiusOfCurvature)).c_str());
	fprintf(out,(",\n\"probeLength\":" 			+ std::to_string(TRACKER::probeLength)).c_str());
	fprintf(out,(",\n\"probeRadius\":" 			+ std::to_string(TRACKER::probeRadius)).c_str());
	fprintf(out,(",\n\"probeCount\":" 			+ std::to_string(TRACKER::probeCount)).c_str());
	fprintf(out,(",\n\"probeQuality\":" 		+ std::to_string(TRACKER::probeQuality)).c_str());
	fprintf(out,(",\n\"minFODamp\":" 			+ std::to_string(TRACKER::minFODamp)).c_str());
    fprintf(out,(",\n\"maxEstInterval\":" 		+ std::to_string(TRACKER::maxEstInterval)).c_str());
    fprintf(out,(",\n\"ignoreWeakLinks\":" 	    + std::to_string(TRACKER::weakLinkThresh)).c_str());
    fprintf(out,(",\n\"dataSupportExponent\":" 	+ std::to_string(TRACKER::dataSupportExponent)).c_str());
	fprintf(out,(",\n\"minLength\":" 			+ std::to_string(TRACKER::minLength)).c_str());

	if (TRACKER::maxLength==DEFAULT_PTT_MAXLENGTH)
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

	fprintf(out,(",\n\"fod\":\"" + TRACKER::img_FOD->getFilePath() + "\"").c_str());
    
    if (TRACKER::fodDiscretization==FODDISC_ON)
        fprintf(out,",\n\"fodDiscretization\":\"ON\"");	
    else
        fprintf(out,",\n\"fodDiscretization\":\"OFF\"");
    
    if (TRACKER::orderOfDirectionsTextInput=="")
        fprintf(out,",\n\"orderOfDirections\":\"XYZ\"");	
    else
        fprintf(out,(",\n\"orderOfDirections\":\"" + TRACKER::orderOfDirectionsTextInput + "\"").c_str());
    fprintf(out,"\n}");


	fprintf(out,",\n\"OUTPUT OPTIONS\": {");
	fprintf(out,("\n\"output\":\"" + OUTPUT::outputFilePath + "\"").c_str());

	// Common output options
	if (OUTPUT::overwriteMode==WRITE_OFF) 			   fprintf(out,",\n\"enableOutputOverwrite\":\"OFF\""); else fprintf(out,",\n\"enableOutputOverwrite\":\"ON\"");
	if (OUTPUT::seedCoordinateWriteMode==WRITE_OFF)	   fprintf(out,",\n\"writeSeedCoordinates\":\"OFF\"");  else fprintf(out,",\n\"writeSeedCoordinates\":\"ON\"");
    if (OUTPUT::dontWriteSegmentsInSeedROI==WRITE_OFF) fprintf(out,",\n\"dontWriteSegmentsInSeedROI\":\"OFF\"");  else fprintf(out,",\n\"dontWriteSegmentsInSeedROI\":\"ON\"");
	if (OUTPUT::colorWriteMode==WRITE_OFF) 			   fprintf(out,",\n\"writeColors\":\"OFF\"");  		 else fprintf(out,",\n\"writeColors\":\"ON\"");
    if (OUTPUT::FODampWriteMode==WRITE_OFF) 		   fprintf(out,",\n\"writeFODamp\":\"OFF\"");  		 else fprintf(out,",\n\"writeFODamp\":\"ON\"");

	// Output options for ptt
	if (OUTPUT::tangentWriteMode==WRITE_OFF) 		   fprintf(out,",\n\"writeTangents\":\"OFF\"");  	 	 else fprintf(out,",\n\"writeTangents\":\"ON\"");
	if (OUTPUT::k1axisWriteMode==WRITE_OFF) 		   fprintf(out,",\n\"writek1axes\":\"OFF\"");  		 else fprintf(out,",\n\"writek1axes\":\"ON\"");
	if (OUTPUT::k2axisWriteMode==WRITE_OFF) 		   fprintf(out,",\n\"writek2axes\":\"OFF\"");  		 else fprintf(out,",\n\"writek2axes\":\"ON\"");
	if (OUTPUT::k1WriteMode==WRITE_OFF) 			   fprintf(out,",\n\"writek1s\":\"OFF\"");  			 else fprintf(out,",\n\"writek1s\":\"ON\"");
	if (OUTPUT::k2WriteMode==WRITE_OFF) 			   fprintf(out,",\n\"writek2s\":\"OFF\"");  			 else fprintf(out,",\n\"writek2s\":\"ON\"");
	if (OUTPUT::curvatureWriteMode==WRITE_OFF) 		   fprintf(out,",\n\"writeCurvatures\":\"OFF\"");  	 else fprintf(out,",\n\"writeCurvatures\":\"ON\"");
	if (OUTPUT::likelihoodWriteMode==WRITE_OFF) 	   fprintf(out,",\n\"writeLikelihoods\":\"OFF\"");  	 else fprintf(out,",\n\"writeLikelihoods\":\"ON\"");

	fprintf(out,"\n}");

	writeTrackingReportInMetadataOutput(out);

	fclose (out);

}
