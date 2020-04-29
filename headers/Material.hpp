#ifndef MATERIALHPP
#define MATERIALHPP

#include <math.h>
#include "Ray.hpp"
#include "Hitable.hpp"
#include "utils.hpp"
#include <stdint.h>

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
	ConstantTexture(Vec3 c): color(c) {}
	virtual ~ConstantTexture() {}
	virtual Vec3 value(double u __attribute__((unused)), double v __attribute__((unused)), const Vec3& p __attribute__((unused))) const override {
		return color;
	}
};

class CheckerTexture: public Texture{
	public:
		Texture* even;
		Texture* odd;
		double scale;
		CheckerTexture(Texture* t0, Texture* t1, double scale): even(t0), odd(t1), scale(scale) {}
		virtual ~CheckerTexture() {}
		virtual Vec3 value(double u __attribute__((unused)), double v __attribute__((unused)), const Vec3& p) const override {
			double sines = sin(scale*p.x())*sin(scale*p.y())*sin(scale*p.z());
			if (sines < 0)
				return odd->value(u, v, p);
			return even->value(u, v, p);
		}
};

class Perlin {
	public:
	static Vec3* ranVec;
	static int* perm_x;
	static int* perm_y;
	static int* perm_z;

	static inline double perlin_interp(Vec3 c[2][2][2], double u, double v, double w){
		double uu = u*u*(3-2*u);
		double vv = v*v*(3-2*v);
		double ww = w*w*(3-2*w);
		double accum = 0;
		for (int i=0; i<2; i++)
			for (int j=0; j<2; j++)
				for (int k=0; k<2; k++){
					Vec3 weight_v(u-i, v-j, w-k);
					accum +=  (i*uu + (1-i)*(1-uu))
							 *(j*vv + (1-j)*(1-vv))
							 *(k*ww + (1-k)*(1-ww))*Vec3::dot(c[i][j][k], weight_v);
				}
		return accum;
	}

	double noise(const Vec3& p) const {
		double u = p.x() - floor(p.x());
		double v = p.y() - floor(p.y());
		double w = p.z() - floor(p.z());
		int i = floor(p.x());
		int j = floor(p.y());
		int k = floor(p.z());
		Vec3 c[2][2][2];
		for (int di=0; di < 2; di++)
			for (int dj=0; dj < 2; dj++)
				for (int dk=0; dk < 2; dk++)
					c[di][dj][dk] = ranVec[perm_x[(i+di) & 255]^perm_y[(j+dj) & 255]^perm_z[(k+dk) & 255]];
		return perlin_interp(c, u, v, w);
	}

	double turb(const Vec3& p, int depth=7) const {
		double accum = 0;
		Vec3 temp_p = p;
		double weight = 1;
		for (int i = 0; i < depth; i++){
			accum += weight*noise(temp_p);
			weight *= 0.5;
			temp_p *= 2;
		}
		return fabs(accum);
	}

