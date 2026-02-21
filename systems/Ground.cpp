#include "Ground.h"

Ground::Ground(b2World& world,
    float posX,
    float posY,
    float width,
    float height)
{
    SCALE = 30.f;

    // Physics Body (Static)
    b2BodyDef bodyDef;
    bodyDef.position.Set(posX / SCALE, posY / SCALE);

    body = world.CreateBody(&bodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox((width / 2.f) / SCALE,
        (height / 2.f) / SCALE);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.6f;
    fixtureDef.restitution = 0.2f;

    body->CreateFixture(&fixtureDef);

    // Visual Shape
    shape.setSize(sf::Vector2f(width, height));
    shape.setOrigin(width / 2.f, height / 2.f);
    shape.setPosition(posX, posY);
    shape.setFillColor(sf::Color::Black);
}

void Ground::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}
