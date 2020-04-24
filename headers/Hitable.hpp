#ifndef HITABLEHPP
#define HITABLEHPP

#include "Ray.hpp"
#include "AABB.hpp"

class Material;

struct HitRecord{
	double t;				//distance traveled to hit point
	double u, v;
	Vec3 p;					//hit point
	Vec3 normal;			//surface normal  
	Material *material; 	//material
};

class Hitable{
	public:
		virtual ~Hitable(){}
		virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
		virtual bool bounding_box(AABB& box) const = 0;
};

class FlipNormals: public Hitable{
	public:
		Hitable* hitable;

		FlipNormals(Hitable* hitable): hitable(hitable){}
		virtual ~FlipNormals(){}

		virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
			if (hitable->hit(r, t_min, t_max, rec)){
				rec.normal = -rec.normal;
				return true;
			}
			return false;
		}

		virtual bool bounding_box(AABB& box) const override {
			return hitable->bounding_box(box);
		}
};

#endif