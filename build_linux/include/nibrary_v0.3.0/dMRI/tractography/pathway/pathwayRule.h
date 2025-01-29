#pragma once

#include "base/nibr.h"
#include "image/image.h"
#include "surface/surface.h"

namespace NIBR
{

	typedef enum {
		undef_src,
		res_pnt_src,  	// Used for seeding purposes only. Reserved for a random single point. 
		sph_src,		// Sphere
		img_mask_src,	// Image - mask
		img_label_src,	// Image - label
		img_pvf_src,	// Image - partial volume
		surf_src,		// Surf
	} Pathway_Src;

	typedef enum {
		undef_type,
		seed,
		discard_seed,	 		 // seed can't be inside
		req_entry,               // must enter
		req_exit,				 // must enter and exit
		req_end_inside,			 // must end inside or nearby
		stop_before_entry, 	     // if enters then stops before entry; 	the last point is inside the ROI.
		stop_at_entry, 			 // if enters then stops at entry; 		the last point is at the ROI border.
		stop_after_entry, 		 // if enters then stops after entry; 	the last point is outside the ROI.
		stop_before_exit,		 // if exits  then stops before exit;  	the last point is inside the ROI.
		stop_at_exit,			 // if exits  then stops at exit;  		the last point is at the ROI border.
		stop_after_exit,		 // if exits  then stops after exit;  	the last point is outside the ROI.
		discard_if_enters,       // must NOT enter
		discard_if_exits,        // if enters then it must NOT exit
		discard_if_ends_inside   // if enters then it must exit
	} Pathway_Type;

	typedef enum {
		either = -1,
		side_A,
		side_B
	} Tracking_Side;

	typedef enum {
		notEnteredYet,
		entered,
		notExitedYet,
		exited,
	} Entry_Status;

	typedef enum {
		surf_useDim_unknown,
		surf_useDim_2D,
		surf_useDim_3D,
	} Surf_Dim;

	struct PathwayRule {
		Pathway_Src		  				 src{undef_src};
		Pathway_Type      				 type{undef_type};
		Tracking_Side     				 side{either};
		Tracking_Side     				 orig_side{either};					// keep orig side for reverting B_pulling
		bool              				 useLabel{false};					// img_label_src, img_pvf_src, surf_src
		int               				 label{0};							// label, volume, label
		float			  				 center[3]{NAN,NAN,NAN};
		float             				 radius{NAN};
		std::string       				 pntListFile{""};
		std::string       				 dirListFile{""};
		std::string      	 			 imageMaskSource{""};
		std::string      	 			 imageLabelSource{""};
		std::string       				 imagePvfSource{""};
		std::string       				 surfaceSource{""};
		float             				 surfaceDiscretizationRes{1};
		std::string     				 surfaceFieldFile4FaceMask{""};
		std::string       				 surfaceFieldFile4VertMask{""};
		std::string       				 surfaceFieldFile4MaskDtype{""};
		std::string       				 surfaceFieldName4Mask{""};
		bool              				 surfaceUseDisc{false};
		float			  				 surfaceDiscCenter[3]{NAN,NAN,NAN};
		float             				 surfaceDiscRadius{NAN};
		std::string       				 surfaceFieldFile4FaceDens{""};
		std::string       				 surfaceFieldFile4VertDens{""};
		std::string       				 surfaceFieldFile4DensDtype{""};
		std::string       				 surfaceFieldName4Dens{""};
		std::string       				 surfaceFieldFile4FaceData{""};
		std::string       				 surfaceFieldFile4VertData{""};
		std::string       				 surfaceFieldFile4DataDtype{""};
		std::string       				 surfaceFieldName4Data{""};
		bool              				 surface4SeedUseNormForDir{false};
		Surf_Dim              		     surfaceUseDim{surf_useDim_unknown};
		bool                             surfaceFlipNormals{false};
		std::vector<std::vector<float>>* data{NULL};
		int                              uniqueId{0}; 						// This is assigned internally when rule is added using pathwayAdd
		Surface*                         surfSrc{NULL};
	};

}
