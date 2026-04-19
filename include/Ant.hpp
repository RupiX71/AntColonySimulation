/**
 * @file Ant.hpp
 * @brief Defines the Ant class using the integration of Semi-Implicit Euler's method for movement using
 * Newton's second law of motion. The wander behaviour method using the Craig Reynolds's steering behaviour
 * algorithm is implemented to give the ants a more natural movement pattern. The animation of 
 * the Ants is also handled in this class, using a sprite sheet and a clock to manage the animation frames.
 * 
 * TODO: Implement other behaviours like seeking, fleeing, and obstacle avoidance to make the ants more interactive with their environment.
 * 
 * @date 2025-10-30
 * @author rupixe
*/

#pragma once
#include <SFML/Graphics.hpp>
#include <World.hpp>
/**
 * @class Ant
 * @brief Manages the entire Ant behaviour
*/

class Ant {
    
public:
    Ant(sf::Texture& texture, sf::Vector2f startPos);

    void update(float dt, sf::Vector2f targetPosition, bool followMouse);
    void draw(sf::RenderWindow& window);

    void updateVertices(sf::Vertex* triangle) const;
    const sf::Sprite& getSprite() const { return sprite; }
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    sf::Vector2f getVelocity() const { return velocity; }
    sf::Vector2f getAcceleration() const { return acceleration; }

    // Debug things
    sf::Vector2f getWanderCenter() const { return wanderCenter; }
    sf::Vector2f getWanderTarget() const { return wanderTarget; }
    float getWanderRadius() const { return wanderRadius; }

private:
    sf::Sprite sprite;              // Sprite of the Ant

    sf::Vector2f velocity;          // Vector Velocity
    sf::Vector2f acceleration;      // Vector Acceleration
    sf::Vector2f wanderCenter;      // Just for debug thingies
    sf::Vector2f wanderTarget;      // Just for debug aswell

    float maxVelocity;              // Max Speed
    float maxAcceleration;          // Max Acceleration
    float steerStrength;            // Strength of steer
    float mass;                     // Mass of the Ant
    float wanderAngle;                  //
    float wanderDistance;               //
    float wanderRadius;                 //

    sf::Clock animationClock;   // Clock for the sprite animation
    int currentFrame;           // Frame counter for animation
    int totalFrames;            // Total frame of the animations
    float frameDuration;        // Frame duration of each animation
    int frameWidth;             // Frame width
    int frameHeight;            // Frame height

    sf::Vector2f seek(sf::Vector2f target);
    sf::Vector2f wander(float dt);

    void move();                                            // Move function
    void animate();                                         // Animate function

    float length(const sf::Vector2f &v);                    // Length Function
    sf::Vector2f normalize(const sf::Vector2f& v);          // Normalize Function
    float randomFloat(float min, float max);
};