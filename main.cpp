#include <SFML/Graphics.hpp>
#include "systems/PhysicsWorld.h"
#include "entities/Ball.h"
#include "systems/Ground.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Dark Ball");

    PhysicsWorld physics;
    Ball ball(physics.getWorld());

    Ground ground(physics.getWorld(), 400.f, 550.f, 800.f, 40.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
			// handle window close event
            if (event.type == sf::Event::Closed)
                window.close();

			// handle ball jump on space key press
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    ball.jump();
                }
            }
        }

        physics.step();
        ball.update();

        window.clear(sf::Color(245, 245, 245));
		ground.draw(window);
        ball.draw(window);
        window.display();
    }
}