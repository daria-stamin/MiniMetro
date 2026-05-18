#include "GameRenderer.h"
#include <cmath>

static void drawPassengerIcon(
    sf::RenderWindow& window,
    StationType type,
    const sf::Vector2f& center,
    float size,
    sf::Color color)
{
    if (type == StationType::Circle)
    {
        sf::CircleShape shape(size);
        shape.setFillColor(color);
        shape.setOrigin(size, size);
        shape.setPosition(center);
        window.draw(shape);
    }
    else if (type == StationType::Square)
    {
        sf::RectangleShape shape(sf::Vector2f(size * 2.f, size * 2.f));
        shape.setFillColor(color);
        shape.setOrigin(size, size);
        shape.setPosition(center);
        window.draw(shape);
    }
    else if (type == StationType::Triangle)
    {
        sf::CircleShape shape(size * 1.2f, 3);
        shape.setFillColor(color);
        shape.setOrigin(size * 1.2f, size * 1.2f);
        shape.setPosition(center);
        window.draw(shape);
    }
}

void GameRenderer::render(
    sf::RenderWindow& window,
    const SimulationEngine& engine)
{
    // ── Lines ────────────────────────────────────────────────────────────
    for (const auto& line : engine.getMetroLines())
    {
        const auto& stations = line->getStations();
        if (stations.size() < 2) continue;

        for (size_t i = 0; i < stations.size() - 1; ++i)
        {
            sf::Vector2f start = stations[i]->getPosition();
            sf::Vector2f end   = stations[i + 1]->getPosition();
            sf::Vector2f diff  = end - start;

            float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            float angle  = std::atan2(diff.y, diff.x) * 180.f / 3.14159265f;

            sf::RectangleShape lineShape(sf::Vector2f(length, 6.f));
            lineShape.setFillColor(line->getColor());
            lineShape.setPosition(start);
            lineShape.setRotation(angle);
            lineShape.setOrigin(0.f, 3.f);
            window.draw(lineShape);
        }
    }

    // ── Trains + passengers inside ───────────────────────────────────────
    for (const auto& line : engine.getMetroLines())
    {
        const auto& lineStations = line->getStations();
        if (lineStations.size() < 2) continue;

        for (const auto& train : line->getTrains())
        {
            sf::Vector2f start = lineStations[train.getCurrentStationIndex()]->getPosition();
            sf::Vector2f end   = lineStations[train.getNextStationIndex()]->getPosition();

            float t = train.getProgress();
            t = t * t * (3.f - 2.f * t);   // smoothstep

            sf::Vector2f trainPos = start + (end - start) * t;

            sf::Vector2f dir = end - start;
            float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;

            const float W = 48.f, H = 18.f;
            sf::RectangleShape body(sf::Vector2f(W, H));
            body.setFillColor(line->getColor());
            body.setOrigin(W / 2.f, H / 2.f);
            body.setPosition(trainPos);
            body.setRotation(angle);
            window.draw(body);

            // Passenger icons in a 3×2 grid, rotated with the train
            const auto& passengers = train.getPassengers();
            const size_t count = std::min(passengers.size(), size_t(6));

            float rad  = angle * 3.14159265f / 180.f;
            float cosA = std::cos(rad), sinA = std::sin(rad);
            float colOff[3] = { -14.f, 0.f, 14.f };
            float rowOff[2] = {  -4.f, 4.f };

            for (size_t i = 0; i < count; ++i)
            {
                float lx = colOff[i % 3], ly = rowOff[i / 3];
                sf::Vector2f iconPos = trainPos + sf::Vector2f(
                    lx * cosA - ly * sinA,
                    lx * sinA + ly * cosA
                );
                drawPassengerIcon(window, passengers[i].getDestinationType(),
                                  iconPos, 3.f, sf::Color(255, 255, 255, 220));
            }
        }
    }

    // ── Stations ─────────────────────────────────────────────────────────
    // Each station draws itself via the virtual draw() method.
    // No more if/else on StationType here.
    for (const auto& station : engine.getStations())
    {
        station->draw(window);

        const auto& passengers = station->getPassengers();
        for (size_t i = 0; i < passengers.size(); ++i)
        {
            sf::Vector2f iconPos(
                station->getPosition().x + 24.f + i * 14.f,
                station->getPosition().y
            );
            drawPassengerIcon(window, passengers[i].getDestinationType(),
                              iconPos, 5.f, sf::Color::Black);
        }
    }
}