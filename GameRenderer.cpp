#include "GameRenderer.h"

#include <cmath>

void GameRenderer::render(sf::RenderWindow& window,const SimulationEngine& engine){
    // =====================
    // DESENAM LINIILE
    // =====================

    for (const auto& line : engine.getMetroLines()){
        const auto& stations = line->getStations();

        for (size_t i = 0;i < stations.size() - 1;i++){
            sf::Vector2f start = stations[i]->getPosition();
            sf::Vector2f end = stations[i + 1]->getPosition();

            sf::Vector2f diff = end - start;

            float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);

            sf::RectangleShape lineShape(sf::Vector2f(length, 6.f));

            lineShape.setFillColor(line->getColor());

            lineShape.setPosition(start);

            float angle = std::atan2(diff.y, diff.x) * 180.f / 3.14159265f;

            lineShape.setRotation(angle);

            lineShape.setOrigin(0.f, 3.f);

            window.draw(lineShape);
        }
    }

    for (const auto& line : engine.getMetroLines()){
        const auto& lineStations = line->getStations();

        for (const auto& train : line->getTrains()) {
            if (lineStations.size() < 2){
                continue;
            }

            int currentIndex = train.getCurrentStationIndex();

            int nextIndex = train.getNextStationIndex();

            sf::Vector2f start = lineStations[currentIndex]->getPosition();

            sf::Vector2f end = lineStations[nextIndex] ->getPosition();
            float t = train.getProgress();

            // SMOOTHSTEP
            t = t * t * (3.f - 2.f * t);

            sf::Vector2f trainPosition =
                start +
                (end - start) * t;
            sf::RectangleShape trainShape( sf::Vector2f(30.f, 16.f));

            trainShape.setFillColor(line->getColor());

            trainShape.setOrigin( 15.f, 8.f );

            trainShape.setPosition(trainPosition);

            sf::Vector2f direction = end - start;

            float angle = std::atan2(
                direction.y,
                direction.x
            ) * 180.f / 3.14159265f;

            trainShape.setRotation(angle);

            window.draw(trainShape);

        }
    }

    // =====================
    // DESENAM STATIILE
    // =====================

    for (const auto& station :
         engine.getStations())
    {
        if (station->getType() == StationType::Circle){
            sf::CircleShape shape(14.f);

            shape.setFillColor( sf::Color(250, 250, 250) );

            shape.setOutlineThickness(5.f);

            shape.setOutlineColor( sf::Color::Black );

            shape.setOrigin(14.f, 14.f);

            shape.setPosition( station->getPosition() );

            window.draw(shape);
        }

        else if (station->getType() == StationType::Square)
        {
            sf::RectangleShape shape(sf::Vector2f(28.f, 28.f));

            shape.setFillColor(sf::Color(250, 250, 250));

            shape.setOutlineThickness(5.f);

            shape.setOutlineColor(sf::Color::Black);

            shape.setOrigin(14.f, 14.f);

            shape.setPosition(station->getPosition());

            window.draw(shape);
        }

        else if (station->getType() == StationType::Triangle){
            sf::CircleShape shape(18.f, 3);

            shape.setFillColor(sf::Color(250, 250, 250));

            shape.setOutlineThickness(5.f);

            shape.setOutlineColor(sf::Color::Black);

            shape.setOrigin(18.f, 18.f);

            shape.setPosition(station->getPosition());

            window.draw(shape);
        }
        const auto& passengers =
    station->getPassengers();

        for (size_t i = 0;
      i < passengers.size();
      i++)
        {
            sf::Vector2f passengerPosition(
                station->getPosition().x
                + 24.f
                + i * 12.f,

                station->getPosition().y
            );

            StationType destinationType =
                passengers[i].getDestinationType();

            // =====================
            // CERC
            // =====================

            if (destinationType ==
                StationType::Circle)
            {
                sf::CircleShape shape(5.f);

                shape.setFillColor(
                    sf::Color::Black
                );

                shape.setOrigin(5.f, 5.f);

                shape.setPosition(
                    passengerPosition
                );

                window.draw(shape);
            }

            // =====================
            // PATRAT
            // =====================

            else if (destinationType ==
                     StationType::Square)
            {
                sf::RectangleShape shape(
                    sf::Vector2f(10.f, 10.f)
                );

                shape.setFillColor(
                    sf::Color::Black
                );

                shape.setOrigin(5.f, 5.f);

                shape.setPosition(
                    passengerPosition
                );

                window.draw(shape);
            }

            // =====================
            // TRIUNGHI
            // =====================

            else if (destinationType ==
                     StationType::Triangle)
            {
                sf::CircleShape shape(7.f, 3);

                shape.setFillColor(
                    sf::Color::Black
                );

                shape.setOrigin(7.f, 7.f);

                shape.setPosition(
                    passengerPosition
                );

                shape.setRotation(180.f);

                window.draw(shape);
            }
        }


    }

}