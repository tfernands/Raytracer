#include <iostream>
#include <thread>
#include <fstream>
#include <random>
#include <atomic>
#include <string>
#include <algorithm>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "Camera.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "HitableList.hpp"
#include "BVHNode.hpp"
#include "scenes.hpp"
#include "float.h"
#include "utils.hpp"
#include "ProgressBar.hpp"

using namespace std;

int max_depth = 12;

atomic_bool kill_render;

Vec3 background(const Ray& r){
	Vec3 unit_direction = Vec3::unit_vector(r.direction());
	float t = unit_direction.y();
	Vec3 sky = (1.0-t)*Vec3(1, 0.9, 0.9)+t*Vec3(0.6, 0.7, 1);
	return sky;//*0.1;
}

Vec3 shoot(const Ray& r, Hitable *world, int depth){
	HitRecord rec;
	if (world->hit(r, 0.001, DBL_MAX, rec)){
		Ray scattered;
		Vec3 attenuation;
		if (depth < max_depth && rec.material->scatter(r, rec, attenuation, scattered)){
			if (rec.material->final_hit) return attenuation;
			else return attenuation*shoot(scattered, world, depth+1);
		}
		else{
			return Vec3(0,0,0);
		}
	}else{
		return background(r);
	}
}

int *indexes;
int *spps;

void render(Vec3* buffer, Hitable* world, const Camera &cam, const Render_config &config, int start, int end){
	while (true){
		for (int idx = start; idx < end; idx++){
			spps[indexes[idx]]++;
			int i = indexes[idx] / config.width;
			int j = indexes[idx] % config.width;
			float u = float(j+random())/float(config.width);
			float v = float(i+random())/float(config.height);
			Ray r = cam.get_ray(u, v);
			buffer[indexes[idx]] += shoot(r, world, 0);
			if (kill_render) return;
		}
	}
	return;
}

