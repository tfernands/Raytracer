#include <iostream>
#include <fstream>
#include "Vec3.hpp"

using namespace std;

int main(){
	ofstream myfile;
	myfile.open ("teste_image.ppm");
	int nx = 200;
	int ny = 100;

	myfile << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny-1; j >= 0; j--){
		for (int i = 0; i < nx; i++){
			Vec3 color(float(i) / float(nx), float(j) / float(ny), 0.2);
			int ir = int(255.99*color.r());
			int ig = int(255.99*color.g());
			int ib = int(255.99*color.b());
			myfile << ir << " " << ig << " " << ib <<"\n";
		}
		cout << j << endl;
	}
	myfile.close();
	return 0;
}