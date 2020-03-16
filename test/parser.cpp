#include <iostream>
#include <string>
#include <vector>

static void show_usage(std::string name){
	std::cerr << "Usage: " << name << " <option(s)> SOURCES "<< std::endl
			  << "Options:"<< std::endl
			  << "\t-help,--help\t\t\tShow this help message"<< std::endl
			  << "\t-f,--file\tFILENAME\tSpecify the destination path"<< std::endl
			  << "\t-w,--width\tWIDTH\t\tSpecify the width of the image"<< std::endl
			  << "\t-h,--height\tHEIGHT\t\tSpecify the height of the image"<< std::endl
			  << "\t-spp,--samples\tSAMPLES\t\tSpecify the number of rays sampled from each pixel"<< std::endl
			  << "\t-seed\t\tSEED\t\tSpecify the seed for the random generator"<< std::endl;		  
}

void parseArgs(int argc, char** argv){

}

int main(int argc, char* argv[]){
	if (argc > 1 && argc < 3) {
		show_usage(argv[0]);
		return 1;
	}
	std::vector <std::string> sources;
	std::string filepath = "teste.png";
	int width 	= 400;
	int height 	= 200;
	int spp 	= 10;
	int seed 	= 0;
	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if ((arg == "-help") || (arg == "--help")) {
			show_usage(argv[0]);
			return 0;
		}
		else if ((arg == "-f") || (arg == "--file")) {
			if (i + 1 < argc){
				i++;
				filepath = argv[i];
			}
			else {
				std::cerr << "--file option requires one argument." << std::endl;
				return 1;
			}  
		}
		else if ((arg == "-w") || (arg == "--width")) {
			if (i + 1 < argc){
				i++;
				width = atoi(argv[i]); 
			}
			else {
				std::cerr << "--width option requires one argument." << std::endl;
				return 1;
			}  
		}
		else if ((arg == "-h") || (arg == "--height")) {
			if (i + 1 < argc){
				i++;
				height = atoi(argv[i]); 
			}
			else {
				std::cerr << "--height option requires one argument." << std::endl;
				return 1;
			}  
		}
		else if ((arg == "-spp") || (arg == "--samples")) {
			if (i + 1 < argc){
				i++;
				spp = atoi(argv[i]); 
			}
			else {
				std::cerr << "--samples option requires one argument." << std::endl;
				return 1;
			}  
		}
		else if ((arg == "-seed")) {
			if (i + 1 < argc){
				i++;
				seed = atoi(argv[i]); 
			}
			else {
				std::cerr << "--seed option requires one argument." << std::endl;
				return 1;
			}  
		}
		else {
			if (sources.size() == 0){
				width = atoi(argv[i]);
			}
			else if (sources.size() == 1){
				height = atoi(argv[i]);
			}
			else if (sources.size() == 2){
				spp = atoi(argv[i]);
			}
			sources.push_back(argv[i]);
		}
	}
	std::cout << "filepath:\t" << filepath << std::endl;
	std::cout << "width:\t\t" << width << std::endl;
	std::cout << "height:\t\t" << height << std::endl;
	std::cout << "spp:\t\t" << spp << std::endl;
	std::cout << "seed:\t\t" << seed << std::endl;
	for (int i = 0; i < sources.size(); i++) std::cout << sources[i] << std::endl;
	return 0;
}
