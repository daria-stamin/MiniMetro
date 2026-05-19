#include "MetroLine.h"

MetroLine::MetroLine(const sf::Color& color) : color(color) {}

sf::Color MetroLine::getColor() const { return color; }
int MetroLine::getDeliveredPassengers() const { return deliveredPassengers; }

const std::vector<Train>& MetroLine::getTrains() const { return trains;  }
const std::vector<std::shared_ptr<Station>>& MetroLine::getStations() const { return stations; }

void MetroLine::addStation(std::shared_ptr<Station> station){
    stations.push_back(std::move(station));
}

void MetroLine::addTrain(){
    if (stations.size() >= 2)
        trains.push_back(Train());
}

void MetroLine::update(float dt){
    for (auto& train : trains){
        int prevIndex = train.getCurrentStationIndex();
        train.update(static_cast<int>(stations.size()), dt);
        int currentIndex = train.getCurrentStationIndex();

        auto& currentStation  = stations[currentIndex];
        auto& trainPassengers = train.getPassengers();

        // Drop off
        for (auto it = trainPassengers.begin(); it != trainPassengers.end();){
            if (it->getDestinationType() == currentStation->getType())
            {
                it = trainPassengers.erase(it);
                ++deliveredPassengers;
            }
            else ++it;
        }

        // Board (only when train just arrived at a new station)
        if (currentIndex != prevIndex){
            constexpr size_t MAX_CAPACITY = 6;
            auto& waiting = currentStation->getPassengers();

            while (!waiting.empty() && trainPassengers.size() < MAX_CAPACITY)
            {
                trainPassengers.push_back(waiting.front());
                waiting.erase(waiting.begin());
            }
        }
    }
}