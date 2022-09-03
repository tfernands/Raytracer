#ifndef CAMERAHPP
#define CAMERAHPP

#include <iostream>
#include "Vec3.hpp"
#include "Ray.hpp"
#include "utils.hpp"

class Camera{
	private:
		Vec3 vup;
		double fov, aspect, focus_dist;

	public:
		Vec3 top_left_corner;
		Vec3 horizontal;
		Vec3 vertical;
		Vec3 origin;
		Vec3 u, v, w;
		double lens_radius;

		Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, double vfov, double aspect, double aperture, double focus_dist){
			this->fov = vfov;
			this->aspect = aspect;
			this->vup = vup;
			this->focus_dist = focus_dist;
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

		void set(Vec3 lookfrom, Vec3 lookat, Vec3 vup, double vfov, double aspect, double aperture, double focus_dist){
			this->fov = vfov;
			this->aspect = aspect;
			this->vup = vup;
			this->focus_dist = focus_dist;
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
		
		double get_fov(){
			return fov;
		}

		double get_aspect(){
			return aspect;
		}

		double get_aperture(){
			return lens_radius*2;
		}

		double get_focus_dist(){
			return focus_dist;
		}

		Vec3 get_vup(){
			return vup;
		}


		Ray get_ray(double s, double t) const{
			if (lens_radius == 0){
				return Ray(origin, top_left_corner+s*horizontal-t*vertical-origin);
			}
			Vec3 rd = lens_radius*random_in_unit_disk();
			Vec3 offset = u*rd.x() + v*rd.y();
			Vec3 temp_origin = origin+offset;
			return Ray(temp_origin, top_left_corner+s*horizontal-t*vertical-temp_origin);
		}


		friend std::ostream& operator<<(std::ostream &out, Camera &c){
			out << "origin: " << c.origin << " | w: " << c.w << " | u: " << c.u << " | v: " << c.v << " | FOV: " << c.fov << " | Focus: " << c.focus_dist;
			return out;
		}

};

#endif