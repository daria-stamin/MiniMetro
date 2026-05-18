#include "MetroLine.h"

MetroLine::MetroLine(const sf::Color& color) : color(color)
{
}

sf::Color MetroLine::getColor() const
{
    return color;
}

void MetroLine::addStation(std::shared_ptr<Station> station)
{
    stations.push_back(station);
}

const std::vector<std::shared_ptr<Station>>& MetroLine::getStations() const
{
    return stations;
}

void MetroLine::addTrain()
{
    if (stations.size() >= 2)
    {
        trains.push_back(Train());
    }
}

void MetroLine::update(float dt)
{
    for (auto& train : trains)
    {
        int prevIndex = train.getCurrentStationIndex();

        train.update(static_cast<int>(stations.size()), dt);

        int currentIndex = train.getCurrentStationIndex();

        auto currentStation = stations[currentIndex];

        // =====================
        // DROP OFF passengers whose destination matches this station
        // =====================
        auto& trainPassengers = train.getPassengers();

        for (auto it = trainPassengers.begin(); it != trainPassengers.end();)
        {
            if (it->getDestinationType() == currentStation->getType())
            {
                it = trainPassengers.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // =====================
        // BOARD passengers from this station if the train just arrived
        // (i.e. the station index changed this tick)
        // =====================
        if (currentIndex != prevIndex)
        {
            const size_t MAX_TRAIN_CAPACITY = 6;

            auto& waitingPassengers =
                currentStation->getPassengers();

            while (!waitingPassengers.empty() &&
                   trainPassengers.size() < MAX_TRAIN_CAPACITY)
            {
                // Board the first waiting passenger
                trainPassengers.push_back(waitingPassengers.front());
                waitingPassengers.erase(waitingPassengers.begin());
            }
        }
    }
}

const std::vector<Train>& MetroLine::getTrains() const
{
    return trains;
}