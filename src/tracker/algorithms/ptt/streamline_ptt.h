#ifndef SRC_TRACKER_ALGORITHMS_PTT_STREAMLINE_PTT_H_
#define SRC_TRACKER_ALGORITHMS_PTT_STREAMLINE_PTT_H_

#include "../../base/streamline.h"
#include "PTF.h"

class Streamline_PTT : public Streamline {

public:

	Streamline_PTT();
	~Streamline_PTT();

	void flush();
	void push(void *segment);
	void pop_back();
	void flip();

	std::vector<Coordinate> tangent;
	std::vector<Coordinate> k1axis;
	std::vector<Coordinate> k2axis;
	std::vector<float>      k1;
	std::vector<float>      k2;
	std::vector<float>      curvature;
	std::vector<float>      likelihood;

	size_t sampling_prop_postEstItCount;
	size_t sampling_prop_generated;
	size_t sampling_prop_fail;
	size_t sampling_prop_tries;
	size_t sampling_prop_reject;

	size_t sampling_init_postEstItCount;
	size_t sampling_init_generated;
	size_t sampling_init_fail;
	size_t sampling_init_tries;
	size_t sampling_init_reject;

};



#endif
