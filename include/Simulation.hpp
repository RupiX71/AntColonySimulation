#pragma once
#include <SFML/Graphics.hpp>
#include <Ant.hpp>
#include <vector>

class Simulation {
    
public:
    Simulation();
    ~Simulation();

    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    sf::RenderWindow window;
    sf::Clock clock;

    sf::Texture antTexture;
    std::vector<Ant> ants;
    
};