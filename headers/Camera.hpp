#ifndef CAMERAHPP
#define CAMERAHPP

#include "Ray.hpp"
#include "utils.hpp"


class Camera{
public:

	Vec3 top_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 origin;
	Vec3 u, v, w;
	double lens_radius;

	Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, double vfov, double aspect, double aperture, double focus_dist){
		lens_radius = aperture/2;
		double theta = vfov*M_PI/180;
		double half_height = tan(theta/2);
		double half_width = aspect * half_height;
		w = Vec3::unit_vector(lookfrom-lookat);
		u = Vec3::unit_vector(Vec3::cross(vup, w));
		v = Vec3::cross(w, u);
		origin = lookfrom;
		top_left_corner = origin - half_width*focus_dist*u + half_height*focus_dist*v -focus_dist*w;
		horizontal = 2*half_width*focus_dist*u;
		vertical = 2*half_height*focus_dist*v;
	}

	void set(Vec3 lookfrom, Vec3 lookat, Vec3 vup, double vfov, double aspect, double aperture, double focus_dist){
		lens_radius = aperture/2;
		double theta = vfov*M_PI/180;
		double half_height = tan(theta/2);
		double half_width = aspect * half_height;
		w = Vec3::unit_vector(lookfrom-lookat);
		u = Vec3::unit_vector(Vec3::cross(vup, w));
		v = Vec3::cross(w, u);
		origin = lookfrom;
		top_left_corner = origin - half_width*focus_dist*u + half_height*focus_dist*v -focus_dist*w;
		horizontal = 2*half_width*focus_dist*u;
		vertical = 2*half_height*focus_dist*v;
	}

	virtual ~Camera(){}

	Ray get_ray(double s, double t) const{
		Vec3 rd = lens_radius*random_in_unit_disk();
		Vec3 offset = u*rd.x() + v*rd.y();
		Vec3 temp_origin = origin+offset;
		return Ray(temp_origin, top_left_corner+s*horizontal-t*vertical-temp_origin);
	}

};

#endif