#include "MetroLine.h"

MetroLine::MetroLine(const sf::Color& color): color(color){

}
sf::Color MetroLine::getColor() const{
    return color;
}

void MetroLine::addStation(std::shared_ptr<Station> station){
    stations.push_back(station);
}

const std::vector<std::shared_ptr<Station>>&MetroLine::getStations() const{
    return stations;
}


void MetroLine::addTrain(){
    if (stations.size() >= 2){
        trains.push_back(Train());
    }
}

void MetroLine::update(float dt){
    for (auto& train : trains){
        train.update(static_cast<int>(stations.size()), dt);
        int currentIndex =
         train.getCurrentStationIndex();

        auto currentStation =
            stations[currentIndex];
        auto& trainPassengers =
    train.getPassengers();

        for (auto it =
             trainPassengers.begin();
             it != trainPassengers.end();)
        {
            if (it->getDestinationType()
                ==
                currentStation->getType())
            {
                it =
                    trainPassengers.erase(it);
            }
            else
            {
                ++it;
            }
        }

    }
}

const std::vector<Train>&MetroLine::getTrains() const{
    return trains;
}