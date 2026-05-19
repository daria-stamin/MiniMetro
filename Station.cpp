#include "Station.h"
#include "GameConfig.h"
#include <random>
#include <algorithm>

Station::Station(const sf::Vector2f& pos)
    : position(pos) {}

sf::Vector2f Station::getPosition() const { return position; }

const std::vector<Passenger>& Station::getPassengers() const { return passengers; }
std::vector<Passenger>&       Station::getPassengers()       { return passengers; }

bool  Station::isOvercrowded()     const { return overcrowded;    }
float Station::getOvercrowdTimer() const { return overcrowdTimer; }

void Station::addObserver(std::weak_ptr<IGameObserver> observer)
{
    observers.push_back(std::move(observer));
}

void Station::notifyObservers()
{
    // Remove expired observers, then notify the rest
    observers.erase(
        std::remove_if(observers.begin(), observers.end(),
            [](const std::weak_ptr<IGameObserver>& wp) { return wp.expired(); }),
        observers.end());

    for (auto& wp : observers)
        if (auto obs = wp.lock())
            obs->onStationOvercrowded(this);
}

void Station::update(float dt)
{
    // ── Spawn new passenger every 5 seconds ──────────────────────────────
    passengerSpawnTimer += dt;
    if (passengerSpawnTimer >= 5.f)
    {
        passengerSpawnTimer = 0.f;

        static std::mt19937 gen{ std::random_device{}() };
        static std::uniform_int_distribution<int> dist(0, 1);

        StationType others[2];
        int idx = 0;
        for (int i = 0; i < 3; ++i)
        {
            auto candidate = static_cast<StationType>(i);
            if (candidate != getType())
                others[idx++] = candidate;
        }
        passengers.emplace_back(others[dist(gen)]);
    }

    // ── Check overcrowd limit ────────────────────────────────────────────
    const int limit = GameConfig::instance().maxPassengersPerStation();

    if (!overcrowdNotified &&
        static_cast<int>(passengers.size()) >= limit)
    {
        overcrowded       = true;
        overcrowdNotified = true;
        notifyObservers();
    }

    // ── Advance warning timer (used by renderer for blink effect) ────────
    if (overcrowded)
        overcrowdTimer += dt;
}