#pragma once

#include <vector>
#include <memory>
#include "MetroLine.h"
#include "Station.h"

class SimulationEngine
{
private:
    std::vector<std::shared_ptr<Station>> stations;
    std::vector< std::shared_ptr<MetroLine> > metroLines;
    std::shared_ptr<MetroLine> currentLine;

    float stationSpawnTimer;
    float stationSpawnInterval;

public:
    SimulationEngine();

    void spawnNewStation();

    void update(float dt);
    void generateStations();

    void generateMetroLine();

    void finishCurrentLine();

    const std::vector< std::shared_ptr<Station> >& getStations() const;

    const std::vector< std::shared_ptr<MetroLine> >& getMetroLines() const;
    void handleClick( const sf::Vector2f& mousePosition);

};