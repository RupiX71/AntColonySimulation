#pragma once
#include <SFML/Graphics.hpp>

class Ant {
public:
    Ant(sf::Texture& texture, sf::Vector2f startPos);
    void update(float dt, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    sf::Sprite sprite;          // Sprite of the Ant

    sf::Vector2f velocity;      // Vector Velocity
    sf::Vector2f acceleration;  // Vector Acceleration

    float wanderAngle;          // Angle of wandering
    float maxSpeed;             // Max Speed
    float maxForce;             // Max Force

    sf::Clock animationClock;
    int currentFrame;
    int totalFrames;
    float frameDuration;
    int frameWidth;
    int frameHeight;

    void wander(float dt);
    void move(float dt);
    void steerTowards(const sf::Vector2f& desired);
    void animate();

    static float randomFloat(float min, float max);
    static sf::Vector2f normalized(const sf::Vector2f& v);
};