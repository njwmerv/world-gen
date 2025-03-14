#include "chunk.h"
#include <SFML/Graphics.hpp>

Chunk::Chunk(const int chunkX, const int chunkY, const Perlin& noiseGenerator) :
    chunkX(chunkX), chunkY(chunkY), chunkSprite(nullptr), chunkTexture(nullptr)
{
    sf::Image image({SIZE, SIZE});

    for(unsigned x = 0; x < SIZE; x++){
        for(unsigned y = 0; y < SIZE; y++){
            const float noise = noiseGenerator.perlin(
                (chunkX * SIZE + x), (chunkY * SIZE + y),
                PERSISTENCE, LACUNARITY, MAX_OCTAVES
            );
            image.setPixel({x, y}, mapNoiseToColour(noise));
        }
    }

    chunkTexture = new sf::Texture({SIZE, SIZE});
    chunkTexture->loadFromImage(image);
    chunkSprite = new sf::Sprite(*chunkTexture);
    chunkSprite->setPosition({static_cast<float>(chunkX * SIZE), static_cast<float>(chunkY * SIZE)});
}

Chunk::~Chunk() {delete chunkTexture; delete chunkSprite;}

sf::Sprite Chunk::getSprite() const {return *chunkSprite;}

sf::Color Chunk::mapNoiseToColour(const float elevation){
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

bool Chunk::isVisible(sf::View& view) const {
    const float viewWidth = view.getSize().x;
    const float viewHeight = view.getSize().y;

    const float viewLeft = view.getCenter().x - viewWidth / 2;
    const float viewRight = view.getCenter().x + viewWidth / 2;
    const float viewTop = view.getCenter().y - viewHeight / 2;
    const float viewBottom = view.getCenter().y + viewHeight / 2;

    const float chunkLeft = chunkX * SIZE;
    const float chunkRight = chunkX * SIZE + SIZE;
    const float chunkTop = chunkY * SIZE;
    const float chunkBottom = chunkY * SIZE + SIZE;

    return (
        viewLeft < chunkRight &&
        viewRight > chunkLeft &&
        viewTop < chunkBottom &&
        viewBottom > chunkTop
    );
}
