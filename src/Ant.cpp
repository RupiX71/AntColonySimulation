#include <Ant.hpp>
#include <cmath>
#include <cstdlib>

Ant::Ant(sf::Texture& texture, sf::Vector2f startPos)
: maxSpeed(100.f),
  maxForce(20.f),
  velocity(0.f, 0.f),
  acceleration(0.f, 0.f),
  frameWidth(44),
  frameHeight(64),
  totalFrames(4),
  frameDuration(0.15f),
  currentFrame(0)
{
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    sprite.setPosition(startPos);
    sprite.scale(sf::Vector2f(1.f, 1.f));
}

void Ant::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Ant::update(float dt, sf::RenderWindow& window)
{
    acceleration = sf::Vector2f(0.f,0.f);

    calculateWanderForce(dt);

    integratePhysics(dt);

    move(dt);

    animate();
}

void Ant::calculateWanderForce(float dt)
{
    float circleDistance = maxSpeed;
    float circleRadius = 100.f;
    float angleChange = M_PI/6.f;
    

    sf::Vector2f circleCenter = normalized(velocity) * circleDistance;

    wanderAngle += randomFloat(-angleChange, angleChange);

    sf::Vector2f displacement(std::cos(wanderAngle) * circleRadius, std::sin(wanderAngle) * circleRadius);
    sf::Vector2f wanderForce = circleCenter + displacement;

    steerTowards(wanderForce);
}

void Ant::steerTowards(const sf::Vector2f& desired) {
    sf::Vector2f steer = normalized(desired) * maxSpeed;
    sf::Vector2f force = normalized(steer - velocity) * maxForce;
    acceleration += force;
}

void Ant::integratePhysics(float dt) {
    velocity += acceleration * dt;

    if (std::abs(velocity.x) > maxSpeed || std::abs(velocity.y) > maxSpeed) {
        velocity = normalized(velocity) * maxSpeed;
    }
}

void Ant::move(float dt)
{
    sprite.move(velocity * dt);
    float angle = std::atan2(velocity.y, velocity.x);
    sprite.setRotation(angle * 180 / M_PI + 90);
}

void Ant::animate()
{
    if (animationClock.getElapsedTime().asSeconds() > frameDuration)
    {
        currentFrame = (currentFrame + 1) % totalFrames;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
        animationClock.restart();
    }
}

float Ant::randomFloat(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

sf::Vector2f Ant::normalized(const sf::Vector2f& v)
{
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    if (len != 0.f) return v / len;
    return {0.f, 0.f};
}