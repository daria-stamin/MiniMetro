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
        sf::Event event;

        while (window.pollEvent(event))
        {
            float dt = clock.restart().asSeconds();

            engine.update(dt);

            if (event.type == sf::Event::Closed){
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed){
                // LEFT CLICK
                if (event.mouseButton.button == sf::Mouse::Left){
                    sf::Vector2f mousePos( event.mouseButton.x, event.mouseButton.y);

                    engine.handleClick(mousePos);
                }

                // RIGHT CLICK
                else if ( event.mouseButton.button == sf::Mouse::Right){
                    engine.finishCurrentLine();
                }
            }
        }

        window.clear( sf::Color(245, 245, 220) );

        renderer.render(window, engine);

        window.display();
    }

    return 0;
}