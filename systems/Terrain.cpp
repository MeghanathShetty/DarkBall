#include "Terrain.h"
#include <iostream>

Terrain::Terrain(b2World& world,
    const std::vector<sf::Vector2f>& points)
{
    pixelPoints = points;

    if (pixelPoints.size() < 2)
        return;

    b2BodyDef bodyDef;
    body = world.CreateBody(&bodyDef);

    std::vector<b2Vec2> physicsPoints;
    physicsPoints.reserve(pixelPoints.size());

    for (auto& p : pixelPoints)
    {
        physicsPoints.emplace_back(
            p.x / Constants::SCALE,
            p.y / Constants::SCALE
        );
    }

	if (pixelPoints.size() >= 3) // minimum 3 points for polygon (a closed shape)
    {
        // ---- Use Box2D Polygon ----
        // All are convex here, because concave polygons are
        // triangulated into convex ones in Level::load()
		b2PolygonShape polygon;
        polygon.Set(physicsPoints.data(),
            static_cast<int32>(physicsPoints.size()));

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &polygon;
        fixtureDef.friction = 0.8f;
        fixtureDef.restitution = 0.1f;

        body->CreateFixture(&fixtureDef);

        // ---- Build Filled Visual ---- 
        filledShape.setPointCount(pixelPoints.size());
        for (size_t i = 0; i < pixelPoints.size(); ++i)
            filledShape.setPoint(i, pixelPoints[i]);

        filledShape.setFillColor(sf::Color::Black);
    }
    else {
		std::cout << "WARNING: Recieved an"
            "terrain with less than 3 points, terrain ignored." << std::endl;
    }
}

void Terrain::draw(sf::RenderWindow& window)
{
    window.draw(filledShape);
}