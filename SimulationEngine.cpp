#include "SimulationEngine.h"

#include <random>
#include <cmath>

SimulationEngine::SimulationEngine(){
    generateStations();
    stationSpawnTimer = 0.f;
    stationSpawnInterval = 30.f;
}

float distanceBetween(const sf::Vector2f& a,const sf::Vector2f& b){
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

void SimulationEngine::generateStations(){
    std::random_device rd;

    std::mt19937 gen(rd());

    std::uniform_real_distribution<float>
        xDist(100.f, 1100.f);

    std::uniform_real_distribution<float>
        yDist(100.f, 600.f);

    std::uniform_int_distribution<int>
        typeDist(0, 2);

    while (stations.size() < 3)
    {
        sf::Vector2f newPosition( xDist(gen), yDist(gen) );

        bool tooClose = false;

        for (const auto& station : stations)
        {
            float distance = distanceBetween( newPosition, station->getPosition() );

            if (distance < 140.f){
                tooClose = true;
                break;
            }
        }

        if (!tooClose){
            StationType type = static_cast<StationType>( typeDist(gen) );
            auto station = std::make_shared<Station>( type, newPosition );
            stations.push_back(station);
        }
    }
}

const std::vector<std::shared_ptr<Station>>&SimulationEngine::getStations() const
{
    return stations;
}
void SimulationEngine::generateMetroLine()
{
    auto line =
        std::make_shared<MetroLine>(
            sf::Color(255, 80, 80)
        );

    if (stations.size() >= 4)
    {
        line->addStation(stations[0]);

        line->addStation(stations[1]);

        line->addStation(stations[2]);

        line->addStation(stations[3]);
    }

    metroLines.push_back(line);
}

const std::vector<std::shared_ptr<MetroLine>>&SimulationEngine::getMetroLines() const
{
    return metroLines;
}

void SimulationEngine::finishCurrentLine()
{
    if (currentLine)
    {
        currentLine->addTrain();
    }

    currentLine = nullptr;
}
void SimulationEngine::update(float dt)
{
    for (auto& station : stations)
    {
        station->update(dt);
    }
    for (auto& line : metroLines)
    {
        line->update(dt);
    }

    stationSpawnTimer += dt;

    if (stationSpawnTimer >= stationSpawnInterval)
    {
        stationSpawnTimer = 0.f;

        spawnNewStation();
    }
}

void SimulationEngine::handleClick(const sf::Vector2f& mousePosition)
{
    for (const auto& station : stations)
    {
        sf::Vector2f stationPos =
            station->getPosition();

        float dx =mousePosition.x - stationPos.x;

        float dy =mousePosition.y - stationPos.y;

        float distance =std::sqrt(dx * dx + dy * dy);

        if (distance < 25.f)
        {
            // daca nu exista linie activa
            if (!currentLine)
            {
                sf::Color randomColor(
            rand() % 255,
            rand() % 255,
            rand() % 255
        );

                currentLine = std::make_shared<MetroLine>(randomColor);

                metroLines.push_back( currentLine);
            }

            // adaugam statia
            currentLine->addStation(
                station
            );

            break;
        }
    }
}
void SimulationEngine::spawnNewStation(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(100.f, 1100.f);
    std::uniform_real_distribution<float> yDist(100.f, 600.f);
    std::uniform_int_distribution<int> typeDist(0, 2);

    while (true){
        sf::Vector2f newPosition(
            xDist(gen),
            yDist(gen)
        );

        bool tooClose = false;

        for (const auto& station : stations)
        {
            float distance =
                distanceBetween(
                    newPosition,
                    station->getPosition()
                );

            if (distance < 140.f)
            {
                tooClose = true;
                break;
            }
        }

        if (!tooClose)
        {
            StationType type =
                static_cast<StationType>(
                    typeDist(gen)
                );

            auto station =
                std::make_shared<Station>(
                    type,
                    newPosition
                );

            stations.push_back(station);

            break;
        }
    }
}