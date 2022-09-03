#ifndef PLANEHPP
#define PLANEHPP

#include "Hitable.hpp"

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

	virtual bool bounding_box(AABB& box __attribute__((unused))) const override {
		return false;
	}

	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
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

class XYRect: public Hitable{
	public:
		Material *material;
		double x0, x1, y0, y1, k;

		XYRect(double x0, double x1, double y0, double y1, double k, Material* mat){
			this->x0 = x0;
			this->x1 = x1;
			this->y0 = y0;
			this->y1 = y1;
			this->k = k;
			this->material = mat;
		}

		virtual bool bounding_box(AABB& box __attribute__((unused))) const override {
			box = AABB(Vec3(x0,y0,k-0.01), Vec3(x1,y1,k+0.01));
			return true;
		}

		virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
			double t = (k-r.origin().z()) / r.direction().z();
			if (t < t_min && t > t_max)
				return false;
			double x = r.origin().x() +t*r.direction().x();
			double y = r.origin().y() +t*r.direction().y();
			if (x < x0 || x > x1 || y < y0 || y > y1)
				return false;
			rec.u = (x-x0)/(x1-x0);
			rec.v = (y-y0)/(y1-y0);
			rec.t = t;
			rec.material = material;
			rec.p = r.point_at_parameter(t);
			rec.normal = Vec3(0, 0, 1);
			return true;
		}
};

class XZRect: public Hitable{
	public:
		Material *material;
		double x0, x1, z0, z1, k;

		XZRect(double x0, double x1, double z0, double z1, double k, Material* mat){
			this->x0 = x0;
			this->x1 = x1;
			this->z0 = z0;
			this->z1 = z1;
			this->k = k;
			this->material = mat;
		}

		virtual bool bounding_box(AABB& box __attribute__((unused))) const override {
			box = AABB(Vec3(x0,k-0.01,z0), Vec3(x1,k+0.01,z1));
			return true;
		}

		virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
			double t = (k-r.origin().y()) / r.direction().y();
			if (t < t_min && t > t_max)
				return false;
			double x = r.origin().x() +t*r.direction().x();
			double z = r.origin().z() +t*r.direction().z();
			if (x < x0 || x > x1 || z < z0 || z > z1)
				return false;
			rec.u = (x-x0)/(x1-x0);
			rec.v = (z-z0)/(z1-z0);
			rec.t = t;
			rec.material = material;
			rec.p = r.point_at_parameter(t);
			rec.normal = Vec3(0, 1, 0);
			return true;
		}
};

class YZRect: public Hitable{
	public:
		Material *material;
		double y0, y1, z0, z1, k;

		YZRect(double y0, double y1, double z0, double z1, double k, Material* mat){
			this->y0 = y0;
			this->y1 = y1;
			this->z0 = z0;
			this->z1 = z1;
			this->k = k;
			this->material = mat;
		}

		virtual bool bounding_box(AABB& box __attribute__((unused))) const override {
			box = AABB(Vec3(k-0.01,y0,z0), Vec3(k+0.01,y1,z1));
			return true;
		}

		virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override {
			double t = (k-r.origin().x()) / r.direction().x();
			if (t < t_min && t > t_max)
				return false;
			double y = r.origin().y() +t*r.direction().y();
			double z = r.origin().z() +t*r.direction().z();
			if (y < y0 || y > y1 || z < z0 || z > z1)
				return false;
			rec.u = (y-y0)/(y1-y0);
			rec.v = (z-z0)/(z1-z0);
			rec.t = t;
			rec.material = material;
			rec.p = r.point_at_parameter(t);
			rec.normal = Vec3(1, 0, 0);
			return true;
		}
};
#endif