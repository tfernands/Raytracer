#ifndef SPHEREHPP
#define SPHEREHPP

#include "Hitable.hpp"
#include "Material.hpp"

class Sphere: public Hitable{
private:
	static constexpr double bbox_pad = 0.0001;

	static void get_uv(const Vec3& p, double& u , double& v){
		double phi = atan2(p.z(), p.x());
		double theta = asin(p.y());
		u = 1-(phi + M_PI) / (2*M_PI);
		v = (theta + M_PI/2) / M_PI;
	}

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
		double b_size = radius + bbox_pad;
		box = AABB(center-Vec3(b_size, b_size, b_size), center+Vec3(b_size, b_size, b_size));
		return true;
	}

	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
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
				get_uv((rec.p-center)/radius, rec.u, rec.v);
				return true;
			}
		}
		return false;
	}
};

#endif