#pragma once

#include <vector>
#include <memory>
#include "MetroLine.h"
#include "Station.h"

class SimulationEngine
{
    std::vector<std::shared_ptr<Station>>   stations;
    std::vector<std::shared_ptr<MetroLine>> metroLines;
    std::shared_ptr<MetroLine>              currentLine;

    float stationSpawnTimer;
    float stationSpawnInterval;

    void generateStations();
    void spawnNewStation();

public:
    SimulationEngine();

    void update(float dt);
    void handleClick(const sf::Vector2f& mousePosition);
    void finishCurrentLine();

    const std::vector<std::shared_ptr<Station>>&   getStations()   const;
    const std::vector<std::shared_ptr<MetroLine>>& getMetroLines() const;
};