#include "Station.h"

Station::Station(StationType type,const sf::Vector2f& position):type(type), position(position){
    passengerSpawnTimer = 0.f;
}

StationType Station::getType() const{
    return type;
}

sf::Vector2f Station::getPosition() const{
    return position;
}

const std::vector<Passenger>&Station::getPassengers() const{
    return passengers;
}

void Station::addPassenger(const Passenger& passenger){
    passengers.push_back(passenger);
}
sf::Color Station::getColor() const{
    return color;
}
void Station::update(float dt)
{
    passengerSpawnTimer += dt;

    if (passengerSpawnTimer >= 5.f)
    {
        passengerSpawnTimer = 0.f;

        StationType randomType;

        do
        {
            randomType =
                static_cast<StationType>(
                    rand() % 3
                );
        }
        while (randomType == type);

        passengers.push_back(
            Passenger(randomType)
        );
    }
}