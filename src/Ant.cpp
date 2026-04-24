#include <Ant.hpp>
#include <cmath>
#include <cstdlib>

Ant::Ant(sf::Texture& texture, sf::Vector2f startPos)
: frameWidth(44),                   // Frame width of one ant sprite    
  frameHeight(64),                  // Frame height of one ant sprite
  totalFrames(4),                   // Total number of sprite for animation
  frameDuration(0.2f),              // Duration of each frame
  currentFrame(0),                  // Current Frame
  maxVelocity(40.f),                // 40px/s is a reasonable speed for an ant in our simulation scale (1mm = 1px, so 40px/s = 40mm/s = 4cm/s which is a typical speed for an ant)
  maxAcceleration(25.f),            // Max acceleration (improve this explanation)
  steerStrength(6.f),               // Steer Strength
  mass(1.f),                        // Mass of the ant this could be changed
  wanderAngle(0.f),                 // Initial Wander Angle to zero
  wanderRadius(10.f),               //
  wanderDistance(10.f),             //
  velocity(0.f, 0.f),               // Initial Velocity to zero
  acceleration(0.f, 0.f)            // Initial acceleration to zero
{
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    sprite.setPosition(startPos);
    // Scale down the ant sprite since the ant is 64x44 pixels and 64*0.15 = 9.6 pixels and 44*0.10 = 3.5 pixels
    // And since a ant in the real world is around 1cm, we can consider 10 pixels as 1cm in our simulation,
    // so we scale it down to 0.15 making it around 9.6 pixels which is close enough to 10 pixels for our simulation scale.
    sprite.scale(sf::Vector2f(0.15f, 0.15f));
    sprite.setColor(sf::Color::Black);
}

float Ant::length(const sf::Vector2f &v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f Ant::normalize(const sf::Vector2f &v) {
    float len = length(v);
    if (len < 0.0001f) {
        return v;
    }
    else {
        return v / len;
    }
}

float Ant::randomFloat(float min, float max) {
    return min + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (max - min);
}
sf::Vector2f Ant::seek(sf::Vector2f target) {
    sf::Vector2f desiredDirection = target - getPosition();
    sf::Vector2f desiredVelocity = normalize(desiredDirection) * maxVelocity;

    return (desiredVelocity - velocity) * steerStrength;
}

sf::Vector2f Ant::wander(float dt) { // This dt is fixed aas 1/60.f defined in TIME_STEP
    float wanderJitter = 15.f * dt; 
    wanderAngle += randomFloat(-wanderJitter, wanderJitter);
    
    sf::Vector2f foward = normalize(velocity);

    wanderCenter = getPosition() + foward * wanderDistance;

    sf::Vector2f offset(std::cos(wanderAngle) * wanderRadius,
                        std::sin(wanderAngle) * wanderRadius);
    
    wanderTarget = wanderCenter + offset;

    return seek(wanderTarget);
}

void Ant::update(float dt, sf::Vector2f target, bool followMouse) {
    sf::Vector2f steeringForce;

    if (followMouse) {
        steeringForce = seek(target);
    }
    else {
        steeringForce = wander(dt);
    }

    if (length(steeringForce) > maxAcceleration) {
        steeringForce = normalize(steeringForce) * maxAcceleration;
    }

    acceleration = steeringForce / mass;
    velocity += acceleration * dt;

    if (length(velocity) > maxVelocity) {
        velocity = normalize(velocity) * maxVelocity;
    }

    sprite.move(velocity * dt);
    
    move(); 
    animate();
}

void Ant::move()
{
    if (length(velocity) > 0.001f) {
        float angle = std::atan2(velocity.y, velocity.x);
        sprite.setRotation(angle * 180.f / M_PI + 90.f);
    }
}

void Ant::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Ant::updateVertices(sf::Vertex* triangles) const{
    // Getting the sprite transformation for the transformation
    sf::Transform transform = sprite.getTransform();

    // Defining the 4 corners of the ant;
    float left   = 0;
    float right  = frameWidth;
    float top    = frameHeight;
    float bottom = 0;

    sf::Vector2f topLeft = transform.transformPoint(left, top);
    sf::Vector2f topRight = transform.transformPoint(right, top);
    sf::Vector2f bottomRight = transform.transformPoint(right, bottom);
    sf::Vector2f bottomLeft = transform.transformPoint(left, bottom);

    int tu = currentFrame * frameWidth;
    int tv = 0;

    sf::Vector2f texTopLeft(tu, tv + frameHeight);
    sf::Vector2f texTopRight(tu + frameWidth, tv + frameHeight);
    sf::Vector2f texBottomRight(tu + frameWidth, tv);
    sf::Vector2f texBottomLeft(tu ,tv);

    triangles[0].position = topLeft; triangles[0].texCoords = texTopLeft;
    triangles[1].position = topRight; triangles[1].texCoords = texTopRight;
    triangles[2].position = bottomRight; triangles[2].texCoords = texBottomRight;

    triangles[3].position = topLeft; triangles[3].texCoords = texTopLeft;
    triangles[4].position = bottomRight; triangles[4].texCoords = texBottomRight;
    triangles[5].position = bottomLeft; triangles[5].texCoords = texBottomLeft;

    for(int i = 0 ; i < 6 ; i++) {
        triangles[i].color = sf::Color::Black;
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