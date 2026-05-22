#include <SFML/Graphics.hpp>
#include "SimulationEngine.h"
#include "GameRenderer.h"
#include <iostream>


enum class GameState
{
    Menu,
    Playing
};

GameState gameState = GameState::Menu;


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

    sf::Texture menuTexture;

    menuTexture.loadFromFile(
        "assets/meniu.png"
    );

    sf::Sprite menuSprite(menuTexture);

    sf::FloatRect playButton(
     400.f,
     300.f,
     220.f,
     70.f
 );

    sf::FloatRect exitButton(
        400.f,
        385.f,
        200.f,
        70.f
    );


    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        totalTime += dt;

        if (gameState == GameState::Playing)
        {
            engine->update(dt);
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f mousePos(
                    static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y)
                );

                // =========================
                // MENU
                // =========================

                if (gameState == GameState::Menu)
                {
                    if (playButton.contains(mousePos))
                    {
                        gameState = GameState::Playing;
                    }

                    if (exitButton.contains(mousePos))
                    {
                        window.close();
                    }
                }

                // =========================
                // GAME
                // =========================

                else if (gameState == GameState::Playing)
                {
                    if (!engine->isGameOver())
                    {
                        if (event.mouseButton.button ==
                            sf::Mouse::Left)
                        {
                            engine->handleClick(mousePos);
                        }

                        else if (event.mouseButton.button ==
                                 sf::Mouse::Right)
                        {
                            engine->finishCurrentLine();
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(245, 245, 220));

        if (gameState == GameState::Menu)
        {

                window.draw(menuSprite);

        }
        else if (gameState == GameState::Playing)
        {
            renderer.render(window, *engine, font, totalTime);
        }

        window.display();
    }

    return 0;
}