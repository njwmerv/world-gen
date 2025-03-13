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
		return {0, 0, 150};
	}
	else if(elevation < 0.4f){ // Shallow water
		return {0, 100, 200};
	}
	else if(elevation < 0.425f){ // Beach
		return {240, 220, 130};
	}
	else if(elevation < 0.7f){ // Grass
		return {34, 139, 34};
	}
	else if(elevation < 0.8f){ // Mountain
		return {100, 100, 100};
	}
	else{ // Snow
		return {255, 255, 255};
	}
}

int main(){
	constexpr int WINDOW_WIDTH = 1920;
	constexpr int WINDOW_HEIGHT = 1080;
	constexpr int MAX_OCTAVES = 8;

	// Setting up Display (SFML)
	sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Perlin");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	sf::Image noiseImage({WINDOW_WIDTH, WINDOW_HEIGHT}, sf::Color::Black);

	sf::RenderTexture renderTexture({WINDOW_WIDTH, WINDOW_HEIGHT});
	sf::Texture noiseTexture;

	// Generating the Perlin noise
	const int seed = static_cast<int>(time(nullptr));
	Perlin noiseGenerator(seed);

	for(unsigned row = 0; row < WINDOW_HEIGHT; row++){
		for(unsigned col = 0; col < WINDOW_WIDTH; col++){
			float persistence = 1.0f;
			float lacunarity = 0.005f;
			float noise = 0;
			for(int octave = 0; octave < MAX_OCTAVES; octave++){
				noise += persistence * noiseGenerator.perlin(col * lacunarity, row * lacunarity);
				persistence *= 0.5;
				lacunarity *= 2.0;
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