	static void permute(int* p, int n){
		for (int i = n-1; i > 0; i--){
			int target = int(random()*(i+1));
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}

	static Vec3* perlin_generate(){
		Vec3* p = new Vec3[256];
		for (int i = 0; i < 256; i++)
			p[i] = Vec3::unit_vector(random_in_unit_sphere());
		return p;
	}

	static int* perlin_generate_perm(){
		int* p= new int[256];
		for (int i = 0; i < 256; i++)
			p[i] = i;
		permute(p, 256);
		return p;
	}
};

Vec3* Perlin::ranVec = Perlin::perlin_generate();
int* Perlin::perm_x = Perlin::perlin_generate_perm();
int* Perlin::perm_y = Perlin::perlin_generate_perm();
int* Perlin::perm_z = Perlin::perlin_generate_perm();

class NoiseTexture: public Texture{
	public:
	double scale;
	Perlin noise;
	NoiseTexture(double scale): scale(scale){}
	virtual ~NoiseTexture(){}
	virtual Vec3 value(double u __attribute__((unused)), double v __attribute__((unused)), const Vec3& p) const {
		return Vec3(1,1,1)*noise.turb(scale*p);
	}
};

class Marble: public Texture{
	public:
	double scale;
	Perlin noise;
	Marble(double scale): scale(scale){}
	virtual ~Marble(){}
	virtual Vec3 value(double u __attribute__((unused)), double v __attribute__((unused)), const Vec3& p) const {
		return Vec3(1,1,1)*0.5*(1+sin(scale*p.z() + 10*noise.turb(p)));
	}
};

class ImageTexture: public Texture{
	public:
		uint8_t* data;
		int nx, ny;
		ImageTexture(uint8_t *pixels, int A, int B): data(pixels), nx(A), ny(B) {}
		virtual ~ImageTexture(){}
		virtual Vec3 value(double u, double v, const Vec3& p __attribute__((unused))) const {
			int i = u*nx;
			int j = (1-v)*ny-0.001;
			if (i < 0) i = 0;
			if (j < 0) j = 0;
			if (i > nx-1) i = nx-1;
			if (j > ny-1) j = ny-1;
			double r = int(data[3*nx*j + i*3+0])/255.0;
			double g = int(data[3*nx*j + i*3+1])/255.0;
			double b = int(data[3*nx*j + i*3+2])/255.0;
			return Vec3(r, g, b);
		}
};

class InvertTexture: public Texture{
	public:
		Texture* texture;
		InvertTexture(Texture* texture): texture(texture) {}
		virtual Vec3 value(double u, double v, const Vec3& p) const {
			return Vec3(1,1,1)-texture->value(u,v,p);
		}
};

class MultplyTexture: public Texture{
	public:
		Texture *tex1, *tex2;
		MultplyTexture(Texture* tex1, Texture* tex2): tex1(tex1), tex2(tex2) {}
		virtual Vec3 value(double u, double v, const Vec3& p) const {
			return tex1->value(u,v,p)*tex2->value(u,v,p);
		}
};

class AddTexture: public Texture{
	public:
		Texture *tex1, *tex2;
		AddTexture(Texture* tex1, Texture* tex2): tex1(tex1), tex2(tex2) {}
		virtual Vec3 value(double u, double v, const Vec3& p) const {
			return tex1->value(u,v,p)+tex2->value(u,v,p);
		}
};

class BWTexture: public Texture{
	public:
		Texture *tex;
		BWTexture(Texture* tex): tex(tex) {}
		virtual Vec3 value(double u, double v, const Vec3& p) const {
			double value = tex->value(u,v,p).mean();
			return Vec3(value, value, value);
		}
};

class SkyMapTexture: public Texture{
	public:
		SkyMapTexture() {}
		virtual Vec3 value(double u __attribute__((unused)), double v, const Vec3& p __attribute__((unused))) const {
			Vec3 sky = (1.0-v)*Vec3(1, 0.7, 0.6)+v*Vec3(0.6, 0.7, 1);
			return sky;
		}
};

// ================== Materials =======================

class Material{
	public:
	virtual ~Material(){}
	virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const = 0;
	virtual Vec3 emitted(float u __attribute__((unused)), float v __attribute__((unused)), const Vec3& p __attribute__((unused))) const {
		return Vec3(0, 0, 0);
	}
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
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}
};

class Metal: public Material{
	public:
	
	Texture* albedo;
	Texture* fuzz;

	Metal(Vec3 albedo, double fuzz){
		this->albedo = new ConstantTexture(albedo);
		this->fuzz = new ConstantTexture(Vec3(fuzz, fuzz, fuzz));
	}

	Metal(Texture* albedo, Texture* fuzz){
		this->albedo = albedo;
		this->fuzz = fuzz;
	}
	virtual ~Metal(){}

	virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const {
		Vec3 reflected = reflect(Vec3::unit_vector(r_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected+fuzz->value(rec.u, rec.v, rec.p).clip(0, 1)*random_in_unit_sphere());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
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
			Vec3 reflected = reflect(r_in.direction(), rec.normal);
			scattered = Ray(rec.p, reflected);
		}
		else{
			scattered = Ray(rec.p, refracted);
		}
		return true;
	}
};

class DiffuseLight: public Material{
	public:
		Texture* emit;
		DiffuseLight(Texture* a): emit(a) {}
		virtual ~DiffuseLight() {}
		virtual bool scatter(const Ray& r_in __attribute__((unused)), const HitRecord& rec __attribute__((unused)), Vec3& attenuation __attribute__((unused)), Ray& scattered __attribute__((unused))) const {
			return false;
		}
		virtual Vec3 emitted(float u, float v, const Vec3& p) const {
			return emit->value(u, v, p);
		}
};

class PortalMat: public Material{
	public:
		Vec3 loc_1;
		Vec3 loc_2;
		Vec3 albedo;
		PortalMat(Vec3 loc_1, Vec3 loc_2, Vec3 albedo): loc_1(loc_1), loc_2(loc_2), albedo(albedo) {}
		virtual ~PortalMat(){}
		virtual bool scatter(const Ray &r_in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const{
			Vec3 origin;
			double ref_idx = 2;
			double dot_d_n = Vec3::dot(r_in.direction(), rec.normal);
			double cosine;
			if (dot_d_n > 0){
				origin = loc_2+(rec.p-loc_1);//rec.p;
				cosine = ref_idx * dot_d_n/r_in.direction().length();
			}
			else{
				origin = loc_2+(rec.p-loc_1);
				cosine = -dot_d_n/r_in.direction().length();
			}

			double reflect_prob = schlick(cosine, ref_idx);
			if (drand48(rand_gen) < reflect_prob){
				attenuation = albedo;
			}
			else{
				attenuation = Vec3(1,1,1);
			}
			scattered = Ray(origin, r_in.direction());
			return true;
		}
};

#endif