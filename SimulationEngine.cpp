#include "SimulationEngine.h"
#include "StationFactory.h"
#include "GameConfig.h"
#include <random>
#include <cmath>
#include <algorithm>
#include <numeric>

namespace
{
    float distanceBetween(const sf::Vector2f& a, const sf::Vector2f& b)
    {
        float dx = a.x - b.x, dy = a.y - b.y;
        return std::sqrt(dx * dx + dy * dy);
    }

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
    : stationSpawnTimer(0.f)
    , stationSpawnInterval(GameConfig::instance().stationSpawnInterval())
{
    generateStations();
}

void SimulationEngine::generateStations()
{
    std::mt19937 gen{ std::random_device{}() };

    std::vector<StationType> types = {
        StationType::Circle,
        StationType::Square,
        StationType::Triangle
    };
    std::shuffle(types.begin(), types.end(), gen);

    for (StationType type : types)
    {
        auto pos     = randomFreePosition(stations, gen);
        auto station = StationFactory::create(type, pos);
        // Register self as observer using weak_ptr — no ownership cycle
        station->addObserver(weak_from_this());
        stations.push_back(std::move(station));
    }
}

void SimulationEngine::spawnNewStation()
{
    std::mt19937 gen{ std::random_device{}() };
    std::uniform_int_distribution<int> typeDist(0, 2);

    auto pos     = randomFreePosition(stations, gen);
    auto type    = static_cast<StationType>(typeDist(gen));
    auto station = StationFactory::create(type, pos);
    station->addObserver(weak_from_this());
    stations.push_back(std::move(station));
}

void SimulationEngine::onStationOvercrowded(Station* /*station*/)
{
    // Station already set its blink flag; we start the game-over countdown.
    // GameRenderer will show the blink for overcrowdWarningDuration seconds,
    // after which isGameOver() returns true and the game-over screen appears.
    gameOver = true;
}

bool SimulationEngine::isGameOver() const
{
    if (!gameOver) return false;
    // Only show game-over screen after the warning animation plays out
    return gameOverTimer >= GameConfig::instance().overcrowdWarningDuration();
}

int SimulationEngine::getScore() const
{
    return std::accumulate(
        metroLines.begin(), metroLines.end(), 0,
        [](int sum, const auto& line) {
            return sum + line->getDeliveredPassengers();
        });
}

void SimulationEngine::update(float dt)
{
    if (isGameOver()) return;   // freeze simulation on game over

    for (auto& station : stations)
        station->update(dt);

    for (auto& line : metroLines)
        line->update(dt);

    if (gameOver)
        gameOverTimer += dt;

    stationSpawnTimer += dt;
    if (stationSpawnTimer >= stationSpawnInterval)
    {
        stationSpawnTimer = 0.f;
        spawnNewStation();
    }
}

void SimulationEngine::handleClick(const sf::Vector2f& mousePos)
{
    if (isGameOver()) return;

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

const std::vector<std::shared_ptr<Station>>&   SimulationEngine::getStations()   const { return stations;   }
const std::vector<std::shared_ptr<MetroLine>>& SimulationEngine::getMetroLines() const { return metroLines; }