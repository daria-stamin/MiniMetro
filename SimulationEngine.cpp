#include "SimulationEngine.h"
#include "StationFactory.h"
#include <random>
#include <cmath>
#include <algorithm>

namespace
{
    float distanceBetween(const sf::Vector2f& a, const sf::Vector2f& b)
    {
        float dx = a.x - b.x, dy = a.y - b.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    // Returns a random position at least minDist away from all existing stations
    sf::Vector2f randomFreePosition(
        const std::vector<std::shared_ptr<Station>>& stations,
        std::mt19937& gen,
        float minDist = 140.f)
    {
        std::uniform_real_distribution<float> xDist(100.f, 1100.f);
        std::uniform_real_distribution<float> yDist(100.f,  600.f);

        while (true)
        {
            sf::Vector2f pos(xDist(gen), yDist(gen));
            bool tooClose = std::any_of(
                stations.begin(), stations.end(),
                [&](const auto& s) {
                    return distanceBetween(pos, s->getPosition()) < minDist;
                });
            if (!tooClose) return pos;
        }
    }
}

SimulationEngine::SimulationEngine()
    : stationSpawnTimer(0.f), stationSpawnInterval(30.f)
{
    generateStations();
}

void SimulationEngine::generateStations()
{
    std::mt19937 gen{ std::random_device{}() };

    // Guaranteed: one of each type, in shuffled order
    std::vector<StationType> types = {
        StationType::Circle,
        StationType::Square,
        StationType::Triangle
    };
    std::shuffle(types.begin(), types.end(), gen);

    for (StationType type : types)
    {
        auto pos = randomFreePosition(stations, gen);
        stations.push_back(StationFactory::create(type, pos));
    }
}

void SimulationEngine::spawnNewStation()
{
    std::mt19937 gen{ std::random_device{}() };
    std::uniform_int_distribution<int> typeDist(0, 2);

    auto pos  = randomFreePosition(stations, gen);
    auto type = static_cast<StationType>(typeDist(gen));
    stations.push_back(StationFactory::create(type, pos));
}

void SimulationEngine::update(float dt)
{
    for (auto& station : stations)
        station->update(dt);

    for (auto& line : metroLines)
        line->update(dt);

    stationSpawnTimer += dt;
    if (stationSpawnTimer >= stationSpawnInterval)
    {
        stationSpawnTimer = 0.f;
        spawnNewStation();
    }
}

void SimulationEngine::handleClick(const sf::Vector2f& mousePos)
{
    for (const auto& station : stations)
    {
        float dx = mousePos.x - station->getPosition().x;
        float dy = mousePos.y - station->getPosition().y;

        if (std::sqrt(dx * dx + dy * dy) < 25.f)
        {
            if (!currentLine)
            {
                std::mt19937 gen{ std::random_device{}() };
                std::uniform_int_distribution<int> cd(60, 240);
                currentLine = std::make_shared<MetroLine>(
                    sf::Color(cd(gen), cd(gen), cd(gen)));
                metroLines.push_back(currentLine);
            }
            currentLine->addStation(station);
            break;
        }
    }
}

void SimulationEngine::finishCurrentLine()
{
    if (currentLine)
        currentLine->addTrain();
    currentLine = nullptr;
}

const std::vector<std::shared_ptr<Station>>&    SimulationEngine::getStations()   const { return stations;   }
const std::vector<std::shared_ptr<MetroLine>>&  SimulationEngine::getMetroLines() const { return metroLines; }