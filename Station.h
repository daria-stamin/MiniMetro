#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

#include "Passenger.h"

class Station
{
private:
    StationType type;
    sf::Color color;
    sf::Vector2f position;

    std::vector<Passenger> passengers;

    float passengerSpawnTimer;

public:
    Station(
        StationType type,
        const sf::Vector2f& position
    );

    void update(float dt);

    StationType getType() const;

    sf::Vector2f getPosition() const;

    const std::vector<Passenger>& getPassengers() const;

    void addPassenger(const Passenger& passenger);

    sf::Color getColor() const;
};