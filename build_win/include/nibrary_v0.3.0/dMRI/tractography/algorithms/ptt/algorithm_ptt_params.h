#pragma once

#include "base/nibr.h"
#include "dMRI/imageTypes/fod_image.h"
#include <set>

#define DEFAULT_PTT_STEPSIZE_IN_PIXELDIM                       0.025
#define DEFAULT_PTT_OUTPUTSTEPSIZE_IN_PIXELDIM                   0.5
#define DEFAULT_PTT_MINRADIUSOFCURVATURE_IN_PIXELDIM             0.5
#define DEFAULT_PTT_MINDATASUPPORT                              0.05
#define DEFAULT_PTT_DATASUPPORTEXPONENT                            1
#define DEFAULT_PTT_WEAKLINKTHRESH                                 0
#define DEFAULT_PTT_WRITEINTERVAL_IN_PIXELDIM                    0.5

#define DEFAULT_PTT_MAXESTINTERVAL                                 1
#define DEFAULT_PTT_INITMAXESTTRIALS                             100
#define DEFAULT_PTT_PROPMAXESTTRIALS                              20
#define DEFAULT_PTT_TRIESPERREJECTIONSAMPLING                   1000
#define DEFAULT_PTT_USEBESTATINIT                              false
#define DEFAULT_PTT_USELEGACYSAMPLING                          false
#define DEFAULT_PTT_SAMPLINGQUALITY                                2

#define DEFAULT_PTT_PROBELENGTH_IN_PIXELDIM                     0.25
#define DEFAULT_PTT_PROBERADIUS_IN_PIXELDIM                      0.5
#define DEFAULT_PTT_PROBECOUNT                                     1
#define DEFAULT_PTT_PROBECOUNT_WHEN_THEREIS_PROBERADIUS            4
#define DEFAULT_PTT_PROBEQUALITY                                   4

#define DEFAULT_PTT_MAXPOSTESTCOMPENS 	  		   			       2

namespace NIBR {

// Tracking parameters
class Params_PTT {

public:

	Params_PTT(){};
	~Params_PTT(){clear();}

	void needsUpdate() {pttIsReady = false;}
	void clear();
	bool update();
	void print();

	// FOD options in Trekker
	std::string           	 	img_FOD_path{""};
	std::string           	 	fod_sphere_path{""};
	bool 						fodIsSym{true};
	bool 						fodDiscretization{true};
	std::string           	    orderOfDirectionsTextInput{""};
	
	// Tracking options
	float                		weakLinkThresh{NAN};

	float                       outputStep_global{NAN};
	float                		stepSize_global{NAN};

	std::string      	 	    outputStep_img_path{""};
	std::string      	 	    stepSize_img_path{""};

	float                		minRadiusOfCurvature_global{NAN};
	float                		minDataSupport_global{NAN};
	float                		dataSupportExponent_global{NAN};

	std::string      	 	    minRadiusOfCurvature_img_path{""};
	std::string      	 	    minDataSupport_img_path{""};
	std::string      	 	    dataSupportExponent_img_path{""};


	// Sampling options
	bool 		    	 		useBestAtInit{false}; 
	bool 		    	 		useLegacySampling{false};
	int			    	 		samplingQuality{-1};

	int             	 		maxEstInterval_global{-1};
	int			    	 		initMaxEstTrials_global{-1};
	int			    	 		propMaxEstTrials_global{-1};
	int 		         		triesPerRejectionSampling_global{-1};

	std::string             	maxEstInterval_img_path{""};
	std::string			    	initMaxEstTrials_img_path{""};
	std::string			    	propMaxEstTrials_img_path{""};
	std::string 		        triesPerRejectionSampling_img_path{""};

	// Probe options
	float               		probeCount_global{NAN};
    float                		probeRadius_global{NAN};
    float               		probeLength_global{NAN};
	float                		probeQuality_global{NAN};

	std::string      	 	    probeCount_img_path{""};
	std::string      	 	    probeRadius_img_path{""};
	std::string      	 	    probeLength_img_path{""};
	std::string      	 	    probeQuality_img_path{""};

	// Superpeak options
	std::string           	    img_peaks_path{""};
	std::string           	    img_bias_path{""};

	// Parameter mask
	std::string           	    img_param_mask_path{""};


	// Derived parameters
	FOD_Image*           	 	img_FOD{NULL};
	Image<bool>*    	 	    img_param_mask{NULL};

	Image<float>*      	 	    outputStep_img{NULL};
	Image<float>*      	 	    stepSize_img{NULL};

	Image<float>*      	 	    minRadiusOfCurvature_img{NULL};
	Image<float>*      	 	    minDataSupport_img{NULL};
	Image<float>*      	 	    dataSupportExponent_img{NULL};

	Image<int>*             	maxEstInterval_img{NULL};
	Image<int>*			    	initMaxEstTrials_img{NULL};
	Image<int>*			    	propMaxEstTrials_img{NULL};
	Image<int>* 		        triesPerRejectionSampling_img{NULL};

	Image<float>*      	 	    probeCount_img{NULL};
	Image<float>*      	 	    probeRadius_img{NULL};
	Image<float>*      	 	    probeLength_img{NULL};
	Image<float>*      	 	    probeQuality_img{NULL};

	bool                 		checkWeakLinks{false};

	float                		maxCurvature_global{NAN};
	float 				 		modMinDataSupport_global{NAN};

	int 												cdfVertCnt;
	int 												cdfFaceCnt;
	std::vector<float>									cdfCurvatures;
	std::vector<std::vector<std::pair<float,float>>> 	cdfk1k2_global;
	std::vector<std::array<int,3>>						cdfFace_global;
	
	// List of parameters to refresh while tracking
	std::set<std::string> 		toRefresh;

private:

	void 						setDefaults();

	bool                        pttIsReady{false};

	std::vector<Point>			fodSphere;
	bool                        fodIsSpheresliced{false};
	float		    		    smallestPixDim{NAN};
	OrderOfDirections			orderOfDirections{XYZ};	

};

}


using namespace NIBR;

template<typename T>
void delete_img(Image<T>*& img) {
	if (img!=NULL) {
		disp(MSG_DETAIL,"Cleaning memory for %s", img->filePath.c_str());
		delete img;
		img = NULL;
	}
}

template<typename T>
bool update_param_img(Image<T>*& img, std::string img_path, std::string name, std::set<std::string>& toRefresh, Image<bool>* img_param_mask) {

	toRefresh.erase(name);

	if ( (img==NULL) || ((img!=NULL) && (img->filePath != img_path)) ) {

		delete_img(img);

		if (img_path != "") {

			if (img_param_mask == NULL) {
				disp(MSG_ERROR, "Tracking with parameter images requires parameter mask image");
				return false;
			}

			disp(MSG_INFO, "Reading %s image", name.c_str());

			img = new Image<T>(img_path);

			if(!img->read()) {
				disp(MSG_ERROR, "Can't read image %s",img_path.c_str());
				return false;		
			}

			if (img->numberOfDimensions != 3) {
				disp(MSG_ERROR, "Parameter image is not 3D: %s",img_path.c_str());
				return false;		
			}

			img->setInterpolationMethod(NEAREST);

			toRefresh.insert(name);

		}

	}

	return true;

}