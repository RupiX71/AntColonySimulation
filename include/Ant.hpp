/**
 * @file Ant.hpp
 * @brief Defines the Ant class using the integration of Semi-Implicit Euler's method for movement using
 * Newton's second law of motion. The wander behaviour method using the Craig Reynolds's steering behaviour
 * algorithm is implemented to give the ants a more natural movement patter. The animation of 
 * the Ants is also handled in this class, using a sprite sheet and a clock to manage the animation frames.
 * 
 * TODO: Implement other behaviours like seeking, fleeing, and obstacle avoidance to make the ants more interactive with their environment.
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
    void update(float dt);
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
    float maxForce;             // Max 
    float wanderAngle;          // Angle of wandering
    float mass;                 // Mass of the Ant

    sf::Clock animationClock;   // Clock for the sprite animation
    int currentFrame;           // Frame counter for animation
    int totalFrames;            // Total frame of the animations
    float frameDuration;        // Frame duration of each animation
    int frameWidth;             // Frame width
    int frameHeight;            // Frame height

    void wander(float dt);                                  // Wander Function
    void move(float dt);                                    // Move function
    void calculateWanderForce(float dt);                    // Wander Force calculation function
    void steerTowards(const sf::Vector2f& desired);         // Steer function
    void applyForce(const sf::Vector2f& force);             // Force application function
    void integratePhysics(float dt);                        // Physics integration function
    void animate();                                         // Animate function

    static float randomFloat(float min, float max);         // Random value function 
    static sf::Vector2f normalized(const sf::Vector2f& v);  // Normalizing function
    static float length(const sf::Vector2f& v);            // Length function
};