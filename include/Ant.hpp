#pragma once
#include <SFML/Graphics.hpp>

class Ant {
public:
    Ant(sf::Texture& texture, sf::Vector2f startPos);
    void update(float dt, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    sf::Sprite sprite;

    sf::Vector2f direction;
    sf::Vector2f target;
    float speed;
    float wanderStrength;

    sf::Clock animationClock;
    int currentFrame;
    int totalFrames;
    float frameDuration;
    int frameWidth;
    int frameHeight;

    void wander(float dt);
    void move(float dt);
    void handleBounds(sf::RenderWindow& window);
    void animate();

    static float randomFloat(float min, float max);
    static void normalize(sf::Vector2f& v);
};