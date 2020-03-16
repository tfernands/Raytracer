#include <iostream>
#include <fstream>
#include <random>
#include "Camera.hpp"
#include "Sphere.hpp"
#include "HitableList.hpp"
#include "float.h"

#include "ProgressBar.hpp"

using namespace std;
std::default_random_engine rand_gen;
std::uniform_real_distribution<float> drand48(0.0,1.0);

Vec3 random_in_unit_sphere(){
	Vec3 p;
	do {
		p = 2.0*Vec3(drand48(rand_gen),drand48(rand_gen),drand48(rand_gen)) - Vec3(1,1,1);
	} while (p.squared_length() >= 1.0);
	return p;
}

Vec3 background(const Ray& r){
	Vec3 unit_direction = Vec3::unit_vector(r.direction());
	float t = 0.5*(unit_direction.y() + 1.0);
	return (1.0-t)*Vec3(1, 1, 1) + t*Vec3(0.5, 0.7, 1);
}

Vec3 get_color(const Ray& r, Hitable *world, int depth){
	HitRecord rec;

	if (world->hit(r, 0.001, FLT_MAX, rec) && depth < 10){
		Vec3 udir = Vec3::unit_vector(r.direction());
		Vec3 dir = udir-2*Vec3::dot(udir, rec.normal)*rec.normal + random_in_unit_sphere()/4;
		//Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5*get_color(Ray(rec.p, dir), world, depth+1);
	}
	return background(r);
}

Vec3 get_normal(const Ray& r, Hitable *world, int depth){
	HitRecord rec;
	if (world->hit(r, 0, FLT_MAX, rec)){
		return 0.5*(rec.normal+Vec3(1,1,1));
	}
	return background(r);
}

int main(){
	
	int nx = 500;
	int ny = 250;
	int spp = 100;

	int list_size = 20;
	Hitable *list[list_size];
	list[0] = new Sphere(Vec3(0,-1000.5,-1), 1000);
	for (int i = 1; i < list_size; i++){
		float cx = drand48(rand_gen)*20-10;
		float r = drand48(rand_gen);
		float cz = -drand48(rand_gen)*6-4;
		list[i] = new Sphere(Vec3(cx,0.5-r,cz), r);
	}
	Hitable *world = new HitableList(list, list_size);

	Camera cam;
	ofstream myfile;
	myfile.open ("output/teste.ppm");
	myfile << "P3\n" << nx << " " << ny << "\n255\n";
	ProgressBar progress_total(ny, 30, '=', '.');
	for (int j = ny-1; j >= 0; j--){
		for (int i = 0; i < nx; i++){
			Vec3 color(0, 0, 0);
			for (int s=0; s < spp; s++){
				float u = float(i+drand48(rand_gen))/float(nx);
				float v = float(j+drand48(rand_gen))/float(ny);
				Ray r = cam.get_ray(u, v);
				color += get_color(r, world, 0);
			}
			color /= float(spp);
			color[0] = sqrt(color[0]);
			color[1] = sqrt(color[1]);
			color[2] = sqrt(color[2]);
			int ir = int(255.99*color.r());
			int ig = int(255.99*color.g());
			int ib = int(255.99*color.b());
			myfile << ir <<' '<< ig <<' '<< ib <<endl;
		}
		++progress_total;
		progress_total.display("frame ", "");
	}
	myfile.close();	
	progress_total.done();

	return 0;
}