//args (1:name 2:width 3:height 4:spp 5:seed
int main(int argc, char **argv){
	
	kill_render = false;

	Render_config config;
	config.width = 800;
	config.height = 450;
	config.threads = thread::hardware_concurrency();
	if (input_handler(config, argc, argv) > 0) return 1;

	Hitable* world = random_scene(1000);	

	Vec3 lookfrom = Vec3(-10.7, 4,-10.4);
	Vec3 lookat = Vec3(1,0.6,-0.5);
	Vec3 vUp = Vec3(0, 1, 0);
	float fov = 45;
	float dist_to_focus = (lookfrom-lookat).length()+0.1;
	float aperture = 0.1;
	Camera cam(lookfrom, lookat, vUp, fov, float(config.width)/float(config.height), aperture, dist_to_focus);
	
	const int buffer_length = config.width*config.height;
	indexes = new int[buffer_length];
	spps = new int[buffer_length];
	Vec3 *buffer = new Vec3[buffer_length];
	for (int i = 0; i < buffer_length; i++){
		indexes[i] = i;
		spps[i] = 0;
	}
	for (int i = 0; i < 40; i++){
		random_shuffle(&indexes[0], &indexes[buffer_length]);
	}
	
	thread* threads[config.threads];
	for (int i = 0; i < config.threads; i++){
		int start = (buffer_length/config.threads)*i;
		int end = (buffer_length/config.threads)*(i+1);
		threads[i] = new thread(render, buffer, world, ref(cam), ref(config), start, end);
	}
	//================ Display and save image ====================

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
		//update window

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
				if (keys[sf::Keyboard::Escape]){

				}
			}
			if (event.type == sf::Event::KeyReleased){
				keys[event.key.code] = false;
			}
		}

		if (keys[sf::Keyboard::W]		||
			keys[sf::Keyboard::A] 		||
			keys[sf::Keyboard::S] 		||
			keys[sf::Keyboard::D] 		||
			keys[sf::Keyboard::Q] 		||
			keys[sf::Keyboard::E] 		||
			keys[sf::Keyboard::R] 		||
			keys[sf::Keyboard::F] 		||
			keys[sf::Keyboard::Z] 		||
			keys[sf::Keyboard::X] 		||
			keys[sf::Keyboard::C] 		||
			keys[sf::Keyboard::V] 		||
			keys[sf::Keyboard::Up] 		||
			keys[sf::Keyboard::Down] 	||
			keys[sf::Keyboard::Left] 	||
			keys[sf::Keyboard::Right] 	 ){
			float rotate_vel = 1*elapsed.asSeconds();
			float move_vel = 10*elapsed.asSeconds();
			float zoom_vel = 1+0.1*elapsed.asSeconds();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
				rotate_vel *= 10;
				move_vel *= 10;
				zoom_vel = 1+0.1*10*elapsed.asSeconds();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)){
				rotate_vel /= 10;
				move_vel /= 10;
			}

			//MOVE
			if (keys[sf::Keyboard::W]){
				lookfrom -= cam.w * move_vel;
				lookat = lookfrom-cam.w;
			}
			if (keys[sf::Keyboard::A]){
				lookfrom -= cam.u * move_vel;
				lookat = lookfrom-cam.w;
			}
			if (keys[sf::Keyboard::S]){
				lookfrom += cam.w * move_vel;
				lookat = lookfrom-cam.w;
			}
			if (keys[sf::Keyboard::D]){
				lookfrom += cam.u * move_vel;
				lookat = lookfrom-cam.w;
			}
			if (keys[sf::Keyboard::R]){
				lookfrom += cam.v * move_vel;
				lookat = lookfrom-cam.w;
			}
			if (keys[sf::Keyboard::F]){
				lookfrom -= cam.v * move_vel;
				lookat = lookfrom-cam.w;
			}

			//ROTATE
			if (keys[sf::Keyboard::Up]){
				lookat = lookfrom-cam.w;
				lookat -= cam.v * rotate_vel;
			}
			if (keys[sf::Keyboard::Down]){
				lookat = lookfrom-cam.w;
				lookat += cam.v * rotate_vel;
			}
			if (keys[sf::Keyboard::Left]){
				lookat = lookfrom-cam.w;
				lookat -= cam.u * rotate_vel;
			}
			if (keys[sf::Keyboard::Right]){
				lookat = lookfrom-cam.w;
				lookat += cam.u * rotate_vel;
			}
			if (keys[sf::Keyboard::Q]){
				vUp -= cam.u * rotate_vel;
			}
			if (keys[sf::Keyboard::E]){
				vUp += cam.u * rotate_vel;
			}

			//ZOOM
			if (keys[sf::Keyboard::Z]){
				fov /= zoom_vel;
			}
			if (keys[sf::Keyboard::X]){
				fov *= zoom_vel;
			}

			//FOCUS
			if (keys[sf::Keyboard::C]){
				dist_to_focus /= zoom_vel;
			}
			if (keys[sf::Keyboard::V]){
				dist_to_focus *= zoom_vel;
			}

			kill_render = true;
			for (int i = 0; i < config.threads; i++){
				if (threads[i]->joinable())
					threads[i]->join();
				delete threads[i];
			}
			kill_render = false;

			cam.set(lookfrom, lookat, vUp, fov, float(config.width)/float(config.height), aperture, dist_to_focus);
			for (int i = 0; i < buffer_length; i++){
				buffer[i].set(0,0,0);
				spps[i] = 0;
			}
			for (int i = 0; i < config.threads; i++){
				int start = (buffer_length/config.threads)*i;
				int end = (buffer_length/config.threads)*(i+1);
				threads[i] = new thread(render, buffer, world, ref(cam), ref(config), start, end);
			}
			int sum = 0;
			while(sum <buffer_length){
				sum = 0;
				for (int i = 0; i < buffer_length; i++){
					sum += spps[i];
				}
			}
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
		if (threads[i]->joinable())
			threads[i]->join();
		delete threads[i];
	}
	delete [] indexes;
	delete [] pixels;
	return 0;
}