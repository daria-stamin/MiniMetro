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
        "MiniMetro"
    );
    window.setFramerateLimit(144);

    auto engine = std::make_shared<SimulationEngine>();

    sf::Font font;
    if (!font.loadFromFile("assets/MontserratL.ttf"))
    {
        if (!font.loadFromFile("C:/Windows/Fonts/MontserratL.ttf"))
            std::cerr << "[main] Could not load font — text will not render.\n";
    }


    sf::Image icon;

    if (icon.loadFromFile("assets/icon.png"))
    {
        window.setIcon(
            icon.getSize().x,
            icon.getSize().y,
            icon.getPixelsPtr()
        );
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



    // ================= CLOCK =================

    // cerc exterior (rama)
    sf::CircleShape clockFrame(40.f);
    clockFrame.setFillColor(sf::Color::White);
    clockFrame.setOutlineColor(sf::Color(62, 62, 62));
    clockFrame.setOutlineThickness(7.f);
    clockFrame.setPosition(1180.f, 10.f);

    // minutar
    sf::RectangleShape minuteHand(sf::Vector2f(30.f, 3.f));
    minuteHand.setFillColor(sf::Color::Black);

    // orar
    sf::RectangleShape hourHand(sf::Vector2f(18.f, 4.f));
    hourHand.setFillColor(sf::Color::Black);


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

        window.clear(sf::Color(232, 232, 232));

        if (gameState == GameState::Menu)
        {
                window.draw(menuSprite);

        }
        else if (gameState == GameState::Playing)
        {
            renderer.render(window, *engine, font, totalTime);
        }

        if (gameState == GameState::Playing)
        {
            float minutesAngle = totalTime * 50.f;
            float hoursAngle = totalTime * 10.f;

            sf::Vector2f center(
                clockFrame.getPosition().x + 40.f,
                clockFrame.getPosition().y + 40.f
            );

            // minutar
            minuteHand.setPosition(center);
            minuteHand.setOrigin(0.f, 1.5f);
            minuteHand.setRotation(minutesAngle);

            // orar
            hourHand.setPosition(center);
            hourHand.setOrigin(0.f, 2.f);
            hourHand.setRotation(hoursAngle);

            // desenare
            window.draw(clockFrame);
            window.draw(hourHand);
            window.draw(minuteHand);

            sf::CircleShape centerDot(4.f);
            centerDot.setFillColor(sf::Color::Black);
            centerDot.setPosition(center.x - 4.f, center.y - 4.f);

            window.draw(centerDot);
        }

        window.display();
    }

    return 0;
}