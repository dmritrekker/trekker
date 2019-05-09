#ifndef SRC_CONFIG_CONFIG_PATHWAY_H_
#define SRC_CONFIG_CONFIG_PATHWAY_H_

#include <iostream>
#include <vector>

#include "../image/roi_image.h"
#include "config_general.h"
#include "config_tracker.h"

typedef enum {
	REQUIREMENT_ORDER_NOTSET = -1,
	NO_ORDER,
	IN_ORDER
} Requirement_Order;

namespace PATHWAY {

extern std::vector<ROI_Image*> img_ROI;

extern Requirement_Order       satisfy_requirements_in_order;
extern std::vector<ROI_Image*> order_of_ROIs;
extern std::vector<ROI_Image*> order_of_side_A_ROIs;
extern std::vector<ROI_Image*> order_of_side_B_ROIs;


void cleanConfigROI();
void readROIImages();
void checkROIorderConsistency();
void print();

}

#endif
