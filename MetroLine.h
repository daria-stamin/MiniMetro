#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Station.h"
#include "Train.h"

class MetroLine
{
private:
    sf::Color color;

    std::vector< std::shared_ptr<Station> > stations;
    std::vector<Train> trains;


public:
    MetroLine(const sf::Color& color);

    void addStation(std::shared_ptr<Station> station);

    void addTrain();

    void update(float dt);

    const std::vector<Train>& getTrains() const;

    const std::vector<std::shared_ptr<Station>>& getStations() const;

    sf::Color getColor() const;
};