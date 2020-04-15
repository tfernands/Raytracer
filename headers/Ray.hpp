#ifndef RAYHPP
#define RAYHPP

#include "Vec3.hpp"

class Ray{
public:

	Vec3 origin_vec, direction_vec;

	Ray(){}
	Ray(const Vec3& origin, const Vec3& direction){
		this->origin_vec = origin;
		this->direction_vec = direction;
	}
	virtual ~Ray(){}

	Vec3 origin() const {return origin_vec;}
	Vec3 direction() const {return direction_vec;}
	Vec3 point_at_parameter(float t) const {return origin_vec + t*direction_vec; }
};

#endif