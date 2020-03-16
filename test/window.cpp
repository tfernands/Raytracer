#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>


int main(int argc, char *argv[]){

	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Test");
	sf::Image        image;
	sf::Texture      texture;
	sf::Sprite       sprite;
	sf::Uint8        *pixels = new sf::Uint8[800 * 600 * 4];

	while(window.isOpen()){
		// Process events
		sf::Event event;
		while (window.pollEvent(event)){
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			std::cout << "isButtonPressed" << std::endl;
		}
		// get global mouse position
		//for(int x = 0; x < 800; x++){
		//	for(int y = 0; y < 600; y++){
		//		pixels[(y*800+x)*4]     = 255; // R?
		//		pixels[(y*800+x)*4 + 1] = 255; // G?
		//		pixels[(y*800+x)*4 + 2] = 255; // B?
		//		pixels[(y*800+x)*4 + 3] = 255; // A?
		//	}
		//}

		sf::Vector2i position = sf::Mouse::getPosition(window);
		int pixel_id = (position.x+position.y*800)*4;
		if (pixel_id > 0 && pixel_id < 800*600*4){
			pixels[pixel_id  ] = 255;	//R
			pixels[pixel_id+1] =   0;	//G
			pixels[pixel_id+2] =   0;	//B
			pixels[pixel_id+3] = 255;	//A
		}

		image.create(800, 600, pixels);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		//window.clear();
		window.draw(sprite);
		window.display();
		
	}

	delete [] pixels;
	return 0;
}