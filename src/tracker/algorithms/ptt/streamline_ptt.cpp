#include "streamline_ptt.h"

Streamline_PTT::Streamline_PTT() {

	sampling_prop_postEstItCount 	= 0;
	sampling_prop_generated 		= 0;
	sampling_prop_fail 				= 0;
	sampling_prop_tries 			= 0;
	sampling_prop_reject 			= 0;

	sampling_init_postEstItCount 	= 0;
	sampling_init_generated 		= 0;
	sampling_init_fail 				= 0;
	sampling_init_tries 			= 0;
	sampling_init_reject 			= 0;

}

Streamline_PTT::~Streamline_PTT() {
	this->flush();
}

void Streamline_PTT::flush() {

	coordinates.clear();

	status 										= STREAMLINE_NOTSET;
	discardingReason 							= DIRCARDINGREASON_NOTSET;
	failingReason 								= FAILREASON_NOTSET;
	terminationReason_sideA 					= TERMINATIONREASON_NOTSET;
	terminationReason_sideB 					= TERMINATIONREASON_NOTSET;

	tangent.clear();
	k1axis.clear();
	k2axis.clear();
	k1.clear();
	k2.clear();
	curvature.clear();
	prior.clear();
	likelihood.clear();
	posterior.clear();

	sampling_prop_postEstItCount 	= 0;
	sampling_prop_generated 		= 0;
	sampling_prop_fail 				= 0;
	sampling_prop_tries 			= 0;
	sampling_prop_reject 			= 0;

	sampling_init_postEstItCount 	= 0;
	sampling_init_generated 		= 0;
	sampling_init_fail 				= 0;
	sampling_init_tries 			= 0;
	sampling_init_reject 			= 0;

}

void Streamline_PTT::push(void *segment) {

	PTF *curve = ((PTF *)segment);

	coordinates.push_back(Coordinate(curve->p[0],curve->p[1],curve->p[2]));

	if (OUTPUT::tangentWriteMode==WRITE_ON) 	   tangent.push_back(Coordinate(curve->F[0][0], curve->F[0][1], curve->F[0][2] ));
	if (OUTPUT::k1axisWriteMode==WRITE_ON) 		    k1axis.push_back(Coordinate(curve->F[1][0], curve->F[1][1], curve->F[1][2] ));
	if (OUTPUT::k2axisWriteMode==WRITE_ON) 		    k2axis.push_back(Coordinate(curve->F[2][0], curve->F[2][1], curve->F[2][2] ));
	if (OUTPUT::k1WriteMode==WRITE_ON) 			        k1.push_back(curve->getk1());
	if (OUTPUT::k2WriteMode==WRITE_ON) 			        k2.push_back(curve->getk2());
	if (OUTPUT::curvatureWriteMode==WRITE_ON) 	 curvature.push_back(curve->getk());
	if (OUTPUT::priorWriteMode==WRITE_ON) 		     prior.push_back(curve->prior);
	if (OUTPUT::likelihoodWriteMode==WRITE_ON) 	likelihood.push_back(curve->likelihood);
	if (OUTPUT::posteriorWriteMode==WRITE_ON) 	 posterior.push_back(curve->posterior);

}

void Streamline_PTT::pop_back() {

	coordinates.pop_back();

	if (OUTPUT::tangentWriteMode==WRITE_ON) 	   tangent.pop_back();
	if (OUTPUT::k1axisWriteMode==WRITE_ON) 		    k1axis.pop_back();
	if (OUTPUT::k2axisWriteMode==WRITE_ON) 		    k2axis.pop_back();
	if (OUTPUT::k1WriteMode==WRITE_ON) 			        k1.pop_back();
	if (OUTPUT::k2WriteMode==WRITE_ON) 			        k2.pop_back();
	if (OUTPUT::curvatureWriteMode==WRITE_ON) 	 curvature.pop_back();
	if (OUTPUT::priorWriteMode==WRITE_ON) 		     prior.pop_back();
	if (OUTPUT::likelihoodWriteMode==WRITE_ON) 	likelihood.pop_back();
	if (OUTPUT::posteriorWriteMode==WRITE_ON) 	 posterior.pop_back();

}


void Streamline_PTT::flip() {

	std::reverse(coordinates.begin(),coordinates.end());

	if (OUTPUT::tangentWriteMode==WRITE_ON)
		std::reverse(tangent.begin(),tangent.end());

	if (OUTPUT::k1axisWriteMode==WRITE_ON)
		std::reverse(k1axis.begin(),k1axis.end());

	if (OUTPUT::k2axisWriteMode==WRITE_ON)
		std::reverse(k2axis.begin(),k2axis.end());

	if (OUTPUT::k1WriteMode==WRITE_ON)
		std::reverse(k1.begin(),k1.end());

	if (OUTPUT::k2WriteMode==WRITE_ON)
		std::reverse(k2.begin(),k2.end());

	if (OUTPUT::curvatureWriteMode==WRITE_ON)
		std::reverse(curvature.begin(),curvature.end());

	if (OUTPUT::priorWriteMode==WRITE_ON)
		std::reverse(prior.begin(),prior.end());

	if (OUTPUT::likelihoodWriteMode==WRITE_ON)
		std::reverse(likelihood.begin(),likelihood.end());

	if (OUTPUT::posteriorWriteMode==WRITE_ON)
		std::reverse(posterior.begin(),posterior.end());

}
