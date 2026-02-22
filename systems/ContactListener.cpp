#include "ContactListener.h"
#include "../entities/Ball.h"

void ContactListener::BeginContact(b2Contact* contact)
{
    auto* fixtureA = contact->GetFixtureA();
    auto* fixtureB = contact->GetFixtureB();

    Ball* ballA = reinterpret_cast<Ball*>(fixtureA->GetUserData().pointer);
    Ball* ballB = reinterpret_cast<Ball*>(fixtureB->GetUserData().pointer);

    if (ballA)
        ballA->setGrounded(true);

    if (ballB)
        ballB->setGrounded(true);
}

void ContactListener::EndContact(b2Contact* contact)
{
    auto* fixtureA = contact->GetFixtureA();
    auto* fixtureB = contact->GetFixtureB();

    Ball* ballA = reinterpret_cast<Ball*>(fixtureA->GetUserData().pointer);
    Ball* ballB = reinterpret_cast<Ball*>(fixtureB->GetUserData().pointer);

    if (ballA)
        ballA->setGrounded(false);

    if (ballB)
        ballB->setGrounded(false);
}