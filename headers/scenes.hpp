#include "EnvironmentSphere.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "HitableList.hpp"
#include "Material.hpp"
#include "bmphelper.hpp"

ImageTexture* bmp2Texture(const std::string& path);

// =============== CAMERAS =================
Camera cornell_box_camera(double nx, double ny){
	Vec3 lookfrom(278,278,-800);
	Vec3 lookat(278,278,0);
	double dist_focus = 10;
	double aperture = 0;
	double fov = 40;
	return Camera(lookfrom, lookat, Vec3(0,1,0), fov, nx/ny, aperture, dist_focus);
}

Camera custom_cam(double nx, double ny){
	Vec3 lookfrom = Vec3(-8.75882, 3.10571, -8.18443);
	Vec3 lookat = Vec3(-0.589403, 0.339594, -0.732994);
	Vec3 vUp = Vec3(0, 1, 0);
	double fov = 30;
	double dist_to_focus = 8.2;
	double aperture = 0.0;
	return Camera(lookfrom, lookat, vUp, fov, nx/ny, aperture, dist_to_focus);
}

// ============ OBJECTS ==============
Hitable* ground_and_sphere(){
	Texture* constColor = new ConstantTexture(Vec3(0.7,0.7,0.7));
	int i = 0;
	Hitable** list = new Hitable*[3];
	list[i++] = new EnvironmentSphere(new SkyMapTexture());
	list[i++] = new Sphere(Vec3(0,-1000,0), 1000, new Lambertian(constColor));
	list[i++] = new Sphere(Vec3(-4,1,0),1, new Lambertian(constColor));
	return new HitableList(list, i);
}

Hitable* two_sphere(){
	Texture* checkerTexture = new CheckerTexture(new ConstantTexture(Vec3(0.2,.3,.1)), new ConstantTexture(Vec3(0.9,.9,.9)), 10);
	int i = 0;
	Hitable** list = new Hitable*[3];
	list[i++] = new EnvironmentSphere(new SkyMapTexture());
	list[i++] = new Sphere(Vec3(0,-10,0),10, new Lambertian(checkerTexture));
	list[i++] = new Sphere(Vec3(0,10,0),10, new Lambertian(checkerTexture));
	return new HitableList(list, i);
}

Hitable* sphere_image_texture(){
	int i = 0;
	Texture* earthTexture = bmp2Texture("./data/planet.bmp");
	Hitable** list = new Hitable*[3];
	list[i++] = new EnvironmentSphere(new SkyMapTexture());
	list[i++] = new Sphere(Vec3(0,0,0),2, new Lambertian(earthTexture));
	list[i++] = new Sphere(Vec3(6,0,0),2, new Metal(new ConstantTexture(Vec3(0.6,0.6,0.6)), new InvertTexture(bmp2Texture("./data/europa.bmp"))));
	return new HitableList(list, i);
}

Hitable* area_light_scene(){
	int i = 0;
	Hitable** list = new Hitable*[5];
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

Hitable* random_scene(int n){
	Texture* checkerTexture = new CheckerTexture(new ConstantTexture(Vec3(0.2,.3,.1)), new ConstantTexture(Vec3(0.9,.9,.9)), 5);
	int i = 0; 
	Hitable** list_spheres = new Hitable*[n+1];
	Hitable* ground_plane = new Sphere(Vec3(0,-100000,0), 100000, new Lambertian(checkerTexture));
	list_spheres[i++] = new Sphere(Vec3(0,1,0),1,new Dielectric(1.3));
	list_spheres[i++] = new Sphere(Vec3(4,1,0),1,new Lambertian(Vec3(0.9,0.9,0.9)));
	list_spheres[i++] =  new Sphere(Vec3(-4,1,0),1,new Metal(new ConstantTexture(Vec3(0.6,0.6,0.6)), new ConstantTexture(Vec3(0,0,0))));
	while(true){
		double choose_mat = random();
		Vec3 center(50*(random()-0.5), random()*0.3+0.07, 50*(random()-0.5));
		if ((center-Vec3(4,0.2,0)).length() > 0.9){
			if (choose_mat < 0.4){
				list_spheres[i++] = new Sphere(center, center.y(), new Lambertian());
			}
			else if (choose_mat < 0.75){
				list_spheres[i++] = new Sphere(
					center, center.y(),
					new Metal(Vec3(0.5*(1+random()), 0.5*(1+random()), 0.5*(1+random())), 0.3*random())
				);
			}
			else{
				list_spheres[i++] = new Sphere(center, center.y(), new Dielectric(1.5));
			}
			if (i == n)
				break;
		}
	}
	Hitable** final_list = new Hitable*[3];
	final_list[0] = new EnvironmentSphere(new SkyMapTexture());
	final_list[1] = ground_plane;
	final_list[2] = new BVHNode(list_spheres, i, 0);
	return new HitableList(final_list, 3);
}


// util
ImageTexture* bmp2Texture(const std::string& path){
	int width, height, channels;
	std::vector<uint8_t> img = bmphelper::read_bmp(path, &width, &height, &channels, false);
	uint8_t* data = new uint8_t[width*height*3];
	for (int i = 0; i < width*height*3; i++)
		data[i] = img[i];
	return new ImageTexture(data, width, height);
}