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

Vec3 get_color(const Ray& r, Hitable *world){
	HitRecord rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)){
		Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5*get_color(Ray(rec.p, target -rec.p), world);
	}
	Vec3 unit_direction = Vec3::unit_vector(r.direction());
	float t = 0.5*(unit_direction.y() + 1.0);
	return (1.0-t)*Vec3(1, 1, 1) + t*Vec3(0.5, 0.7, 1);
}

int main(){

	ofstream myfile;
	
	int nx = 200;
	int ny = 100;
	int spp = 1;

	int list_size = 10;
	Hitable *list[list_size];

	list[0] = new Sphere(Vec3(0,-1000.5,-1), 1000);
	for (int i = 1; i < list_size; i++){
		float cx = drand48(rand_gen)*100;
		float r = drand48(rand_gen)*3;
		float cz = -drand48(rand_gen)*100;
		list[i] = new Sphere(Vec3(cx,r,cz), r);
	}
	Hitable *world = new HitableList(list, list_size);
	Camera cam;
	for (int y = 0; y < 100; y++){
		ProgressBar progressBar(100, 70, '#', '-');
		++progressBar; // record the tick
		char name[50];
		sprintf(name, "output/sequence_%05d.ppm", y);
		myfile.open (name);
		((Sphere*)(list[1]))->center[1] = 2.0*y/100.0;
		myfile << "P3\n" << nx << " " << ny << "\n255\n";
		for (int j = 0; j < ny; j++){
			for (int i = 0; i < nx; i++){
				Vec3 color(0, 0, 0);
				for (int s=0; s < spp; s++){
					float u = float(i+drand48(rand_gen))/float(nx);
					float v = float(j+drand48(rand_gen))/float(ny);
					Ray r = cam.get_ray(u, v);
					Vec3 p = r.point_at_parameter(2.0);
					color += get_color(r, world);
				}
				color /= float(spp);
				color[0] = sqrt(color[0]);
				color[1] = sqrt(color[1]);
				color[2] = sqrt(color[2]);
				int ir = int(255.99*color.r());
				int ig = int(255.99*color.g());
				int ib = int(255.99*color.b());
			}
		}
		progressBar.display();
		myfile.close();	
	}

	delete []list;
	return 0;
}