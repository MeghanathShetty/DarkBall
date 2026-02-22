#include "Ground.h"
#include <cstdlib>
#include "../core/Constants.h"

Ground::Ground(b2World& world, float startX, float startY)
{
    // Create static body
    b2BodyDef bodyDef;
    bodyDef.position.Set(startX / Constants::SCALE, startY / Constants::SCALE);
    body = world.CreateBody(&bodyDef);

    // Build terrain points
    float x = 0.f;
    float y = 0.f;

    // Flat start
    physicsVertices.push_back(b2Vec2(x / Constants::SCALE, y / Constants::SCALE));
    x += 400;
    physicsVertices.push_back(b2Vec2(x / Constants::SCALE, y / Constants::SCALE));

    // Slope up
    x += 300;
    y -= 150;
    physicsVertices.push_back(b2Vec2(x / Constants::SCALE, y / Constants::SCALE));

    // Flat top
    x += 300;
    physicsVertices.push_back(b2Vec2(x / Constants::SCALE, y / Constants::SCALE));

    // Slope down
    x += 300;
    y += 150;
    physicsVertices.push_back(b2Vec2(x / Constants::SCALE, y / Constants::SCALE));

    // Rugged humps
    for (int i = 0; i < 6; i++)
    {
        x += 200;
        y += (std::rand() % 200 - 100);
        physicsVertices.push_back(b2Vec2(x / Constants::SCALE, y / Constants::SCALE));
    }

    b2ChainShape chain;

    // Define ghost vertices
    b2Vec2 prevVertex = physicsVertices.front();
    b2Vec2 nextVertex = physicsVertices.back();

    // Slightly extend them to avoid zero-length ghost edges
    prevVertex.x -= 1.0f / Constants::SCALE;
    nextVertex.x += 1.0f / Constants::SCALE;

    chain.CreateChain(
        physicsVertices.data(),
        physicsVertices.size(),
        prevVertex,
        nextVertex
    );

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &chain;
    fixtureDef.friction = 0.8f;
    fixtureDef.restitution = 0.1f;

    body->CreateFixture(&fixtureDef);

    // Build visual line strip
    visual.setPrimitiveType(sf::LineStrip);
    visual.resize(physicsVertices.size());

    for (size_t i = 0; i < physicsVertices.size(); i++)
    {
        // physicsVertices are stored in Box2D local coordinates (meters) relative
        // to the static body's origin. Convert to world pixels by adding the
        // body's world position (in pixels).
        b2Vec2 bodyPos = body->GetPosition();
        float bodyPxX = bodyPos.x * Constants::SCALE;
        float bodyPxY = bodyPos.y * Constants::SCALE;

        visual[i].position = sf::Vector2f(
            bodyPxX + physicsVertices[i].x * Constants::SCALE,
            bodyPxY + physicsVertices[i].y * Constants::SCALE
        );
        visual[i].color = sf::Color::Black;
    }
}

void Ground::draw(sf::RenderWindow& window)
{
    // Recompute vertex positions relative to the body's world position in
    // case the view or body origin changes. This ensures the visual always
    // matches the physics chain.
    b2Vec2 bodyPos = body->GetPosition();
    float bodyPxX = bodyPos.x * Constants::SCALE;
    float bodyPxY = bodyPos.y * Constants::SCALE;

    for (size_t i = 0; i < physicsVertices.size(); ++i)
    {
        visual[i].position = sf::Vector2f(
            bodyPxX + physicsVertices[i].x * Constants::SCALE,
            bodyPxY + physicsVertices[i].y * Constants::SCALE
        );
        visual[i].color = sf::Color::Black;
    }

    window.draw(visual);
}