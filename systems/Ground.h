#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class Ground
{
private:
    b2Body* body;                 // Physics body
    sf::RectangleShape shape;     // Visual shape

    float SCALE;

public:
    Ground(b2World& world,
        float posX,
        float posY,
        float width,
        float height);

    void draw(sf::RenderWindow& window);
};