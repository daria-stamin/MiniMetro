#include <SFML/Graphics.hpp>

#include "SimulationEngine.h"
#include "GameRenderer.h"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(1280, 720),
        "MetroSim"
    );
    window.setFramerateLimit(144);
    sf::Clock clock;

    SimulationEngine engine;

    GameRenderer renderer;

    while (window.isOpen())
    {
        // FIX: dt computed ONCE per frame, before the event loop.
        // Previously this was inside pollEvent, causing update() to be
        // called multiple times per frame (once per event) or not at all
        // (when there are no events).
        float dt = clock.restart().asSeconds();

        engine.update(dt);

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                // LEFT CLICK — add station to current line (or start new line)
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos(
                        static_cast<float>(event.mouseButton.x),
                        static_cast<float>(event.mouseButton.y)
                    );

                    engine.handleClick(mousePos);
                }

                // RIGHT CLICK — finish current line and spawn train
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    engine.finishCurrentLine();
                }
            }
        }

        window.clear(sf::Color(245, 245, 220));

        renderer.render(window, engine);

        window.display();
    }

    return 0;
}