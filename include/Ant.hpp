/**
 * @file Ant.hpp
 * @brief Defines the Ant class
 * 
 * @date 2025-10-30
 * @author rupixe
*/

#pragma once
#include <SFML/Graphics.hpp>

/**
 * @class Ant
 * @brief Manages the entire Ant behaviour
*/
class Ant {
public:
    Ant(sf::Texture& texture, sf::Vector2f startPos);
    void update(float dt, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    const sf::Sprite& getSprite() const { return sprite; }
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    sf::Vector2f getVelocity() const { return velocity; }
    sf::Vector2f getAcceleration() const { return acceleration; }

private:
    sf::Sprite sprite;          // Sprite of the Ant

    sf::Vector2f velocity;      // Vector Velocity
    sf::Vector2f acceleration;  // Vector Acceleration

    float maxSpeed;             // Max Speed
    float maxForce;             // Max Acceleration
    float wanderAngle;  // Angle of wandering

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