#include "SimulationEngine.h"
#include "StationFactory.h"
#include "GameConfig.h"
#include <random>
#include <cmath>
#include <algorithm>

// 6 distinct line colours, same style as Mini Metro
static const std::array<sf::Color, 6> LINE_COLORS = {
    sf::Color(231,  76,  60),   // red
    sf::Color( 52, 152, 219),   // blue
    sf::Color( 46, 204, 113),   // green
    sf::Color(241, 196,  15),   // yellow
    sf::Color(155,  89, 182),   // purple
    sf::Color(230, 126,  34),   // orange
};

namespace
{
    sf::Vector2f randomFreePosition(
        const std::vector<std::shared_ptr<Station>>& all,
        std::mt19937& gen,
        float minDist = 140.f)
    {
        std::uniform_real_distribution<float> xDist(100.f, 1100.f);
        std::uniform_real_distribution<float> yDist(100.f,  600.f);

        while (true)
        {
            sf::Vector2f pos(xDist(gen), yDist(gen));
            bool tooClose = std::any_of(all.begin(), all.end(),
                [&](const auto& s) {
                    float dx = pos.x - s->getPosition().x;
                    float dy = pos.y - s->getPosition().y;
                    return std::sqrt(dx*dx + dy*dy) < minDist;
                });
            if (!tooClose) return pos;
        }
    }
}

void SimulationEngine::initSlots()
{
    // 6 circles stacked vertically in bottom-right corner
    // x=1245 (near right edge), y from 690 going up, spaced 50px
    for (int i = 0; i < NUM_LINES; ++i)
    {
        sf::Vector2f pos(1245.f, 690.f - i * 50.f);
        slots[i] = LineSlot(LINE_COLORS[i], pos);
    }
}

SimulationEngine::SimulationEngine()
    : slots{
        LineSlot(LINE_COLORS[0], {}),
        LineSlot(LINE_COLORS[1], {}),
        LineSlot(LINE_COLORS[2], {}),
        LineSlot(LINE_COLORS[3], {}),
        LineSlot(LINE_COLORS[4], {}),
        LineSlot(LINE_COLORS[5], {}),
      }
    , stationSpawnTimer(0.f)
    , stationSpawnInterval(GameConfig::instance().stationSpawnInterval())
{
    initSlots();       // set proper HUD positions
    generateStations();
}

void SimulationEngine::generateStations()
{
    std::mt19937 gen{ std::random_device{}() };

    std::vector<StationType> types = {
        StationType::Circle, StationType::Square, StationType::Triangle
    };
    std::shuffle(types.begin(), types.end(), gen);

    for (StationType type : types)
    {
        auto pos     = randomFreePosition(stations.getAll(), gen);
        auto station = StationFactory::create(type, pos);
        station->addObserver(weak_from_this());
        stations.add(std::move(station));
    }
}

void SimulationEngine::spawnNewStation()
{
    std::mt19937 gen{ std::random_device{}() };
    std::uniform_int_distribution<int> typeDist(0, 2);

    auto pos     = randomFreePosition(stations.getAll(), gen);
    auto type    = static_cast<StationType>(typeDist(gen));
    auto station = StationFactory::create(type, pos);
    station->addObserver(weak_from_this());
    stations.add(std::move(station));
}

void SimulationEngine::handleClick(const sf::Vector2f& mousePos)
{
    if (isGameOver()) return;

    // ── Check if a HUD slot was clicked ──────────────────────────────────
    for (auto& slot : slots)
    {
        if (!slot.containsPoint(mousePos)) continue;

        if (!slot.isFree())
        {
            // Remove from renderer's list first, then clear slot
            metroLines.remove(slot.getLine());
            slot.clearLine();

            if (activeSlot == &slot)
                activeSlot = nullptr;
        }
        else
        {
            // Start drawing a new line on this free slot
            auto newLine = std::make_shared<MetroLine>(slot.getColor());
            slot.setLine(newLine);
            metroLines.add(newLine);
            activeSlot = &slot;
        }
        return;   // HUD click handled — don't check stations
    }

    // ── Otherwise check if a station was clicked ──────────────────────────
    if (!activeSlot) return;   // no line being drawn

    auto nearest = findNearest(stations, mousePos, 25.f);
    if (!nearest) return;

    activeSlot->getLine()->addStation(*nearest);
}

void SimulationEngine::finishCurrentLine()
{
    if (isGameOver()) return;
    if (!activeSlot)  return;

    activeSlot->getLine()->addTrain();
    activeSlot = nullptr;
}

void SimulationEngine::onStationOvercrowded(Station*)
{
    gameOver = true;
}

bool SimulationEngine::isGameOver() const
{
    return gameOver &&
           gameOverTimer >= GameConfig::instance().overcrowdWarningDuration();
}

int SimulationEngine::getScore() const
{
    int total = 0;
    for (const auto& line : metroLines.getAll())
        total += line->getDeliveredPassengers();
    return total;
}

int SimulationEngine::getOvercrowdedStationCount() const
{
    return stations.countIf(
        [](const Station& s) { return s.isOvercrowded(); });
}

void SimulationEngine::update(float dt)
{
    if (isGameOver()) return;

    stations.forEach([dt](Station& s)     { s.update(dt); });
    metroLines.forEach([dt](MetroLine& l) { l.update(dt); });

    if (gameOver)
        gameOverTimer += dt;

    stationSpawnTimer += dt;
    if (stationSpawnTimer >= stationSpawnInterval)
    {
        stationSpawnTimer = 0.f;
        spawnNewStation();
    }
}

const std::array<LineSlot, 6>& SimulationEngine::getSlots() const
{
    return slots;
}

const std::vector<std::shared_ptr<Station>>&
SimulationEngine::getStations()   const { return stations.getAll();   }

const std::vector<std::shared_ptr<MetroLine>>&
SimulationEngine::getMetroLines() const { return metroLines.getAll(); }