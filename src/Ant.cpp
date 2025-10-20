#include <Ant.hpp>
#include <cmath>
#include <cstdlib>

Ant::Ant(sf::Texture& texture, sf::Vector2f startPos)
: speed(100.0f),
  wanderStrength(1.0f),
  frameWidth(44),
  frameHeight(64),
  totalFrames(4),
  frameDuration(0.10f),
  currentFrame(0)
{
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    sprite.setPosition(startPos);
}

void Ant::update(float dt, sf::RenderWindow& window)
{
    wander(dt);
    move(dt);
    handleBounds(window);
    animate();
}

void Ant::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Ant::wander(float dt)
{
    float change = randomFloat(-wanderStrength, wanderStrength) * dt;
    float angle = std::atan2(direction.y, direction.x) + change;

    direction.x = std::cos(angle);
    direction.y = std::sin(angle);
    normalize(direction);
}

void Ant::move(float dt)
{
    sprite.move(direction * speed * dt);

    float angle = std::atan2(direction.y, direction.x);
    sprite.setRotation(angle * 180 / M_PI + 90);
}
void Ant::handleBounds(sf::RenderWindow& window) {
    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f bounds(window.getSize());

    if (pos.x < frameWidth / 2.f) {
        pos.x = frameWidth / 2.f;
        direction.x = std::abs(direction.x);
    }
    if (pos.x > bounds.x - frameWidth / 2.f) {
        pos.x = bounds.x - frameWidth / 2.f;
        direction.x = -std::abs(direction.x);
    }

    if (pos.y < frameHeight / 2.f) {
        pos.y = frameHeight / 2.f;
        direction.y = std::abs(direction.y);
    }
    if (pos.y > bounds.y - frameHeight / 2.f) {
        pos.y = bounds.y - frameHeight / 2.f;
        direction.y = -std::abs(direction.y);
    }
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

void Ant::normalize(sf::Vector2f& v)
{
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    if (len != 0.f) v /= len;
}