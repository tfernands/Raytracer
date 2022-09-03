#include <iostream>
#include <thread>
#include <fstream>
#include <random>
#include <atomic>
#include <string>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Camera.hpp"
#include "Material.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "HitableList.hpp"
#include "BVHNode.hpp"
#include "scenes.hpp"
#include "float.h"
#include "utils.hpp"
#include "ProgressBar.hpp"


using namespace std;

int max_depth = 20;

float fog_max_dist = 10;

atomic_bool kill_render;
int *indexes;
int *spps;

Vec3 background(const Ray& r){
	Vec3 unit_direction = Vec3::unit_vector(r.direction());
	float t = 0.5*(unit_direction.y() + 1.0);
	return (1.0-t)*Vec3(1, 1, 1) + t*Vec3(0.5, 0.7, 1);
}

Vec3 shoot(const Ray& r, Hitable *world, int depth){
	HitRecord rec;
	if (world->hit(r, 0.001, DBL_MAX, rec)){
		Ray scattered;

		if (depth < max_depth && drand48() < rec.t/fog_max_dist){
			scattered = Ray(r.origin()+r.direction()*drand48()*min(max_depth,rec.t), random_in_unit_sphere());
			return shoot(scattered, world, depth+1);
		}

		Vec3 attenuation;
		Vec3 emitted = rec.material->emitted(rec.u, rec.v, rec.p);
		if (depth < max_depth && rec.material->scatter(r, rec, attenuation, scattered)){
			return emitted + attenuation*shoot(scattered, world, depth+1);
		}
		else{
			return emitted;
		}
	}else{	
		if (depth < max_depth){	
			Ray scattered = Ray(r.origin()+r.direction()*drand48()*max_depth, random_in_unit_sphere());
			return shoot(scattered, world, depth+1);
		}
		return background(r)*Vec3(.08, .1, .13)*.1;
	}
}

void render(Vec3* buffer, Hitable* world, const Camera &cam, const Render_config &config, int start, int end){
	while (true){
		for (int idx = start; idx < end; idx++){
			spps[indexes[idx]]++;
			int i = indexes[idx] / config.width;
			int j = indexes[idx] % config.width;
			double u = double(j+drand48())/double(config.width);
			double v = double(i+drand48())/double(config.height);
			Ray r = cam.get_ray(u, v);
			buffer[indexes[idx]] += shoot(r, world, 0);
			if (kill_render) return;
		}
	}
}

#include "Vec3.hpp"

