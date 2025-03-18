#include "chunk.h"
#include <mutex>
#include <thread>
#include <SFML/Graphics.hpp>

void Chunk::generatePartialChunk(const Perlin& noiseGenerator, sf::Image& image, std::mutex& mutex,
    const int t, const int b, const int l, const int r) const {
    std::vector<std::vector<sf::Color>> pixels;
    for(unsigned x = l; x < r; x++){
        std::vector<sf::Color> newRow;
        for(unsigned y = t; y < b; y++){
            const float noise = noiseGenerator.perlin(
                (chunkX * SIZE + x), (chunkY * SIZE + y),
                PERSISTENCE, LACUNARITY, MAX_OCTAVES
            );
            newRow.emplace_back(mapNoiseToColour(noise));
        }
        pixels.emplace_back(newRow);
    }
    std::lock_guard<std::mutex> lock(mutex);
    for(unsigned x = l; x < r; x++){
        for(unsigned y = t; y < b; y++){
            image.setPixel({x, y}, pixels.at(x - l).at(y - t));
        }
    }
}

Chunk::Chunk(const int chunkX, const int chunkY, const Perlin& noiseGenerator) :
    chunkX(chunkX), chunkY(chunkY), chunkSprite(nullptr), chunkTexture(nullptr)
{
    sf::Image image({SIZE, SIZE});
    std::vector<std::thread> threads;
    std::mutex mutex;

    const int numVerticalDivisions = 2; // make it a factor of Chunk::SIZE if you don't want grid lines
    const int numHorizontalDivisions = 2;

    for(unsigned x = 0; x < numHorizontalDivisions; x++){
        for(unsigned y = 0; y < numVerticalDivisions; y++){
            threads.emplace_back(&Chunk::generatePartialChunk,
                this, std::ref(noiseGenerator), std::ref(image), std::ref(mutex),
                y * (SIZE / numVerticalDivisions), (y + 1) * (SIZE / numVerticalDivisions),
                x * (SIZE / numHorizontalDivisions), (x + 1) * (SIZE / numHorizontalDivisions));
        }
    }

    for(auto& thread : threads){
        if(thread.joinable()) thread.join();
    }

    chunkTexture = new sf::Texture({SIZE, SIZE});
    chunkTexture->loadFromImage(image);
    chunkSprite = new sf::Sprite(*chunkTexture);
    chunkSprite->setPosition({static_cast<float>(chunkX * SIZE), static_cast<float>(chunkY * SIZE)});
}

Chunk::~Chunk() {delete chunkTexture; delete chunkSprite;}

sf::Sprite Chunk::getSprite() const {return *chunkSprite;}

sf::Color Chunk::mapNoiseToColour(const float elevation){
    if(elevation < 0.225f){ // Deep water
        return {0, 0, 150};
    }
    else if(elevation < 0.4f){ // Shallow water
        return {0, 100, 200};
    }
    else if(elevation < 0.43f){ // Beach
        return {240, 220, 130};
    }
    else if(elevation < 0.62f){ // Grass
        return {34, 139, 34};
    }
    else if(elevation < 0.77f){ // Forest
        return {13, 105, 12};
    }
    else if(elevation < 0.85){
        return {100, 100, 100};
    }
    else if(elevation < 0.92f){ // Mountain
        return {151, 153, 152};
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
