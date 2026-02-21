#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class Ball
{
private:
    b2Body* body;                 // Physics body (Box2D)
    sf::CircleShape shape;        // Visual shape (SFML)
    sf::Color color;
    float radius;

    float SCALE;                  // Pixels per meter
    bool spikeMode;

public:
    Ball(b2World& world);

    void update();                // Sync visual with physics
    void draw(sf::RenderWindow& window);
	void jump();                // Apply an impulse to make the ball jump
    void move(float direction);
    void toggleSpikeMode();
	void drawSpikes(sf::RenderWindow& window);
};