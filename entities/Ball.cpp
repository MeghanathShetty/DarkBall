#include "Ball.h"

Ball::Ball(b2World& world)
{
    SCALE = 30.f;   // 30 pixels = 1 meter

    // ---------------------------
    // Create Physics Body
    // ---------------------------

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;               // Dynamic = affected by gravity
    bodyDef.position.Set(400.f / SCALE, 100.f / SCALE);

    body = world.CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = 15.f / SCALE;         // radius in meters

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;                   // affects mass
    fixtureDef.friction = 0.3f;                  // sliding resistance
    fixtureDef.restitution = 0.2f;               // bounce

    body->CreateFixture(&fixtureDef);

    // ---------------------------
    // Create Visual Shape
    // ---------------------------

    shape.setRadius(15.f);
    shape.setOrigin(15.f, 15.f);
    shape.setFillColor(sf::Color::White);
}

void Ball::update()
{
    // Get position from physics world
    b2Vec2 position = body->GetPosition();
    float angle = body->GetAngle();

    // Convert meters → pixels
    shape.setPosition(position.x * SCALE, position.y * SCALE);

    // Convert radians → degrees
    shape.setRotation(angle * 180.f / b2_pi);
}

void Ball::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}