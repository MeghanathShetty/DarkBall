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
    circleShape.m_radius = 30.f / SCALE;         // radius in meters // physics

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;                   // affects mass
    fixtureDef.friction = 0.3f;                  // sliding resistance
    fixtureDef.restitution = 0.2f;               // bounce

    body->CreateFixture(&fixtureDef);

    // ---------------------------
    // Create Visual Shape
    // ---------------------------

    shape.setRadius(30.f);                 // visual
    shape.setOrigin(30.f, 30.f);
    shape.setFillColor(sf::Color::Black);
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

void Ball::jump()
{
    b2Vec2 velocity = body->GetLinearVelocity();

    // Only allow jump if nearly not moving vertically
    if (abs(velocity.y) < 0.01f)
    {
        float impulseStrength = -10.0f;  // negative = upward

        body->ApplyLinearImpulse(
            b2Vec2(0.0f, impulseStrength),
            body->GetWorldCenter(),
            true
        );
    }
}