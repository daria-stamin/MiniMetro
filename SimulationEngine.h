#pragma once

#include <memory>
#include <array>
#include "Repository.h"
#include "MetroLine.h"
#include "Station.h"
#include "LineSlot.h"
#include "IGameObserver.h"

class SimulationEngine : public IGameObserver,
                         public std::enable_shared_from_this<SimulationEngine>
{
    static constexpr int NUM_LINES = 6;

    Repository<Station>        stations;
    Repository<MetroLine>      metroLines;

    // Fixed 6 slots — each holds a colour and optionally an active line
    std::array<LineSlot, NUM_LINES> slots;

    // The slot whose line is currently being drawn (nullptr = none)
    LineSlot* activeSlot = nullptr;

    float stationSpawnTimer;
    float stationSpawnInterval;

    bool  gameOver      = false;
    float gameOverTimer = 0.f;

    void generateStations();
    void spawnNewStation();
    void initSlots();

public:
    SimulationEngine();

    void update(float dt);

    // Called on LEFT click
    void handleClick(const sf::Vector2f& mousePosition);

    // Called on RIGHT click — finalises current line, spawns train
    void finishCurrentLine();

    void onStationOvercrowded(Station* station) override;

    bool isGameOver()              const;
    int  getScore()                const;
    int  getOvercrowdedStationCount() const;

    const std::array<LineSlot, NUM_LINES>& getSlots() const;

    const std::vector<std::shared_ptr<Station>>&   getStations()   const;
    const std::vector<std::shared_ptr<MetroLine>>& getMetroLines() const;
};