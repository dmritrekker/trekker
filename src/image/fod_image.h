#ifndef SRC_IMAGE_FOD_IMAGE_H_
#define SRC_IMAGE_FOD_IMAGE_H_

#include "../config/config_general.h"
#include "../image/image.h"

class FOD_Image : public Image {
public:
	FOD_Image();
	~FOD_Image();
	bool  		 readImage();
	int 		 getSHorder();
	float 		 getSmallestPixdim();

	bool 		 iseven; // True if it is even ordered
};

#endif
