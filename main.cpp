#include <SFML/Graphics.hpp>
#include "SimulationEngine.h"
#include "GameRenderer.h"
#include <iostream>

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(1280, 720),
        "MetroSim"
    );
    window.setFramerateLimit(144);

    auto engine = std::make_shared<SimulationEngine>();

    sf::Font font;
    if (!font.loadFromFile("data/arial.ttf"))
    {
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
            std::cerr << "[main] Could not load font — text will not render.\n";
    }

    GameRenderer renderer;
    sf::Clock clock;
    float totalTime = 0.f;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        totalTime += dt;

        engine->update(dt);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!engine->isGameOver() &&
                event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    engine->handleClick(sf::Vector2f(
                        static_cast<float>(event.mouseButton.x),
                        static_cast<float>(event.mouseButton.y)));
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    engine->finishCurrentLine();
                }
            }
        }

        window.clear(sf::Color(245, 245, 220));
        renderer.render(window, *engine, font, totalTime);
        window.display();
    }

    return 0;
}