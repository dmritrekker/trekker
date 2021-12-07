#ifndef SRC_TRACKER_ALGORITHMS_LOCAL_PROBABILISTIC_STREAMLINE_LOCAL_PROBABILISTIC_H_
#define SRC_TRACKER_ALGORITHMS_LOCAL_PROBABILISTIC_STREAMLINE_LOCAL_PROBABILISTIC_H_

#include "../../base/streamline.h"

class Streamline_Local_Probabilistic : public Streamline {

public :

	Streamline_Local_Probabilistic();
	~Streamline_Local_Probabilistic();

	void flush();
	void flip();
	void push(void *segment);
	void pop_back();

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
