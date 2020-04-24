#include "Sphere.hpp"
#include "Plane.hpp"
#include "HitableList.hpp"
#include "Material.hpp"

#include "bmphelper.hpp"

Hitable* ground_and_sphere(){
	int i = 0;
	Hitable** list = new Hitable*[2];
	Texture* constColor = new ConstantTexture(Vec3(0.7,0.7,0.7));
	list[i++] = new Sphere(Vec3(0,-1000,0), 1000, new Lambertian(constColor));
	list[i++] = new Sphere(Vec3(-4,1,0),1, new Lambertian(constColor));
	return new HitableList(list, i);
}

Hitable* two_sphere(){
	int i = 0;
	Hitable** list = new Hitable*[2];
	Texture* checkerTexture = new CheckerTexture(new ConstantTexture(Vec3(0.2,.3,.1)), new ConstantTexture(Vec3(0.9,.9,.9)));
	list[i++] = new Sphere(Vec3(0,-10,0),10, new Lambertian(checkerTexture));
	list[i++] = new Sphere(Vec3(0,10,0),10, new Lambertian(checkerTexture));
	return new HitableList(list, i);
}

Hitable* sphere_image_texture(){
	int i = 0;
	Hitable** list = new Hitable*[2];
	int width, height, channels;
	std::vector<uint8_t> img = bmphelper::read_bmp("./data/planet.bmp", &width, &height, &channels, false);
	uint8_t* data = new uint8_t[width*height*3];
	for (int i = 0; i < width*height*3; i++){
		data[i] = img[i];
	}
	list[i++] = new Sphere(Vec3(0,-1000,0),1000, new Metal(Vec3(0.8, 0.8, 0.8),0.2));
	list[i++] = new Sphere(Vec3(0,2,0),2, new Lambertian(new ImageTexture(data, width, height)));
	return new HitableList(list, i);
}

Hitable* area_light_scene(){
	int i = 0;
	Hitable** list = new Hitable*[3];
	list[i++] = new Sphere(Vec3(0,-1000,0),1000, new Lambertian(new Marble(4)));
	list[i++] = new Sphere(Vec3(0,2,0),2, new Lambertian(new Marble(5)));
	list[i++] = new Sphere(Vec3(0,7,0), 2, new DiffuseLight(new ConstantTexture(Vec3(4,4,4))));
	list[i++] = new XYRect(3, 5, 1, 3, -2, new DiffuseLight(new ConstantTexture(Vec3(4,4,4))));
	return new BVHNode(list, i);
}

Hitable* cornell_box(){
	Material* red = new Lambertian(Vec3(0.65, 0.05, 0.05));
	Material* white = new Lambertian(Vec3(0.73, 0.73, 0.73));
	Material* green = new Lambertian(Vec3(0.12, 0.45, 0.15));
	Material* light = new DiffuseLight(new ConstantTexture(Vec3(15,15,15)*10));
	int i = 0; 
	Hitable** list = new Hitable*[6];
	list[i++] = new YZRect(0, 555, 0, 555, 555, green);
	list[i++] = new YZRect(0, 555, 0, 555, 0, red);
	list[i++] = new XZRect(213, 343, 227, 332, 554, light);
	list[i++] = new XZRect(0, 555, 0, 555, 555, white);
	list[i++] = new XZRect(0, 555, 0, 555, 0, white);
	list[i++] = new XYRect(0, 555, 0, 555, 555, white);
	return new HitableList(list, i);
}

Camera cornell_box_camera(double nx, double ny){
	Vec3 lookfrom(278,278,-800);
	Vec3 lookat(278,278,0);
	double dist_focus = 10;
	double aperture = 0;
	double fov = 40;
	return Camera(lookfrom, lookat, Vec3(0,1,0), fov, nx/ny, aperture, dist_focus);
}

Camera custom_cam(double nx, double ny){
	Vec3 lookfrom = Vec3(-10.7, 4,-10);
	Vec3 lookat = Vec3(1,0.6,-0.5);
	Vec3 vUp = Vec3(0, 1, 0);
	double fov = 45;
	double dist_to_focus = (lookfrom-lookat).length()+0.1;
	double aperture = 0;
	return Camera(lookfrom, lookat, vUp, fov, nx/ny, aperture, dist_to_focus);
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