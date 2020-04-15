#ifndef MATERIALHPP
#define MATERIALHPP

#include <math.h>
#include "Ray.hpp"
#include "Hitable.hpp"
#include "utils.hpp"

struct HitRecord;


//ray operations
Vec3 reflect(const Vec3 &v, const Vec3 &n){
	return v - 2*Vec3::dot(v, n)*n;
}

bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted){
	Vec3 uv = Vec3::unit_vector(v);
	float dt = Vec3::dot(uv, n);
	float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
	if (discriminant > 0){
		refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
		return true;
	}
	else
		return false;
}

float schlick(float cosine, float ref_idx){
	float r0 = (1-ref_idx)/(1+ref_idx);
	r0 = r0*r0;
	return r0 + (1-r0)*pow((1-cosine),5);
}

class Texture{
public:
	virtual Vec3 value(float u, float v, const Vec3& p) const = 0;
};

class Material{
public:
	bool final_hit = false;
	virtual ~Material(){}
	virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const = 0;
};

class Lambertian: public Material{
public:

	Vec3 albedo;

	Lambertian(){
		this->albedo = Vec3(random()*random(), random()*random(), random()*random());
	}
	Lambertian(const Vec3 &albedo){
		this->albedo = albedo;
	}
	virtual ~Lambertian(){}

	virtual bool scatter(const Ray& r_in __attribute__((unused)), const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const {
		Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = Ray(rec.p, target-rec.p);
		attenuation = albedo;
		return true;
	}
};

class Metal: public Material{
public:
	
	Vec3 albedo;
	float fuzz;

	Metal(const Vec3& a, float f){
		albedo = a;
		if (f < 0) fuzz = 0;
		else if (f < 1) fuzz = f;
		else fuzz = 1;
	}
	virtual ~Metal(){}

	virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const {
		Vec3 reflected = reflect(Vec3::unit_vector(r_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected+fuzz*random_in_unit_sphere());
		attenuation = albedo;
		return (Vec3::dot(scattered.direction(), rec.normal) > 0);
	}
};


class Dielectric: public Material{
public:

	Vec3 transparency;

	float ref_idx;
	Dielectric(float ri){
		transparency = Vec3(1-random()*0.2,1-random()*0.2,1-random()*0.2);
		ref_idx = ri;
	}
	virtual ~Dielectric(){}

	virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const{
		Vec3 outward_normal;
		Vec3 reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = transparency;
		Vec3 refracted;
		float reflect_prob;
		float cosine;
		float dot_d_n = Vec3::dot(r_in.direction(), rec.normal);
		if (dot_d_n > 0){
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot_d_n/r_in.direction().length();
		}
		else{
			outward_normal = rec.normal;
			ni_over_nt = 1/ref_idx;
			cosine = -dot_d_n/r_in.direction().length();
		}
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)){
			reflect_prob = schlick(cosine, ref_idx);
		}
		else{
			reflect_prob = 1.0;
		}
		if (drand48(rand_gen) < reflect_prob){
			scattered = Ray(rec.p, reflected);
		}
		else{
			scattered = Ray(rec.p, refracted);
		}
		return true;
	}
};

#endif