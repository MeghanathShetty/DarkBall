#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class Ball
{
private:
    b2Body* body; // The Box2D body that handles the ball’s movement and physics behavior.
    b2Fixture* fixture; // The fixture attached to the body that defines mass, friction, and bounce.
    sf::CircleShape shape;        // Visual shape (SFML)
    sf::Color color;
    float radius;

    float SCALE;                  // Pixels per meter
    bool spikeMode;

    // Animations
    float spikeProgress;     // 0.0 -> 1.0
    float spikeSpeed;        // how fast spikes grow

public:
    Ball(b2World& world);

    void update();                // Sync visual with physics
    void draw(sf::RenderWindow& window);
	void jump();                // Apply an impulse to make the ball jump
    void move(float direction);
    void toggleSpikeMode();
	void drawSpikes(sf::RenderWindow& window);
    void spikeyMode(b2Fixture* fixDef);
    void bouncyMode(b2Fixture* fixDef); // normal mode
};