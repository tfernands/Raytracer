#include "Sphere.hpp"
#include "Plane.hpp"
#include "HitableList.hpp"
#include "HitableList.hpp"

Hitable* ground_and_sphere(){
	int i = 0;
	Hitable** list_spheres = new Hitable*[2];
	Texture* constColor = new ConstantTexture(Vec3(0.7,0.7,0.7));
	list_spheres[i++] = new Sphere(Vec3(0,-1000,0), 1000, new Lambertian(constColor));
	list_spheres[i++] = new Sphere(Vec3(-4,1,0),1, new Lambertian(constColor));
	return new HitableList(list_spheres, i);
}

Hitable* two_sphere(){
	int i = 0;
	Hitable** list_spheres = new Hitable*[2];
	Texture* checkerTexture = new CheckerTexture(new ConstantTexture(Vec3(0.2,.3,.1)), new ConstantTexture(Vec3(0.9,.9,.9)));
	list_spheres[i++] = new Sphere(Vec3(0,-10,0),10, new Lambertian(checkerTexture));
	list_spheres[i++] = new Sphere(Vec3(0,10,0),10, new Lambertian(checkerTexture));
	return new HitableList(list_spheres, i);
}

Hitable* two_perlin_spheres(){
	int i = 0;
	Hitable** list_spheres = new Hitable*[2];
	Texture* pertext = new NoiseTexture(5);
	list_spheres[i++] = new Sphere(Vec3(0,-1000,0),1000, new Lambertian(pertext));
	list_spheres[i++] = new Sphere(Vec3(0,2,0),2, new Lambertian(pertext));
	return new HitableList(list_spheres, i);
}

Hitable* random_scene(int n){
	int i = 0; 
	Hitable** list_spheres = new Hitable*[n+1];
	Texture* checkerTexture = new CheckerTexture(new ConstantTexture(Vec3(0.2,.3,.1)), new ConstantTexture(Vec3(0.9,.9,.9)));
	Hitable* ground_plane = new Sphere(Vec3(0,-10000,0), 10000, new Lambertian(checkerTexture));
	list_spheres[i++] = new Sphere(Vec3(0,1,0),1,new Dielectric(1.3));
	list_spheres[i++] = new Sphere(Vec3(4,1,0),1,new Lambertian(Vec3(0.95,.95,.95)));
	list_spheres[i++] = new Sphere(Vec3(-4,1,0),1, new Metal(Vec3(0.7,0.6,0.5),0));
	while(true){
		double choose_mat = random();
		Vec3 center(40*(random()-0.5), random()*0.3+0.07, 40*(random()-0.5));
		if ((center-Vec3(4,0.2,0)).length() > 0.9){
			if (choose_mat < 0.4){
				list_spheres[i++] = new Sphere(center, center.y(), new Lambertian());
			}
			else if (choose_mat < 0.75){
				list_spheres[i++] = new Sphere(
					center, center.y(),
					new Metal(Vec3(0.5*(1+random()), 0.5*(1+random()), 0.5*(1+random())),0.3*random())
				);
			}
			else{
				list_spheres[i++] = new Sphere(center, center.y(), new Dielectric(1.5));
			}
			if (i == n){
				break;
			}
		}
	}
	Hitable** final_list = new Hitable*[2];
	final_list[0] = ground_plane;
	final_list[1] = new BVHNode(list_spheres, i, 0);
	return new HitableList(final_list, 2);
}