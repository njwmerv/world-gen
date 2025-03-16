#ifndef WORLD_H
#define WORLD_H
#include <set>
#include <SFML/Graphics.hpp>
#include "chunk.h"
#include "perlin.h"

class World {
public:
    int worldOffsetX;
    int worldOffsetY;
    sf::Clock clock;
    sf::RenderWindow window;
    sf::View view;
    Perlin noiseGenerator;
    std::vector<Chunk*> chunksList;
    std::set<std::pair<int, int>> chunks;
    sf::Font font;
    sf::Text positionText;

    static constexpr int MOVEMENT_SPEED = 64;

    World(int seed, unsigned viewWidth, unsigned viewHeight);

    ~World();

    bool update();

private:
    void handleInput(const sf::Event::KeyPressed* key);

    void generateChunks();

    void displayPosition();
};

#endif //WORLD_H
