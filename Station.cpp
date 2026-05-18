#include "Station.h"
#include <random>

Station::Station(const sf::Vector2f& pos)
    : position(pos) {}

sf::Vector2f Station::getPosition() const { return position; }

const std::vector<Passenger>& Station::getPassengers() const { return passengers; }
std::vector<Passenger>&       Station::getPassengers()       { return passengers; }

void Station::update(float dt)
{
    passengerSpawnTimer += dt;
    if (passengerSpawnTimer < 5.f) return;

    passengerSpawnTimer = 0.f;

    // Pick a random destination type that is NOT this station's type
    static std::mt19937 gen{ std::random_device{}() };
    static std::uniform_int_distribution<int> dist(0, 1);

    // Collect the two other types
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