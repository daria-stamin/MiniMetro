#include "GameConfig.h"
#include <fstream>
#include <sstream>
#include <iostream>

GameConfig::GameConfig()
{
    std::ifstream file("data/config.txt");
    if (!file.is_open())
    {
        std::cerr << "[GameConfig] data/config.txt not found, using defaults.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line))
        parseLine(line);
}

void GameConfig::parseLine(const std::string& line)
{
    // Skip empty lines and comments
    if (line.empty() || line[0] == '#') return;

    std::istringstream ss(line);
    std::string key, eq, value;

    if (!(ss >> key >> eq >> value) || eq != "=") return;

    try
    {
        if      (key == "max_passengers_per_station") maxPassengers   = std::stoi(value);
        else if (key == "station_spawn_interval")     spawnInterval   = std::stof(value);
        else if (key == "train_speed")                trainSpd        = std::stof(value);
        else if (key == "overcrowd_warning_duration") warningDuration = std::stof(value);
    }
    catch (const std::exception& e)
    {
        std::cerr << "[GameConfig] Bad value for key '" << key << "': " << e.what() << '\n';
    }
}