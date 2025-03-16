#include "world.h"
#include <iostream>
#include <sstream>
#include <thread>

World::World(const int seed, unsigned viewWidth, unsigned viewHeight):
    worldOffsetX(50), worldOffsetY(50),
    window(sf::VideoMode({viewWidth, viewHeight}), "World"),
    noiseGenerator(seed, Chunk::SIZE, Chunk::SIZE),
    font{"./../Roboto-Regular.ttf"}, positionText{font}{
    window.setFramerateLimit(60);
    view.setCenter({
        static_cast<float>(worldOffsetX * Chunk::SIZE + viewWidth / 2),
        static_cast<float>(worldOffsetY * Chunk::SIZE + viewHeight / 2)
    });
    view.setSize({static_cast<float>(viewWidth), static_cast<float>(viewHeight)});
    window.setView(view);
    positionText.setCharacterSize(48);
    positionText.setFillColor(sf::Color::White);
    positionText.setOutlineColor(sf::Color::Black);
    positionText.setOutlineThickness(3);

    // Initial chunks
    generateChunks();
}

World::~World(){
    for(auto& chunk : chunksList) delete chunk;
}

bool World::update(){
    while(window.isOpen()){
        while(const std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
            if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                handleInput(keyPressed);
            }
        }
        window.setView(view);
        window.clear();

        // Generate new chunks
        generateChunks();

        // Draw chunks
        for(auto& chunk : chunksList){
            if(chunk->isVisible(view)) window.draw(chunk->getSprite());
        }

        // Create text for showing position
        try{displayPosition();}
        catch(std::exception& e){
            std::cerr << "Failed to render font: " << e.what() << std::endl;
            return false;
        }

        window.display();

        return true;
    }
    return false;
}

void World::generateChunks(){
    const auto t1 = std::chrono::high_resolution_clock::now();
    bool generated = false;
    const int nSectorsX = window.getSize().x / Chunk::SIZE + 1;
    const int nSectorsY = window.getSize().y / Chunk::SIZE + 1;
    std::vector<std::thread> threads;

    for(int x = -1; x < nSectorsX; x++){
        if(x + worldOffsetX < 0) continue;
        for(int y = -1; y < nSectorsY; y++){
            if(y + worldOffsetY < 0 || chunks.contains({x + worldOffsetX, y + worldOffsetY})) continue;
            std::cout << "Generating chunk at: " << (x + worldOffsetX) << ", " << (y + worldOffsetY) << std::endl;
            threads.emplace_back(&generateAChunk, this, x, y, std::ref(noiseGenerator));
            generated = true;
        }
    }

    for(auto& thread : threads){
        if(thread.joinable()) thread.join();
    }

    if(generated){
        const auto t2 = std::chrono::high_resolution_clock::now();
        const auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        std::cout << "Chunk generation time: " << ms_int.count() << " ms" << std::endl;
    }
}

void World::generateAChunk(const int x, const int y, const Perlin& noiseGenerator){
    Chunk* chunk =  new Chunk{x + worldOffsetX, y + worldOffsetY, noiseGenerator};
    chunksList.emplace_back(chunk);
    chunks.insert({x + worldOffsetX, y + worldOffsetY});
}

void World::handleInput(const sf::Event::KeyPressed* key){
    const float delta = clock.restart().asSeconds();
    const float movementSpeed = Chunk::SIZE * delta;
    float viewTop = view.getCenter().y - view.getSize().y / 2;
    float viewLeft = view.getCenter().x - view.getSize().x / 2;
    if(viewLeft - movementSpeed >= 0
        && (key->scancode == sf::Keyboard::Scan::A || key->scancode == sf::Keyboard::Scan::Left)){
        view.move({-movementSpeed, 0});
        viewLeft = view.getCenter().x - view.getSize().x / 2;
        worldOffsetX = static_cast<int>(viewLeft / Chunk::SIZE);
    }
    if(key->scancode == sf::Keyboard::Scan::D || key->scancode == sf::Keyboard::Scan::Right){
        view.move({movementSpeed, 0});
        viewLeft = view.getCenter().x - view.getSize().x / 2;
        worldOffsetX = static_cast<int>(viewLeft / Chunk::SIZE);
    }
    if(viewTop - movementSpeed >= 0
        && (key->scancode == sf::Keyboard::Scan::W || key->scancode == sf::Keyboard::Scan::Up)){
        view.move({0, -movementSpeed});
        viewTop = view.getCenter().y - view.getSize().y / 2;
        worldOffsetY = static_cast<int>(viewTop / Chunk::SIZE);
    }
    if(key->scancode == sf::Keyboard::Scan::S || key->scancode == sf::Keyboard::Scan::Down){
        view.move({0, movementSpeed});
        viewTop = view.getCenter().y - view.getSize().y / 2;
        worldOffsetY = static_cast<int>(viewTop / Chunk::SIZE);
    }
}

void World::displayPosition(){
    positionText.setPosition({view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2});
    std::ostringstream oss;
    oss << "Position: (" << worldOffsetX << ", " << worldOffsetY << ")";
    positionText.setString(oss.str());
    window.draw(positionText);
}
