#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Passenger.h"
#include "IGameObserver.h"

class Station
{
    sf::Vector2f           position;
    std::vector<Passenger> passengers;
    float                  passengerSpawnTimer = 0.f;

    // Observer — weak_ptr breaks ownership cycle:
    // SimulationEngine owns Stations (shared_ptr<Station>),
    // Stations must NOT own SimulationEngine back.
    std::vector<std::weak_ptr<IGameObserver>> observers;

    // Overcrowd visual warning
    bool  overcrowded       = false;
    float overcrowdTimer    = 0.f;
    bool  overcrowdNotified = false;  // fire the event only once

public:
    explicit Station(const sf::Vector2f& position);

    virtual ~Station() = default;
    Station(const Station&)            = default;
    Station& operator=(const Station&) = default;
    Station(Station&&)                 = default;
    Station& operator=(Station&&)      = default;

    // ── Pure virtual interface ────────────────────────────────────────────
    virtual StationType getType()                      const = 0;
    virtual void        draw(sf::RenderWindow& window) const = 0;

    // ── Common behaviour ─────────────────────────────────────────────────
    void update(float dt);

    sf::Vector2f getPosition() const;

    const std::vector<Passenger>& getPassengers() const;
    std::vector<Passenger>&       getPassengers();

    // ── Observer subject ─────────────────────────────────────────────────
    void addObserver(std::weak_ptr<IGameObserver> observer);

    // ── Overcrowd state (read by GameRenderer for visual effect) ─────────
    bool  isOvercrowded()     const;
    float getOvercrowdTimer() const;

private:
    void notifyObservers();
};