#include "Terrain.h"

Terrain::Terrain(b2World& world,
    const std::vector<sf::Vector2f>& points)
{
    pixelPoints = points;

    // Validate input
    if (pixelPoints.size() < 2)
    {
        // Not enough points to build a chain; leave visual empty and return.
        visual.clear();
        visual.setPrimitiveType(sf::Lines);
        return;
    }

    // Create static body
    b2BodyDef bodyDef;
    body = world.CreateBody(&bodyDef);

    // Convert to Box2D meters
    std::vector<b2Vec2> physicsPoints;
    physicsPoints.reserve(pixelPoints.size());
    for (auto& p : pixelPoints)
    {
        physicsPoints.emplace_back(
            p.x / Constants::SCALE,
            p.y / Constants::SCALE
        );
    }

    b2ChainShape chain;
    b2Vec2 prev = physicsPoints.front();
    b2Vec2 next = physicsPoints.back();

    // Slightly extend to avoid zero-length ghost edges
    prev.x -= 0.01f;
    next.x += 0.01f;

    chain.CreateChain(physicsPoints.data(),
        static_cast<int32>(physicsPoints.size()),
        prev,
        next);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &chain;
    fixtureDef.friction = 0.8f;
    fixtureDef.restitution = 0.1f;
    fixtureDef.userData.pointer = 0;

    body->CreateFixture(&fixtureDef);

    // Build visual
    visual.setPrimitiveType(sf::LineStrip);
    visual.resize(pixelPoints.size());

    for (size_t i = 0; i < pixelPoints.size(); i++)
    {
        visual[i].position = pixelPoints[i];
        visual[i].color = sf::Color::Black;
    }
}

void Terrain::draw(sf::RenderWindow& window)
{
    window.draw(visual);
}