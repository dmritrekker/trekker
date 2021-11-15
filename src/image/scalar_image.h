#ifndef SRC_IMAGE_SCALAR_IMAGE_H_
#define SRC_IMAGE_SCALAR_IMAGE_H_

#include "image.h"

class SCALAR_Image : public Image {

public:
    SCALAR_Image() {};
	bool  readImage();
};

#endif
