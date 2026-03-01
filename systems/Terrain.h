#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "../core/Constants.h"

class Terrain
{
public:
    Terrain(b2World& world,
        const std::vector<sf::Vector2f>& points);

    void draw(sf::RenderWindow& window);

private:
    b2Body* body = nullptr;
    std::vector<sf::Vector2f> pixelPoints;
    sf::VertexArray visual;
};