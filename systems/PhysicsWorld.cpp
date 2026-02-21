#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
    : world(b2Vec2(0.0f, 9.8f))   // gravity (m/s^2)
{
}

void PhysicsWorld::step(float dt)
{
    // Forward variable timestep to Box2D. Caller should clamp dt to reasonable values.
    world.Step(dt, 8, 3);
}

b2World& PhysicsWorld::getWorld()
{
    return world;
}