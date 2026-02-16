#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
    : world(b2Vec2(0.0f, 0.275f))   // gravity
{
}

void PhysicsWorld::step()
{
    world.Step(1.f / 60.f, 8, 3);
}

b2World& PhysicsWorld::getWorld()
{
    return world;
}