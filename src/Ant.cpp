#include <Ant.hpp>
#include <cmath>
#include <cstdlib>

Ant::Ant(sf::Texture& texture, sf::Vector2f startPos)
: maxSpeed(60.f),           // 60px/s is a reasonable speed for an ant in our simulation scale (1mm = 1px, so 60px/s = 60mm/s = 6cm/s which is a typical speed for an ant)
  maxForce(25.f),           // Max force their muscles  can exert, this is a bit arbitrary but should allow for responsive movement without being too twitchy
  mass(1.0f),               // Bigger = more effort to turn
  velocity(0.f, 0.f),       
  acceleration(0.f, 0.f),
  wanderAngle(0.f),
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
    // Scale down the ant sprite since the ant is 64x64 pixels and 64*0.15 = 9.6 pixels
    // And since a ant in the real world is around 1cm, we can consider 10 pixels as 1cm in our simulation,
    // so we scale it down to 0.15 making it around 9.6 pixels which is close enough to 10 pixels for our simulation scale.
    sprite.scale(sf::Vector2f(0.15f, 0.15f));

    wanderAngle = randomFloat(0.f, M_PI * 2.f);
    velocity.x = std::cos(wanderAngle) * (maxSpeed/4);
    velocity.x = std::sin(wanderAngle) * (maxSpeed/4);
}

float Ant::length(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f Ant::normalized(const sf::Vector2f& v)
{
    float len = length(v);
    if (len != 0.f) return v / len;
    return {0.f, 0.f};
}

float Ant::randomFloat(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

void Ant::update(float dt)
{
    acceleration = sf::Vector2f(0.f,0.f);

    calculateWanderForce(dt);

    integratePhysics(dt);

    move(dt);

    animate();
}

void Ant::applyForce(const sf::Vector2f& force) {
    // F = M * a -> a = F/M
    acceleration += (force / mass);
}

void Ant::steerTowards(const sf::Vector2f& desiredDirection) {
    // The velocity the ant would like
    sf::Vector2f desiredVelocity = normalized(desiredDirection) * maxSpeed;

    // The ammount of force needed to correct the route
    sf::Vector2f steeringForce = desiredVelocity - velocity;

    if (length(steeringForce) > maxForce) {
        steeringForce = normalized(steeringForce) * maxForce;
    }

    applyForce(steeringForce);
}

void Ant::integratePhysics(float dt) {
    velocity += acceleration * dt;

    if (length(velocity) > maxSpeed) {
        velocity = normalized(velocity) * maxSpeed;
    }
}

void Ant::calculateWanderForce(float dt)
{
    // This is the parameters of the behaviour of the ant
    float circleDistance = 50.f;        // How far the circle ahead
    float circleRadius = 40.f;          // How large the circle is
    float jitter = 1.f;                // How many idea changes in a second
    
    sf::Vector2f circleCenter = normalized(velocity) * circleDistance;

    wanderAngle += randomFloat(-jitter, jitter) * dt;

    sf::Vector2f displacement(std::cos(wanderAngle) * circleRadius,
                              std::sin(wanderAngle) * circleRadius);

    sf::Vector2f targetDirection = circleCenter + displacement;

    steerTowards(targetDirection);
}

void Ant::move(float dt)
{
    sprite.move(velocity * dt);
    float angle = std::atan2(velocity.y, velocity.x);
    sprite.setRotation(angle * 180 / M_PI + 90);
}

void Ant::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
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