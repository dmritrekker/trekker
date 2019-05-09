#ifndef SRC_IMAGE_FOD_IMAGE_H_
#define SRC_IMAGE_FOD_IMAGE_H_

#include "../config/config_general.h"
#include "../image/image.h"

class FOD_Image : public Image {
public:
	FOD_Image();
	~FOD_Image();
	bool  		 readImage();
	int 		 getSHorder(){return (sqrt(8*nim->nt+1)-3)/2;};
	float 		 getSmallestPixdim();
};

#endif
