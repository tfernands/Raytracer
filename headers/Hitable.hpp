#ifndef HITABLEHPP
#define HITABLEHPP

#include "Ray.hpp"

class Material;

struct HitRecord{
	double t;				//distance traveled to hit point
	Vec3 p;					//hit point
	Vec3 normal;			//surface normal  
	Material *material; 	//material
};

class Hitable{
public:
	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
	virtual ~Hitable(){}
};

#endif