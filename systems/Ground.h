#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>

class Ground
{
private:
    b2Body* body;

    std::vector<b2Vec2> physicsVertices;
    sf::VertexArray visual;

public:
    Ground(b2World& world, float startX, float startY);

    void draw(sf::RenderWindow& window);
};