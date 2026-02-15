#include <SFML/Graphics.hpp>
#include "systems/PhysicsWorld.h"
#include "entities/Ball.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Dark Ball");

    PhysicsWorld physics;
    Ball ball(physics.getWorld());

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        physics.step();
        ball.update();

        window.clear();
        ball.draw(window);
        window.display();
    }
}