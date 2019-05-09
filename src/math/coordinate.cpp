#include "coordinate.h"

Coordinate::Coordinate() {
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

Coordinate::~Coordinate() { }

Coordinate::Coordinate(float _x, float _y, float _z) {
	x=_x;
	y=_y;
	z=_z;
}


Coordinate::Coordinate( const Coordinate& obj) {
	x = obj.x;
	y = obj.y;
	z = obj.z;
}

void Coordinate::print() {
	std::cout << "[" << x << "," << y << "," << z << "]";
}

Coordinate& Coordinate::operator= (const Coordinate& obj) {
	x = obj.x;
	y = obj.y;
	z = obj.z;
	return *this;
}


const Coordinate Coordinate::operator+ (const Coordinate& obj) const {
	Coordinate out;
	out.x = x + obj.x;
	out.y = y + obj.y;
	out.z = z + obj.z;
	return out;
}

const Coordinate Coordinate::operator- (const Coordinate& obj) const {
	Coordinate out;
	out.x = x - obj.x;
	out.y = y - obj.y;
	out.z = z - obj.z;
	return out;
}

const Coordinate Coordinate::operator* (const float scale) const {
	Coordinate out;
	out.x = x*scale;
	out.y = y*scale;
	out.z = z*scale;
	return out;
}


float Coordinate::norm() {
	return std::sqrt(x*x+y*y+z*z);
}

void Coordinate::normalize() {
	float magi = 1.0/this->norm();
	x *= magi;
	y *= magi;
	z *= magi;
}
