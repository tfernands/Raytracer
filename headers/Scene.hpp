#include "Sphere.hpp"
#include "Plane.hpp"
#include "HitableList.hpp"

Hitable* ground_and_sphere(){
	int i = 0; 
	Hitable** list = new Hitable*[2];
	list[i++] = new InfinitPlane(Vec3(0,0,0), Vec3(0,1,0), new Lambertian(Vec3(1,1,1)));
	list[i++] = new Sphere(Vec3(-4,1,0),1, new Metal(Vec3(0.7,0.6,0.5),0));
	return new HitableList(list, i);
}

Hitable* random_scene(){
	int n = 50;
	int i = 0; 
	Hitable** list = new Hitable*[n+1];
	list[i++] = new InfinitPlane(Vec3(0,0,0), Vec3(0,1,0), new Lambertian(Vec3(0.7,0.7,0.7)));
	list[i++] = new Sphere(Vec3(0,1,0),1,new Dielectric(1.3));
	list[i++] = new Sphere(Vec3(4,1,0),1,new Lambertian(Vec3(1,.8,.6)));
	list[i++] = new Sphere(Vec3(-4,1,0),1, new Metal(Vec3(0.7,0.6,0.5),0));
	while(true){
		double choose_mat = random();
		Vec3 center(30*(random()-0.5), 0.2, 30*(random()-0.5));
		if ((center-Vec3(4,0.2,0)).length() > 0.9){
			if (choose_mat < 0.4){
				list[i++] = new Sphere(center, center.y(), new Lambertian());
			}
			else if (choose_mat < 0.75){
				list[i++] = new Sphere(center, center.y(),
					new Metal(Vec3(0.5*(1+random()), 0.5*(1+random()), 0.5*(1+random())),0.5*random()));
			}
			else{
				list[i++] = new Sphere(center, center.y(), new Dielectric(1.5));
			}
			if (i == n){
				return new HitableList(list, i);
			}
		}
	}
	return new HitableList(list, i);
}