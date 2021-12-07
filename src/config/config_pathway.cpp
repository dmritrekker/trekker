#include "config_pathway.h"

using namespace GENERAL;

namespace PATHWAY {

std::vector<ROI_Image*> img_ROI;
std::vector<ROI_Image*> order_of_ROIs;
std::vector<ROI_Image*> order_of_side_A_ROIs;
std::vector<ROI_Image*> order_of_side_B_ROIs;
Requirement_Order satisfy_requirements_in_order 	= REQUIREMENT_ORDER_NOTSET;

void cleanConfigROI() {

	for (std::vector<ROI_Image*>::iterator it = img_ROI.begin(); it != img_ROI.end(); ++it)
		delete *it;
    
    img_ROI.clear();
    order_of_ROIs.clear();
    order_of_side_A_ROIs.clear();
    order_of_side_B_ROIs.clear();

}

void readROIImages() {

	for (std::vector<ROI_Image*>::iterator it = img_ROI.begin(); it != img_ROI.end(); ++it) {
		if (GENERAL::verboseLevel!=QUITE) std::cout << "Reading pathway image              : " << (*it)->getFilePath() << std::endl;
		if(!(*it)->readImage()) exit(EXIT_FAILURE);
	}

}

void print() {
	std::cout << std::endl;
	std::cout << "PATHWAY OPTIONS"<< std::endl;

	if (img_ROI.size()==0) {
		std::cout << "Not specified" << std::endl;
	} else {

		for (std::vector<ROI_Image*>::iterator it = img_ROI.begin(); it != img_ROI.end(); ++it) {
			switch ((*it)->type) {
			case roi_type_req_entry: 				std::cout << "require_entry          "; break;
			case roi_type_req_exit: 				std::cout << "require_exit           "; break;
			case roi_type_stop_at_entry:			std::cout << "stop_at_entry          "; break;
			case roi_type_stop_at_exit: 			std::cout << "stop_at_exit           "; break;
			case roi_type_discard_if_enters: 		std::cout << "discard_if_enters      "; break;
			case roi_type_discard_if_exits: 		std::cout << "discard_if_exits       "; break;
			case roi_type_discard_if_ends_inside: 	std::cout << "discard_if_ends_inside "; break;
			default: break;
			}

			switch ((*it)->side) {
			case side_A: std::cout << "(A) : "; break;
			case side_B: std::cout << "(B) : "; break;
			default:     std::cout << "    : "; break;
			}

			int label;
			if ((*it)->getLabel(label))
				std::cout << "(using label " << label << ") ";

			std::cout << (*it)->getFilePath() << std::endl;

			if (GENERAL::verboseLevel>ON) std::cout << std::endl << "-----------------" << std::endl;
			if (GENERAL::verboseLevel>ON) (*it)->printInfo();
			if (GENERAL::verboseLevel>ON) std::cout << "-----------------" << std::endl << std::endl;
		}

	}
}

void checkROIorderConsistency() {
	if ((!order_of_side_A_ROIs.empty() || !order_of_side_B_ROIs.empty()) && (!order_of_ROIs.empty())) {
		std::cout << "In order to use the pathway rules with sides, sides for all pathway images must be defined" << std::endl;
		exit(EXIT_FAILURE);
	}

	if ((!order_of_side_A_ROIs.empty() || !order_of_side_B_ROIs.empty()) && (TRACKER::directionality==ONE_SIDED)) {
		std::cout << "In order to use the pathway rules with sides, directionally must be \"two-sided\" " << std::endl;
		exit(EXIT_FAILURE);
	}

	if ((satisfy_requirements_in_order==IN_ORDER) && (order_of_side_A_ROIs.empty() && order_of_side_B_ROIs.empty())) {
		std::cout << "In order to use the -pathway=satisfy_requirements_in_order option, sides for all pathway images must be defined" << std::endl;
		exit(EXIT_FAILURE);
	}
}

}
