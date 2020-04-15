#ifndef HITABLEHPP
#define HITABLEHPP

#include "Ray.hpp"
#include "AABB.hpp"

class Material;

struct HitRecord{
	float t;				//distance traveled to hit point
	Vec3 p;					//hit point
	Vec3 normal;			//surface normal  
	Material *material; 	//material
};

class Hitable{
public:
	virtual ~Hitable(){}
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
	virtual bool bounding_box(AABB& box) const = 0;
};

#endif