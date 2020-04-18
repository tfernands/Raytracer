#ifndef MATERIALHPP
#define MATERIALHPP

#include <math.h>
#include "Ray.hpp"
#include "Hitable.hpp"
#include "utils.hpp"

struct HitRecord;

// ============= UTIL ==================

Vec3 reflect(const Vec3 &v, const Vec3 &n){
	return v - 2*Vec3::dot(v, n)*n;
}

bool refract(const Vec3& v, const Vec3& n, double ni_over_nt, Vec3& refracted){
	Vec3 uv = Vec3::unit_vector(v);
	double dt = Vec3::dot(uv, n);
	double discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
	if (discriminant > 0){
		refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
		return true;
	}
	else
		return false;
}

double schlick(double cosine, double ref_idx){
	double r0 = (1-ref_idx)/(1+ref_idx);
	r0 = r0*r0;
	return r0 + (1-r0)*pow((1-cosine),5);
}

// ================== TEXTURES =======================

class Texture{
	public:
	virtual ~Texture(){}
	virtual Vec3 value(double u, double v, const Vec3& p) const = 0;
};

class ConstantTexture: public Texture{
	public:
	Vec3 color;
	ConstantTexture() {}
	ConstantTexture(Vec3 c): color(c) {}
	virtual Vec3 value(double u __attribute__((unused)), double v __attribute__((unused)), const Vec3& p __attribute__((unused))) const override {
		return color;
	}
};

class CheckerTexture: public Texture{
	public:
		Texture* even;
		Texture* odd;
		CheckerTexture() {}
		CheckerTexture(Texture* t0, Texture* t1): even(t0), odd(t1) {}
		virtual ~CheckerTexture() {}
		virtual Vec3 value(double u __attribute__((unused)), double v __attribute__((unused)), const Vec3& p) const override {
			double sines = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
			if (sines < 0)
				return odd->value(u, v, p);
			return even->value(u, v, p);
		}
};

class Perlin {
	public:
	static double* ranDouble;
	static int* perm_x;
	static int* perm_y;
	static int* perm_z;

	// Perlin(){
	// 	ranDouble = Perlin::perlin_generate();
	// 	perm_x = Perlin::perlin_generate_perm();
	// 	perm_y = Perlin::perlin_generate_perm();
	// 	perm_z = Perlin::perlin_generate_perm();
	// }

	double noise(const Vec3& p) {
		double u = p.x() - floor(p.x());
		double v = p.y() - floor(p.y());
		double w = p.z() - floor(p.z());
		int i = int(4*p.x()) & 255;
		int j = int(4*p.y()) & 255;
		int k = int(4*p.z()) & 255;
		return ranDouble[perm_x[i]^perm_y[j]^perm_z[k]];
	}

	static void permute(int* p, int n){
		for (int i = n-1; i > 0; i--){
			int target = int(random()*(i+1));
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}

	static double* perlin_generate(){
		double* p = new double[256];
		for (int i = 0; i < 256; i++){
			p[i] = random();
		}
		return p;
	}

	static int* perlin_generate_perm(){
		int* p= new int[256];
		for (int i = 0; i < 256; i++){
			p[i] = i;
		}
		permute(p, 256);
		return p;
	}

};

double* Perlin::ranDouble = Perlin::perlin_generate();
int* Perlin::perm_x = Perlin::perlin_generate_perm();
int* Perlin::perm_y = Perlin::perlin_generate_perm();
int* Perlin::perm_z = Perlin::perlin_generate_perm();

class NoiseTexture: public Texture{
public:
	Perlin* noise;
	NoiseTexture(){
		noise = new Perlin();
	}
	virtual ~NoiseTexture(){}
	virtual Vec3 value(double u __attribute__((unused)), double v __attribute__((unused)), const Vec3& p) const {
		return Vec3(1,1,1)*noise->noise(p);
	}
};
// ================== Materials =======================

class Material{
	public:
	bool final_hit = false;
	virtual ~Material(){}
	virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const = 0;
};

class Lambertian: public Material{
	public:
	Texture* albedo;

	Lambertian(){
		albedo = new ConstantTexture(Vec3(random()*0.9, random()*0.9, random()*0.9));
	}
	Lambertian(Vec3 albedo){
		this->albedo = new ConstantTexture(albedo);
	}
	Lambertian(Texture* albedo): albedo(albedo){}
	virtual ~Lambertian(){}

	virtual bool scatter(const Ray& r_in __attribute__((unused)), const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const {
		Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = Ray(rec.p, target-rec.p);
		attenuation = albedo->value(0, 0, rec.p);
		return true;
	}
};

class Metal: public Material{
	public:
	
	Vec3 albedo;
	double fuzz;

	Metal(const Vec3& a, double f){
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

	double ref_idx;
	Dielectric(double ri){
		transparency = Vec3(1-random()*0.2,1-random()*0.2,1-random()*0.2);
		ref_idx = ri;
	}
	virtual ~Dielectric(){}

	virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const{
		Vec3 outward_normal;
		Vec3 reflected = reflect(r_in.direction(), rec.normal);
		double ni_over_nt;
		attenuation = transparency;
		Vec3 refracted;
		double reflect_prob;
		double cosine;
		double dot_d_n = Vec3::dot(r_in.direction(), rec.normal);
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