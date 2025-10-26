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
    void timeCounter(float dt);

    sf::RenderWindow window;
    sf::Clock clock;
    sf::Font font;
    sf::Text timeText;
    
    float simulationTime = 0.f;

    sf::Texture antTexture;
    std::vector<Ant> ants;
};