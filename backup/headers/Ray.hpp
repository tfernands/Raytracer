#ifndef RAYHPP
#define RAYHPP

#include "Vec3.hpp"

class Ray{
public:

	Vec3 A, B;

	Ray();
	Ray(const Vec3& a, const Vec3& b);
	virtual ~Ray();

	Vec3 origin() const;
	Vec3 direction() const;
	Vec3 point_at_parameter(float t) const;
};

#endif