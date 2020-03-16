#ifndef CAMERAHPP
#define CAMERAHPP

#include "Ray.hpp"

class Camera{
public:

	Vec3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 origin;

	Camera(){
		lower_left_corner = Vec3(-2,-1,-2);
		horizontal = Vec3(4,0,0);
		vertical = Vec3(0,2,0);
		origin = Vec3(0,0,0);
	}

	Ray get_ray(float u, float v){
		return Ray(origin, lower_left_corner+u*horizontal+v*vertical-origin);
	}

};

#endif