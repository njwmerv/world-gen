#ifndef CHUNK_H
#define CHUNK_H
#include <SFML/Graphics.hpp>
#include "perlin.h"

class Chunk{
public:
    static constexpr int SIZE = 256; // Size of chunks (pixels)

    Chunk(int chunkX, int chunkY, const Perlin& noiseGenerator);

    ~Chunk();

    bool isVisible(sf::View& view) const;

    [[nodiscard]] sf::Sprite getSprite() const;

private:
    int chunkX;
    int chunkY;
    sf::Sprite* chunkSprite;
    sf::Texture* chunkTexture;

    static constexpr float PERSISTENCE = 2.0f;
    static constexpr float LACUNARITY = 0.005f;
    static constexpr int MAX_OCTAVES = 4;

    static sf::Color mapNoiseToColour(float elevation);
};

#endif //CHUNK_H
