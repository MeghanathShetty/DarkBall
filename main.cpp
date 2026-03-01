#include <SFML/Graphics.hpp>
#include "systems/PhysicsWorld.h"
#include "entities/Ball.h"
#include "systems/ContactListener.h"
#include "systems/Level.h"

int main()
{
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

    Level level;
    level.load("tutorial.json", physics.getWorld());

    // Camera
    sf::View view;
    view.setSize(window.getSize().x, window.getSize().y);
    view.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);
    window.setView(view);

    // Fixed timestep
    sf::Clock clock;
    const float timeStep = 1.f / 60.f;
    float accumulator = 0.f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                    ball.jump();
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                    ball.toggleSpikeMode();
            }
        }

        float direction = 0.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            direction -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            direction += 1.0f;

        float frameTime = clock.restart().asSeconds();
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

        // Camera follows ball (X and Y)
        view.setCenter(ball.getPosition());
        window.setView(view);

        // RENDER
        window.clear(sf::Color(245, 245, 245));

        level.draw(window);  // draw terrain FIRST
        ball.draw(window);   // then ball

        window.display();
    }

    return 0;
}