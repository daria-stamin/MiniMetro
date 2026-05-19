#pragma once

#include <vector>
#include <memory>
#include "MetroLine.h"
#include "Station.h"
#include "IGameObserver.h"

// SimulationEngine implements IGameObserver so it can receive
// overcrowd notifications from stations via weak_ptr — no ownership cycle.
class SimulationEngine : public IGameObserver,
                         public std::enable_shared_from_this<SimulationEngine>
{
    std::vector<std::shared_ptr<Station>>   stations;
    std::vector<std::shared_ptr<MetroLine>> metroLines;
    std::shared_ptr<MetroLine>              currentLine;

    float stationSpawnTimer;
    float stationSpawnInterval;

    bool  gameOver      = false;
    float gameOverTimer = 0.f;   // delay between blink start and full game over screen

    void generateStations();
    void spawnNewStation();

public:
    SimulationEngine();

    void update(float dt);
    void handleClick(const sf::Vector2f& mousePosition);
    void finishCurrentLine();

    // IGameObserver
    void onStationOvercrowded(Station* station) override;

    bool  isGameOver()  const;
    int   getScore()    const;   // total delivered passengers across all lines

    const std::vector<std::shared_ptr<Station>>&   getStations()   const;
    const std::vector<std::shared_ptr<MetroLine>>& getMetroLines() const;
};