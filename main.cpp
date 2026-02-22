#include <SFML/Graphics.hpp>
#include "systems/PhysicsWorld.h"
#include "entities/Ball.h"
#include "systems/Ground.h"
#include "systems/ContactListener.h"

int main()
{
	//sf::RenderWindow window(sf::VideoMode(800, 600), "Dark Ball"); // static window size
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    sf::RenderWindow window(
        desktop,
        "Dark Ball",
        sf::Style::Fullscreen
    );
    PhysicsWorld physics;
    ContactListener contactListener;
    physics.getWorld().SetContactListener(&contactListener);
    Ball ball(physics.getWorld());

    Ground ground(physics.getWorld(), 400.f, 550.f);
    sf::View view;
    view.setSize(window.getSize().x, window.getSize().y);
    view.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);
    window.setView(view);
    // Fixed timestep accumulator so physics runs in real time regardless of FPS.
    sf::Clock clock;
    const float timeStep = 1.f / 60.f;
    float accumulator = 0.f;

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

            // enter/exit ball spike mode on right mouse button press
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    ball.toggleSpikeMode();
                }
            }
        }

        float direction = 0.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            direction -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            direction += 1.0f;
        }

        // We'll apply player input during each physics step below so movement is
        // processed at the fixed physics rate (independent of render FPS).

        // accumulate real time and step physics in fixed increments
        float frameTime = clock.restart().asSeconds();
        // clamp frameTime to avoid spiral of death after a pause
        if (frameTime > 0.25f)
            frameTime = 0.25f;

        accumulator += frameTime;
        while (accumulator >= timeStep)
        {
            ball.move(direction);
            physics.step(timeStep);
            ball.update();
            accumulator -= timeStep;
        }

        // update camera to follow ball
        sf::Vector2f center = view.getCenter();
        center.x = ball.getPosition().x;
        view.setCenter(center);
        window.setView(view);

        window.clear(sf::Color(245, 245, 245));
        ground.draw(window);
        ball.draw(window);
        window.display();
    }
}