#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Passenger.h"

// Abstract base class for all station types.
// Subclasses (CircleStation, SquareStation, TriangleStation) implement
// getType() and draw() — polimorfism real in loc de if/else pe enum.
class Station
{
    sf::Vector2f position;
    std::vector<Passenger> passengers;
    float passengerSpawnTimer = 0.f;

public:
    explicit Station(const sf::Vector2f& position);

    // Rule of 5 — base class with virtual destructor; rest = default
    virtual ~Station() = default;
    Station(const Station&) = default;
    Station& operator=(const Station&) = default;
    Station(Station&&) = default;
    Station& operator=(Station&&) = default;

    // ── Pure virtual interface ────────────────────────────────────────────
    virtual StationType getType() const = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;

    // ── Common behaviour ─────────────────────────────────────────────────
    void update(float dt);

    sf::Vector2f getPosition() const;

    const std::vector<Passenger>& getPassengers() const;
    std::vector<Passenger>&       getPassengers();           // for boarding
};