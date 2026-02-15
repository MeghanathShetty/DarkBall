#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class Ball
{
private:
    b2Body* body;                 // Physics body (Box2D)
    sf::CircleShape shape;        // Visual shape (SFML)

    float SCALE;                  // Pixels per meter

public:
    Ball(b2World& world);

    void update();                // Sync visual with physics
    void draw(sf::RenderWindow& window);
};