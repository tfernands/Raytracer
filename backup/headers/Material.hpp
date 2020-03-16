#ifndef MATERIALHPP
#define MATERIALHPP

#include "Ray.hpp"
#include "Hitable.hpp"

struct HitRecord;

std::default_random_engine rand_gen;
std::uniform_real_distribution<float> drand48(0.0,1.0);

Vec3 random_in_unit_sphere(){
	Vec3 p;
	do {
		p = 2.0*Vec3(drand48(rand_gen),drand48(rand_gen),drand48(rand_gen)) - Vec3(1,1,1);
	} while (p.squared_length() >= 1.0);
	return p;
}

Vec3 reflect(const Vec3 &v, const Vec3 &n){
	return v - 2*Vec3::dot(v, n)*n;
}

class Material{
public:
	virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const = 0;
};

class Lambertian: public Material{
public:

	Vec3 albedo;

	Lambertian(const Vec3& a): albedo(a){}
	virtual bool scatter(const Ray& r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const {
		Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = Ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}
};

class Metal: public Material{
public:
	
	Vec3 albedo;

	Metal(const Vec3& a): albedo(a){}
	virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const {
		Vec3 reflected = reflect(Vec3::unit_vector(r_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected);
		attenuation = albedo;
		return (Vec3::dot(scattered.direction(), rec.normal) > 0);
	}
};

#endif