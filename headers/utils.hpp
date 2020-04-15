#ifndef UTIL_F_HPP
#define UTIL_F_HPP

#include <iostream>
#include <fstream>
#include "Vec3.hpp"

using namespace std;

std::default_random_engine rand_gen;
std::uniform_real_distribution<float> drand48(0.0,1.0);

void set_seed(int seed){
	rand_gen.seed(seed);
}

float random(){
	return drand48(rand_gen);
}

float max(float a, float b){
	if (a > b) return a;
	else return b;
}

float min(float a, float b){
	if (a < b) return a;
	else return b;
}

Vec3 random_in_unit_sphere(){
	Vec3 p;
	do {
		p = 2.0*Vec3(random(),random(),random()) - Vec3(1,1,1);
	} while (p.squared_length() >= 1.0);
	return p;
}

Vec3 random_in_unit_disk(){
	Vec3 p;
	do{
		p = 2*Vec3(random(),random(),0) - Vec3(1,1,0);
	}while(Vec3::dot(p,p) >= 1.0);
	return p;
}


Vec3 bufferToPixel(const Vec3 &pixel, const int &spp){
	if (spp == 0) return Vec3(0,0,0);
	Vec3 color = pixel/float(spp);
	color[0] = sqrt(color[0]);
	color[1] = sqrt(color[1]);
	color[2] = sqrt(color[2]);
	color.clip(0,1);
	color *= 255.99;
	return color;
}

void save_as_ppm(const char *name, Vec3 *buffer, const int &nx, const int &ny, const int &spp){
	ofstream myfile;
	myfile.open(name);
	myfile << "P3\n" << nx << " " << ny << "\n255\n";
	int n = nx*ny;
	for (int idx = 0; idx < n; idx++){
		Vec3 color = bufferToPixel(buffer[idx], spp);
		myfile << int(color.r()) <<' '<< int(color.g()) <<' '<< int(color.b()) <<endl;
	}
	myfile.close();	
}

typedef struct Render_config{
	char* file_name = (char*)"rendered_img.png";
	int threads = 1;
	int width = 400;
	int height = 225;
	int seed = 1998;
} Render_config;

static void show_usage(std::string name){
	std::cerr << "Usage: " << name << " <option(s)> SOURCES "<< std::endl
			  << "Options:"<< std::endl
			  << "\t-help,--help\t\t\tShow this help message"<< std::endl
			  << "\t-f,--file\tFILENAME\tSpecify the destination path"<< std::endl
			  << "\t-w,--width\tWIDTH\t\tSpecify the width of the image"<< std::endl
			  << "\t-h,--height\tHEIGHT\t\tSpecify the height of the image"<< std::endl
			  << "\t-sz,--size\tWIDTH HEIGHT\tSpecify the width and height of the image"<< std::endl
			  << "\t--seed\t\tSEED\t\tSpecify the seed for the random generator"<< std::endl
			  << "\t-t,--threads\tTHREADS\t\tSpecify the number of threads to render"<< std::endl;		  
}

int input_handler(Render_config &config, int argc, char **argv){

	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if ((arg == "-help") || (arg == "--help")) {
			show_usage(argv[0]);
		}
		else if ((arg == "-f") || (arg == "--file")) {
			if (i + 1 < argc){
				i++;
				config.file_name = argv[i];
			}
			else {
				std::cerr << "--file option requires one argument." << std::endl;
				return 1;
			}  
		}
		else if ((arg == "-w") || (arg == "--width")) {
			if (i + 1 < argc){
				i++;
				config.width = atoi(argv[i]); 
			}
			else {
				std::cerr << "--width option requires one argument." << std::endl;
				return 1;
			}  
		}
		else if ((arg == "-h") || (arg == "--height")) {
			if (i + 1 < argc){
				i++;
				config.height = atoi(argv[i]); 
			}
			else {
				std::cerr << "--height option requires one argument." << std::endl;
				return 1;
			}  
		}
		else if ((arg == "-sz") ||(arg == "--size")) {
			if (i + 2 < argc && atoi(argv[i+2]) != 0 && atoi(argv[i+1]) != 0){
				i++;
				config.width = atoi(argv[i++]); 
				config.height = atoi(argv[i]); 
			}
			else {
				std::cerr << "--size option requires two arguments." << std::endl;
				return 1;
			}  
		}
		else if ((arg == "--seed")) {
			if (i + 1 < argc){
				i++;
				config.seed = atoi(argv[i]); 
			}
			else {
				std::cerr << "--seed option requires one argument." << std::endl;
				return 1;
			}  
		}
		else if ((arg == "-t") || (arg == "--threads")) {
			if (i + 1 < argc){
				i++;
				config.threads = atoi(argv[i]); 
			}
			else {
				std::cerr << "--threads option requires one argument." << std::endl;
				return 1;
			}  
		}
		else{
			show_usage(argv[0]);
			return 1;
		}
	}
	if (config.width <= 0){
		std::cerr << "width less or equal 0" << std::endl;
		return 1;
	}
	if (config.height <= 0){
		std::cerr << "height less or equal 0" << std::endl;
		return 1;
	}
	if (config.threads <= 0){
		std::cerr << "threads less or equal 0" << std::endl;
		return 1;
	}
	set_seed(config.seed);
	printf("Name: %s | resolution: %d, %d | seed %d | threads %d\n",
		config.file_name, config.width, config.height, config.seed, config.threads);
	return 0;
}

#endif