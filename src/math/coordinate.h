#ifndef SRC_MATH_COORDINATE_H_
#define SRC_MATH_COORDINATE_H_

#include <iostream>
#include <stdlib.h>
#include <cmath>

class Coordinate {
public:

	Coordinate();
	Coordinate(const Coordinate& obj);
	Coordinate(float _x, float _y, float _z);
	~Coordinate();

	Coordinate& operator= (const Coordinate& obj);
	const Coordinate operator+ (const Coordinate& obj) const;
	const Coordinate operator- (const Coordinate& obj) const;
	const Coordinate operator* (const float scale) const;

	float norm();
	void  normalize();
	void  print();

	float x;
	float y;
	float z;
};

#endif