int main(int argc, char **argv){

	cout << "MAIN LOOP" << endl;
	
	kill_render = false;
	Render_config config;
	config.threads = 16;
	config.width = 800;
	config.height = 500;
	config.threads = thread::hardware_concurrency();
	if (input_handler(config, argc, argv) > 0) return 1;

	// Initialize scene
	Hitable* world = area_light_scene();
	
	Camera cam = custom_cam(config.width, config.height);

	const int buffer_length = config.width*config.height;
	indexes = new int[buffer_length];
	spps = new int[buffer_length];
	Vec3 *buffer = new Vec3[buffer_length];
	for (int i = 0; i < buffer_length; i++){
		indexes[i] = i;
		spps[i] = 0;
	}
	for (int i = 0; i < 45; i++){
		random_shuffle(&indexes[0], &indexes[buffer_length]);
	}
	
	// start threads
	int nthreads = config.threads;
	thread* threads[nthreads];
	for (int i = 0; i < nthreads; i++){
		int start = (buffer_length/nthreads)*i;
		int end = (buffer_length/nthreads)*(i+1);
		threads[i] = new thread(render, buffer, world, ref(cam), ref(config), start, end);
	}

	// ================ Display and save image ====================
	
	std::cout << "Openning Display" << std::endl;

	sf::RenderWindow window(sf::VideoMode(config.width, config.height, 32), config.file_name);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
	sf::Image        image;
	sf::Texture      texture;
	sf::Sprite       sprite;
	sf::Uint8        *pixels = new sf::Uint8[buffer_length*4];
	sf::Clock		 clock;
	int keys_count = 400;
	bool keys[keys_count];
	for (int i = 0; i < keys_count; i++) keys[i] = false;
	while(window.isOpen()){
		// update window
		sf::Time elapsed = clock.restart();
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				image.saveToFile(string(config.file_name));
				kill_render = true;
				for (int i = 0; i < config.threads; i++)
					if (threads[i]->joinable())
						threads[i]->join();
				window.close();
			}
			if (event.type == sf::Event::KeyPressed){
				keys[event.key.code] = true;
				if (keys[sf::Keyboard::P]) image.saveToFile(string(config.file_name));
			}
			if (event.type == sf::Event::KeyReleased)
				keys[event.key.code] = false;
		}

		if (keys[sf::Keyboard::W] || keys[sf::Keyboard::A] || keys[sf::Keyboard::S] ||
			keys[sf::Keyboard::D] || keys[sf::Keyboard::Q] || keys[sf::Keyboard::E] ||
			keys[sf::Keyboard::R] || keys[sf::Keyboard::F] || keys[sf::Keyboard::Z] ||
			keys[sf::Keyboard::X] || keys[sf::Keyboard::C] || keys[sf::Keyboard::V] ||
			keys[sf::Keyboard::Up] || keys[sf::Keyboard::Down] ||
			keys[sf::Keyboard::Left] || keys[sf::Keyboard::Right]){
			double rotate_vel = 1*elapsed.asSeconds();
			double move_vel = 10*elapsed.asSeconds();
			double zoom_vel = 1+0.1*elapsed.asSeconds();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
				move_vel *= 10;
				zoom_vel = 1+0.1*10*elapsed.asSeconds();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)){
				rotate_vel /= 10;
				move_vel /= 10;
			}

			// MOVE
			Vec3 lookfrom = cam.origin;
			if (keys[sf::Keyboard::W])
				lookfrom -= cam.w * move_vel;
			if (keys[sf::Keyboard::A])
				lookfrom -= cam.u * move_vel;
			if (keys[sf::Keyboard::S])
				lookfrom += cam.w * move_vel;
			if (keys[sf::Keyboard::D])
				lookfrom += cam.u * move_vel;
			if (keys[sf::Keyboard::R])
				lookfrom += cam.v * move_vel;
			if (keys[sf::Keyboard::F])
				lookfrom -= cam.v * move_vel;

			// ROTATE
			Vec3 lookat = lookfrom-cam.w;
			Vec3 vUp = cam.get_vup();
			if (keys[sf::Keyboard::Up])
				lookat -= cam.v * rotate_vel;
			if (keys[sf::Keyboard::Down])
				lookat += cam.v * rotate_vel;
			if (keys[sf::Keyboard::Left])
				lookat -= cam.u * rotate_vel;
			if (keys[sf::Keyboard::Right])
				lookat += cam.u * rotate_vel;
			if (keys[sf::Keyboard::Q])
				vUp -= cam.u * rotate_vel;
			if (keys[sf::Keyboard::E])
				vUp += cam.u * rotate_vel;

			// ZOOM
			double fov = cam.get_fov();
			if (keys[sf::Keyboard::Z])
				fov /= zoom_vel;
			if (keys[sf::Keyboard::X])
				fov *= zoom_vel;

			// FOCUS
			double dist_to_focus = cam.get_focus_dist();
			if (keys[sf::Keyboard::C])
				dist_to_focus /= zoom_vel;
			if (keys[sf::Keyboard::V])
				dist_to_focus *= zoom_vel;

			// Restart render
			kill_render = true;
			std::cout<<"Killing threads"<<std::endl;
			for (int i = 0; i < config.threads; i++){
				if (threads[i]->joinable())
					threads[i]->join();
				delete threads[i];
			}
			std::cout<<"No threads running"<<std::endl;
			kill_render = false;

			cam.set(lookfrom, lookat, vUp, fov, cam.get_aspect(), cam.get_aperture(), dist_to_focus);
			for (int i = 0; i < buffer_length; i++){
				buffer[i].set(0,0,0);
				spps[i] = 0;
			}
			for (int i = 0; i < config.threads; i++){
				int start = (buffer_length/config.threads)*i;
				int end = (buffer_length/config.threads)*(i+1);
				threads[i] = new thread(render, buffer, world, ref(cam), ref(config), start, end);
			}

			//Wait some of the image to be rendered
			std::cout<<"Wait for the first pass be rendered"<<std::endl;
			int sum = 0;
			while(sum < buffer_length*.1){
				sum = 0;
				for (int i = 0; i < buffer_length; i++){
					if (spps[i] >= 1) {
						sum += 1;
					}
				}
			}
			std::cout<<"Display\n"<<std::endl;
			//std::cout << cam << std::endl;
		}

		for (int idx = 0; idx < buffer_length; idx++){
			Vec3 px = bufferToPixel(buffer[idx], spps[idx]);
			int px_id = idx*4;	
			pixels[px_id  ] = int(px.r());	//R
			pixels[px_id+1] = int(px.g());	//G
			pixels[px_id+2] = int(px.b());	//B
			pixels[px_id+3] = 255;			//A
		}
		image.create(config.width, config.height, pixels);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		window.draw(sprite);
		window.display();
	}
	for (int i = 0; i < config.threads; i++){
		if (threads[i]->joinable()) threads[i]->join();
		delete threads[i];
	}
	delete [] indexes;
	delete [] pixels;
	return 0;
}