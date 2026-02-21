#pragma once
#include <box2d/box2d.h>

class PhysicsWorld
{
private:
    b2World world;

public:
    PhysicsWorld();

    void step(float dt);

    b2World& getWorld();
};