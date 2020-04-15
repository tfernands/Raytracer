#ifndef PLANEHPP
#define PLANEHPP

#include "Hitable.hpp"
#include "Material.hpp"

class InfinitPlane: public Hitable{
private:
	Vec3 point, normal;

public:
	Material *material;

	InfinitPlane(){}

	InfinitPlane(Vec3 point, Vec3 normal, Material *mat){
		this->point = point;
		this->normal = normal;
		this->material = mat;
	};

	virtual ~InfinitPlane(){}

	virtual bool bounding_box(AABB& box __attribute__((unused))) const override{
		box = AABB(Vec3(-100, 0, -100), Vec3(100, -1, 100));
		return true;
	}

	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override{
		double t = (Vec3::dot(-normal, r.origin())-Vec3::dot(-normal, point))/Vec3::dot(r.direction(), normal);
		if (t > t_min && t < t_max){
			rec.t = t;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = normal;
			rec.material = material;
			return true;
		}
		return false;
	}
};
#endif