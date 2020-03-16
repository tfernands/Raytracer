#ifndef SPHEREHPP
#define SPHEREHPP

#include "Hitable.hpp"

class Sphere: public Hitable{
public:
	Vec3 center;
	float radius;

	Sphere(){}
	Sphere(Vec3 cen, float r){
		center = cen;
		radius = r;
	};
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const{
	Vec3 oc = r.origin() - center;
	float a = Vec3::dot(r.direction(), r.direction());
	float b = Vec3::dot(oc, r.direction());
	float c = Vec3::dot(oc, oc) - radius*radius;
	float delta = b*b - a*c;
	if (delta > 0){
		float temp = (-b -sqrt(delta))/a;
		if (temp > t_max || temp < t_min)
			temp = (-b +sqrt(delta))/a;
		if (temp < t_max && temp > t_min){
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center)/radius;
			return true;
		}
	}
	return false;
}

#endif