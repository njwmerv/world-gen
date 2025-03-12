#include <algorithm>
#include <SFML/Graphics.hpp>
#include "perlin.h"
#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Window/VideoMode.hpp"
using namespace std;

sf::Color simpleTerrainColor(const float elevation) {
	if(elevation < 0.3f){ // Deep water
		return sf::Color(0, 0, 150);
	}
	else if(elevation < 0.4f){ // Shallow water
		return sf::Color(0, 100, 200);
	}
	else if(elevation < 0.425f){ // Beach
		return sf::Color(240, 220, 130);
	}
	else if(elevation < 0.7f){ // Grass
		return sf::Color(34, 139, 34);
	}
	else if(elevation < 0.8f){ // Mountain
		return sf::Color(100, 100, 100);
	}
	else{ // Snow
		return sf::Color(255, 255, 255);
	}
}

int main(){
	const int WINDOW_WIDTH = 1920;
	const int WINDOW_HEIGHT = 1080;
	const int MAX_OCTAVES = 8;

	// Setting up Display (SFML)
	sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Perlin");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	sf::Image noiseImage({WINDOW_WIDTH, WINDOW_HEIGHT}, sf::Color::Black);

	sf::RenderTexture renderTexture({WINDOW_WIDTH, WINDOW_HEIGHT});
	sf::Texture noiseTexture;

	// Generating the Perlin noise
	const int seed = static_cast<int>(time(0));
	Perlin noiseGenerator = Perlin(seed);

	for(unsigned row = 0; row < WINDOW_HEIGHT; row++){
		for(unsigned col = 0; col < WINDOW_WIDTH; col++){
			float amplitude = 1.0;
			float frequency = 0.005f;
			float noise = 0;
			for(int octave = 0; octave < MAX_OCTAVES; octave++){
				noise += amplitude * noiseGenerator.perlin(col * frequency, row * frequency);
				amplitude *= 0.5;
				frequency *= 2.0;
			}
			noise = (noise + 1.0) / 2.0; // transform from [-1, 1] -> [0, 1]
			noiseImage.setPixel({col, row}, simpleTerrainColor(noise));
		}
	}

	// Setting up display 2
	noiseTexture.loadFromImage(noiseImage);
	const sf::Sprite noiseSprite(noiseTexture);

	// Main loop
	while(window.isOpen()){
		while(const std::optional event = window.pollEvent()){
			if(event->is<sf::Event::Closed>()) window.close();
		}

		window.clear(sf::Color::Black);
		window.draw(noiseSprite);

		window.display();
	}

	return 0;
}