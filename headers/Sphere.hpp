#ifndef SPHEREHPP
#define SPHEREHPP

#include "Hitable.hpp"
#include "Material.hpp"

class Sphere: public Hitable{
public:
	Vec3 center;
	double radius;
	Material *material;

	Sphere(){}

	Sphere(Vec3 cen, double r, Material *mat){
		center = cen;
		radius = r;
		material = mat;
	}

	virtual ~Sphere(){}

	virtual bool bounding_box(AABB& box) const override{
		box = AABB(center-Vec3(radius, radius, radius), center+Vec3(radius, radius, radius));
		return true;
	}

	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override{
		Vec3 oc = r.origin() - center;
		double a = Vec3::dot(r.direction(), r.direction());
		double b = Vec3::dot(oc, r.direction());
		double c = Vec3::dot(oc, oc) - radius*radius;
		double delta = b*b - a*c;
		if (delta > 0){
			double temp = (-b -sqrt(delta))/a;
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