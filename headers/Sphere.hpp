#ifndef SPHEREHPP
#define SPHEREHPP

#include "Hitable.hpp"
#include "Material.hpp"

class Sphere: public Hitable{
public:
	Vec3 center;
	float radius;
	Material *material;

	Sphere(){}

	Sphere(Vec3 cen, float r, Material *mat){
		center = cen;
		radius = r;
		material = mat;
	}

	virtual ~Sphere(){}

	virtual bool bounding_box(AABB& box) const override{
		box = AABB(center-Vec3(radius, radius, radius), center+Vec3(radius, radius, radius));
		return true;
	}

	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override{
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
				rec.material = material;
				return true;
			}
		}
		return false;
	}
};

#endif